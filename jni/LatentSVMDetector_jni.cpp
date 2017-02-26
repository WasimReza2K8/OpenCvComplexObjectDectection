//
// Created by bjit-4 on 1/30/17.
//
#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/contrib/detection_based_tracker.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <android/asset_manager.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <vector>

#include <android/log.h>

#define LOG_TAG "FaceDetection/DetectionBasedTracker"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_LatentSvmTracker_nativeDetectObject
        (JNIEnv *jenv, jclass, jstring jFileName, jstring imagePath) {

    const char *model_filename = jenv->GetStringUTFChars(jFileName, NULL);
    const char *image_filename = jenv->GetStringUTFChars(imagePath, NULL);
    jstring returnString;

    IplImage *image = cvLoadImage(image_filename);
    CvLatentSvmDetector *detector = cvLoadLatentSvmDetector(model_filename);


    CvMemStorage *storage = cvCreateMemStorage(0);
    CvSeq *detections = 0;
    int i = 0;
    int64 start = 0, finish = 0;

    start = cvGetTickCount();
    detections = cvLatentSvmDetectObjects(image, detector, storage, 0.5f, 100);
    finish = cvGetTickCount();
    printf("detection time = %.3f\n",
           (float) (finish - start) / (float) (cvGetTickFrequency() * 1000000.0));
    jfloat time = (float) (finish - start) / (float) (cvGetTickFrequency() * 1000000.0);
    char buf[64]; // assumed large enough to cope with result

    sprintf(buf, "%d", time);  // error checking omitted

    returnString =  jenv->NewStringUTF(buf);
    strcat("-", jenv->GetStringUTFChars(returnString, NULL));

    LOGD("Checking detector =" + detector->num_components + (detector != NULL) ? "NOT NULL"
                                                                               : "NULL");

//    CvFont *font = new CvFont;
//    cvInitFont(font, CV_FONT_VECTOR0, 0.4f, 0.4f, 0, 1, 8); //rate of width and height is 1:2




    for (i = 0; i < detections->total; i++) {
        CvObjectDetection detection = *(CvObjectDetection *) cvGetSeqElem(detections, i);
        float score = detection.score;
       // String s = (String) detection.score;
        LOGD("Native detector score = %lf",score);
       // strcat(score, jenv->GetStringUTFChars(returnString, NULL));
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

    }
    cvReleaseMemStorage(&storage);
}

