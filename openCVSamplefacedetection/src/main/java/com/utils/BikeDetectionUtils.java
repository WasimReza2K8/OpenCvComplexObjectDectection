
package com.utils;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by bjit-4 on 1/26/17.
 */

public class BikeDetectionUtils {



    private static File createFileFromInputStream(String fileName, InputStream inputStream) {

        try {
            File f = new File(fileName);
            OutputStream outputStream = new FileOutputStream(f);
            byte buffer[] = new byte[1024];
            int length = 0;

            while ((length = inputStream.read(buffer)) > 0) {
                outputStream.write(buffer, 0, length);
            }

            outputStream.close();
            inputStream.close();

            return f;
        } catch (IOException e) {
            //Logging exception
        }

        return null;
    }

    public static File getSourceFile(Context context, int id, String name, String folder) {
        File cascadeDir = context.getDir(folder, Context.MODE_PRIVATE);
        File file = new File(cascadeDir, name);
        boolean existed = true;
        FileInputStream fis = null;
        try {
            fis = new FileInputStream(file);
        } catch (FileNotFoundException e) {
            existed = false;
        } finally {
            try {
                fis.close();
            } catch (Exception e) {
            }
        }
        if (existed == true)
            return file;

        try {
            InputStream is = context.getResources().openRawResource(id);
            FileOutputStream os = new FileOutputStream(file);

            byte[] buffer = new byte[4096];
            int bytesRead;
            while ((bytesRead = is.read(buffer)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
            is.close();
            os.close();
        } catch (IOException e) {
            e.printStackTrace();
            Log.e("BICYCLE", "Failed to load file " + name + ". Exception thrown: " + e);
        }

        return file;
    }

    public static void writeLog() {
        try {
            File file = new File(Environment.getExternalStorageDirectory() + File.separator + "logcat.txt");
            file.createNewFile();
            Runtime.getRuntime().exec("logcat -c");
            Runtime.getRuntime().exec("logcat -v time -f " + file.getAbsolutePath());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
