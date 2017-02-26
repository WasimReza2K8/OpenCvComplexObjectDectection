
package org.opencv.samples.facedetect;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.utils.BikeDetectionUtils;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Mat;

import java.io.File;
import java.io.FileOutputStream;

public class BdActivity extends Activity implements CvCameraViewListener2 {

    private static final String TAG = "OCVSample::Activity";
    private Mat mRgba;
    private Mat mGray;
    private DetectionBasedTracker tracker;
    private File mXmlFile;
    private File bicycleFile;
    private String timeScoreStr;

    private CameraBridgeViewBase mOpenCvCameraView;
    private static final int PICK_IMAGE = 400;

    private TextView mStatusView;
    private ProgressBar mProgressBar;
    private SharedPreferences sp;
    private SharedPreferences.OnSharedPreferenceChangeListener ospcl;

    private final String KEY_EXECUTED = "key_executed";
    private boolean isFirstTime = true;
    private ImageView imageView;

    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                    Log.i(TAG, "OpenCV loaded successfully");
                    System.loadLibrary("detection_based_tracker");

                    try {
                        mXmlFile = BikeDetectionUtils.getSourceFile(BdActivity.this, R.raw.bicycle,
                                "bicycle.xml", "model");
                    } catch (Exception e) {
                        e.printStackTrace();
                        Log.e(TAG, "Failed to load xml. Exception thrown: " + e);
                    }

                    mOpenCvCameraView.enableView();

                    break;

                default:
                    super.onManagerConnected(status);
                    break;
            }
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.activity_bike_detection);

        mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.fd_activity_surface_view);
        mOpenCvCameraView.setCvCameraViewListener(this);
        imageView = (ImageView) findViewById(R.id.imageView);

        mStatusView = (TextView) findViewById(R.id.statusView);
        mProgressBar = (ProgressBar) findViewById(R.id.progressBar);

        sp = PreferenceManager.getDefaultSharedPreferences(this);
        ospcl = new SharedPreferences.OnSharedPreferenceChangeListener() {
            @Override
            public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
                if (mRgba != null) {
                    bitmap = getBitmapFromMat();
                    bicycleFile = getFile(bitmap);

                    if (key.equalsIgnoreCase(KEY_EXECUTED)) {
                        if (bicycleFile != null & bicycleFile.exists()) {
                            mProgressBar.setVisibility(View.VISIBLE);
                            mStatusView.setText("Hello");
                            detect(mXmlFile.getAbsolutePath(), bicycleFile.getAbsolutePath(),
                                    bitmap);
                        }
                    }
                }
            }
        };
        sp.registerOnSharedPreferenceChangeListener(ospcl);
    }

    @Override
    public void onPause() {
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onResume() {
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_11, this, mLoaderCallback);
//        initOpenCV(OpenCVLoader.OPENCV_VERSION_2_4_11, this, mLoaderCallback);
    }


    @Override
    public void onDestroy() {
        super.onDestroy();
        mOpenCvCameraView.disableView();
        sp.unregisterOnSharedPreferenceChangeListener(ospcl);
    }

    public void onCameraViewStarted(int width, int height) {
        mGray = new Mat();
        mRgba = new Mat();
    }

    public void onCameraViewStopped() {
        mGray.release();
        mRgba.release();
    }

    Bitmap bitmap;

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
        mRgba = inputFrame.rgba();
        mGray = inputFrame.gray();

        if (isFirstTime) {
            isFirstTime = false;
            bitmap = getBitmapFromMat();
            bicycleFile = getFile(bitmap);
            Log.i(TAG, "Bicycle File =" + (bicycleFile != null));
            detect(mXmlFile.getAbsolutePath(), bicycleFile.getAbsolutePath(), bitmap);
        }

        return mRgba;
    }

    private Bitmap getBitmapFromMat() {
        Bitmap bitmap = Bitmap.createBitmap(mOpenCvCameraView.getWidth() / 4,
                mOpenCvCameraView.getHeight() / 4, Bitmap.Config.ARGB_8888);
        try {
            bitmap = Bitmap.createBitmap(mRgba.cols(), mRgba.rows(), Bitmap.Config.ARGB_8888);
            Utils.matToBitmap(mRgba, bitmap);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return bitmap;
    }

    private File getFile(Bitmap bitmap) {

        String root = Environment.getExternalStorageDirectory().toString();
        File myDir = new File(root + "/Bike_Detection");
        myDir.mkdirs();

        String fname = "Bike.jpg";
        File file = new File(myDir, fname);
        if (file.exists())
            file.delete();
        try {
            FileOutputStream out = new FileOutputStream(file);
            bitmap.compress(Bitmap.CompressFormat.JPEG, 90, out);
            out.flush();
            out.close();

        } catch (Exception e) {
            e.printStackTrace();
        }

        return file;
    }

    public void detect(final String fileName, final String imagePath, final Bitmap bitmap) {

        new AsyncTask<Void, Void, Void>() {
            private float[] values;
            // private DetectionBasedTracker tracker;

            @Override
            protected void onPreExecute() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mProgressBar.setVisibility(View.VISIBLE);
                    }
                });
            }

            @Override
            protected Void doInBackground(Void... voids) {
                values = DetectionBasedTracker.getInstance().detect(fileName, imagePath);
                return null;
            }

            @Override
            protected void onPostExecute(Void aVoid) {
                PreferenceManager.getDefaultSharedPreferences(BdActivity.this).edit()
                        .putLong(KEY_EXECUTED, System.currentTimeMillis()).commit();
                timeScoreStr = "";
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mProgressBar.setVisibility(View.INVISIBLE);

                        if (values != null) {
                            if (bitmap != null) {
                                imageView.setImageBitmap(bitmap);
                            }
                            mStatusView.setText("Status: " + bikeDetected(values) + "  Time Taken: "
                                    + values[0]);
                        }
                    }
                });

            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    public void selectImage(View v) {
        Intent intent = new Intent();
        intent.setType("image/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(Intent.createChooser(intent, "Select Picture"), PICK_IMAGE);
    }

    private String bikeDetected(float[] values) {
        String status = "Not detected";

        if (values.length > 1 && getMax(values) > -0.5) {
            status = "Detected";
        }

        return status;
    }

    private float getMax(float[] score) {
        float max = score[1];
        for (int i = 1; i < score.length; i++) {
            if (score[i] > max) {
                max = score[i];
            }
        }
        return max;
    }
}
