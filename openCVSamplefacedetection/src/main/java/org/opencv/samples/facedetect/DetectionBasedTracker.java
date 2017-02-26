package org.opencv.samples.facedetect;

import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;

public class DetectionBasedTracker {

    private static DetectionBasedTracker sDetectionBasedTracker;
    private DetectionBasedTracker(){}

    public static DetectionBasedTracker getInstance(){
        if(sDetectionBasedTracker == null){
            sDetectionBasedTracker = new DetectionBasedTracker();
        }
        return sDetectionBasedTracker;
    }

    public void start() {
        nativeStart(mNativeObj);
    }

    public void stop() {
        nativeStop(mNativeObj);
    }

    public void setMinFaceSize(int size) {
        nativeSetFaceSize(mNativeObj, size);
    }

    public void detect(Mat imageGray, MatOfRect faces) {
        nativeDetect(mNativeObj, imageGray.getNativeObjAddr(), faces.getNativeObjAddr());
    }


    public float[] detect(final String fileName, final String imagePath) {
       return nativeDetectObject(fileName, imagePath);

    }


    public void release() {
        nativeDestroyObject(mNativeObj);
        mNativeObj = 0;
    }

    private long mNativeObj = 0;

    private static native long nativeCreateObject(String cascadeName, int minFaceSize);

    private static native void nativeDestroyObject(long thiz);

    private static native void nativeStart(long thiz);

    private static native void nativeStop(long thiz);

    private static native void nativeSetFaceSize(long thiz, int size);

    private static native void nativeDetect(long thiz, long inputImage, long faces);

    private static native void nativeInitialize(String fileName);

    private static native float[] nativeDetectObject(String fileName, String imagePath);
}
