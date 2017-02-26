#include <DetectionBasedTracker_jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/detection_based_tracker.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <android/asset_manager.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <vector>
#include <iostream>

#include <android/log.h>

#define LOG_TAG "FaceDetection/DetectionBasedTracker"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;

inline void vector_Rect_to_Mat(vector<Rect> &v_rect, Mat &mat) {
    mat = Mat(v_rect, true);
}

JNIEXPORT jlong JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeCreateObject
        (JNIEnv *jenv, jclass, jstring jFileName, jint faceSize) {
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeCreateObject enter");
    const char *jnamestr = jenv->GetStringUTFChars(jFileName, NULL);
    string stdFileName(jnamestr);
    jlong result = 0;

    try {
        DetectionBasedTracker::Parameters DetectorParams;
        if (faceSize > 0)
            DetectorParams.minObjectSize = faceSize;
        result = (jlong) new DetectionBasedTracker(stdFileName, DetectorParams);
    }
    catch (cv::Exception &e) {
        LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if (!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...) {
        LOGD("nativeCreateObject caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je,
                       "Unknown exception in JNI code of DetectionBasedTracker.nativeCreateObject()");
        return 0;
    }

    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeCreateObject exit");
    return result;
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDestroyObject
        (JNIEnv *jenv, jclass, jlong thiz) {
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDestroyObject enter");
    try {
        if (thiz != 0) {
            ((DetectionBasedTracker *) thiz)->stop();
            delete (DetectionBasedTracker *) thiz;
        }
    }
    catch (cv::Exception &e) {
        LOGD("nativeestroyObject caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if (!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...) {
        LOGD("nativeDestroyObject caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je,
                       "Unknown exception in JNI code of DetectionBasedTracker.nativeDestroyObject()");
    }
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDestroyObject exit");
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStart
        (JNIEnv *jenv, jclass, jlong thiz) {
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStart enter");
    try {
        ((DetectionBasedTracker *) thiz)->run();
    }
    catch (cv::Exception &e) {
        LOGD("nativeStart caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if (!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...) {
        LOGD("nativeStart caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeStart()");
    }
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStart exit");
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStop
        (JNIEnv *jenv, jclass, jlong thiz) {
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStop enter");
    try {
        ((DetectionBasedTracker *) thiz)->stop();
    }
    catch (cv::Exception &e) {
        LOGD("nativeStop caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if (!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...) {
        LOGD("nativeStop caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeStop()");
    }
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStop exit");
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeSetFaceSize
        (JNIEnv *jenv, jclass, jlong thiz, jint faceSize) {
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeSetFaceSize enter");
    try {
        if (faceSize > 0) {
            DetectionBasedTracker::Parameters DetectorParams = \
            ((DetectionBasedTracker *) thiz)->getParameters();
            DetectorParams.minObjectSize = faceSize;
            ((DetectionBasedTracker *) thiz)->setParameters(DetectorParams);
        }
    }
    catch (cv::Exception &e) {
        LOGD("nativeStop caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if (!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...) {
        LOGD("nativeSetFaceSize caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je,
                       "Unknown exception in JNI code of DetectionBasedTracker.nativeSetFaceSize()");
    }
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeSetFaceSize exit");
}


JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDetect
        (JNIEnv *jenv, jclass, jlong thiz, jlong imageGray, jlong faces) {
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDetect enter");
    try {
        vector<Rect> RectFaces;
        ((DetectionBasedTracker *) thiz)->process(*((Mat *) imageGray));
        ((DetectionBasedTracker *) thiz)->getObjects(RectFaces);
        vector_Rect_to_Mat(RectFaces, *((Mat *) faces));
    }
    catch (cv::Exception &e) {
        LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if (!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...) {
        LOGD("nativeDetect caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, "Unknown exception in JNI code DetectionBasedTracker.nativeDetect()");
    }
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDetect exit");
}


CvLatentSvmDetector *detector;

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeInitialize
        (JNIEnv *jenv, jclass, jstring fileNameStr) {

//    jobject assetManager = state_param->activity->assetManager;
//    AAssetManager* mgr = AAssetManager_fromJava(jenv, assetManager);
//
//    AAssetDir* assetDir = AAssetManager_openDir(mgr, "");
//    const char* filename = (const char*)NULL;
//    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) {
//        AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_STREAMING);
//        char buf[BUFSIZ];
//        int nb_read = 0;
//        FILE* out = fopen(filename, "w");
//        while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0)
//            fwrite(buf, nb_read, 1, out);
//        fclose(out);
//        AAsset_close(asset);
//    }
//    AAssetDir_close(assetDir);

    const char *fileName = jenv->GetStringUTFChars(fileNameStr, NULL);
    CvLatentSvmDetector *detector = cvLoadLatentSvmDetector(fileName);
    LOGD("Checking detector =" + detector->num_components + (detector != NULL) ? "NOT NULL"
                                                                               : "NULL");
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeObjectDetect(
        JNIEnv *jenv, jclass) {
    IplImage *image;

}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeObjectDestory(
        JNIEnv *jenv, jclass) {
    cvReleaseLatentSvmDetector(&detector);
}


JNIEXPORT jfloatArray JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDetectObject
        (JNIEnv *jenv, jclass, jstring jFileName, jstring imagePath) {

    const char *model_filename = jenv->GetStringUTFChars(jFileName, NULL);
    const char *image_filename = jenv->GetStringUTFChars(imagePath, NULL);

    IplImage *image = cvLoadImage(image_filename);
    CvLatentSvmDetector *detector = cvLoadLatentSvmDetector(model_filename);

    LOGD("DetectObject Comes here size=%d,%d ", image->height,detector->num_components);
    CvMemStorage *storage = cvCreateMemStorage(0);
    LOGD("DetectObject Comes here 2");
    CvSeq *detections = 0;
    LOGD("DetectObject Comes here 3");
    int i = 0;
    int64 start = 0, finish = 0;

    start = cvGetTickCount();
    LOGD("DetectObject Comes here 4");
    detections = cvLatentSvmDetectObjects(image, detector, storage, 0.5f, 10);
    LOGD("DetectObject Comes here 5");
    finish = cvGetTickCount();

    LOGD("DetectObject detection time = %.3f\n",
         (float) (finish - start) / (float) (cvGetTickFrequency() * 1000000.0));
    float time = (float) (finish - start) / (float) (cvGetTickFrequency() * 1000000.0);
//    printf("detection time = %.3f\n",
//           (float) (finish - start) / (float) (cvGetTickFrequency() * 1000000.0));
//
//    LOGD("Checking detector =" + detector->num_components + (detector != NULL) ? "NOT NULL"
//                                                                               : "NULL");
//
//    CvFont *font = new CvFont;
//    cvInitFont(font, CV_FONT_VECTOR0, 0.4f, 0.4f, 0, 1, 8); //rate of width and height is 1:2
//
//
//
//

    jint arSize = detections->total + 1;
    jfloatArray result = jenv->NewFloatArray(arSize);
    jfloat returnValues[arSize];
    //string returnValue = Java_org_opencv_samples_facedetect_DetectionBasedTracker_tostr(time);
    returnValues[0] = time;

    LOGD("DetectObject size =%d",detections->total);
    for (i = 0; i < detections->total; i++) {
        CvObjectDetection detection = *(CvObjectDetection *) cvGetSeqElem(detections, i);
        float score = detection.score;
        returnValues[i+1] = score;
        LOGD("DetectObject detection score = %lf\n",score);

//        CvRect bounding_box = detection.rect;
//        cvRectangle(image, cvPoint(bounding_box.x, bounding_box.y),
//                    cvPoint(bounding_box.x + bounding_box.width,
//                            bounding_box.y + bounding_box.height),
//                    CV_RGB(cvRound(255.0f * score * -1), 0, 0), 1);
//        printf("[%d] - %d %d %d %d : %lf\n", i, bounding_box.x, bounding_box.y, bounding_box.width,
//               bounding_box.height, score);
//
//        char szText[255];
//        sprintf(szText, "[%d]:%lf", i, score);  //make string
//        cvPutText(image, szText, cvPoint(bounding_box.x, bounding_box.y + 10),
//                  font, CV_RGB(255, 255, 255)); //draw text on the IplImage* (Image)
//
    }
    jenv->SetFloatArrayRegion(result, 0, arSize, returnValues);
    cvReleaseMemStorage(&storage);
    return result;
}

template <typename T> JNIEXPORT string JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_tostr(const T& t) {
    ostringstream os;
    os<<t;
    return os.str();
}



