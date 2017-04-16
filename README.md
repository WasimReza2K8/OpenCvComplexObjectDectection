# OpenCvComplexObjectDectection
This repo is used to upload opencv complex object detection with latent SVM in android.
For complex object detection like bicycle, car etc. in android phone this example may be helpful. Clone or download the code from here. Then open it with android studio. This example using opencv version 2.4.11. It can be replaced by 3.1.0 version. This example initially used to detect bicycle but you can use it to detect car, bus and many other complex objects. For car detection you need to change the following code of BdActivity line 64.
```java 
mXmlFile = BikeDetectionUtils.getSourceFile(BdActivity.this, R.raw.bicycle, "bicycle.xml", "model");
```
There are many files for object detection. Suppose, if you want to detect car you can use the following code instead of the above code.
```java 
mXmlFile = BikeDetectionUtils.getSourceFile(BdActivity.this, R.raw.car, "car.xml", "model");
```
![alt tag](https://github.com/WasimReza2K8/OpenCvComplexObjectDectection/blob/master/openCVSamplefacedetection/src/main/res/raw/detect_bicycle.png)
![alt tag](https://github.com/WasimReza2K8/OpenCvComplexObjectDectection/blob/master/openCVSamplefacedetection/src/main/res/raw/detect_car.png)

The detection time can be various upon some factors like camera quality, background, angle etc. For object detection here latent SVM has been implemented. It is bit slow process. It will take around 5 - 12 sec to detect the object. This is the android implementation of [this xmls.](https://github.com/AlexeyAB/OpenCV-detection-models/tree/master/latentsvmdetector)

# References
* https://github.com/AlexeyAB/OpenCV-detection-models 
* http://study.marearts.com/2015/04/cat-detection-using-latent-svm-in-opencv.html
