#pragma once
#define DLIB

//#include "stdafx.h"

#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/ml.hpp"
#include "opencv2/core/types.hpp"

#include <cmath>
#include <queue>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>   
#include <iostream>  
#include <sstream>
#include <stack>
#include <algorithm>

//#include "opencv2/core/types_c.h"

#ifdef DLIB
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>

using namespace dlib;
#endif

using namespace cv;
using namespace std;
using namespace ml;

void hDelay(double DELAY_TIME, string MESSAGE);

void SVMtest();

void fakeTrackbar(int, void*);

void trainBySVM(Mat trainingData, Mat trainingClasses, int SVM_KERNEL_TYPE, int term_max_count = 100, double term_epsilon = 1e-6);

string type2str(int type);

std::vector<float> predictBySVM(Mat testData, Mat testClasses, bool DISPLAY_RESULT = false);

Point getPoint(std::vector<full_object_detection> shapes, int part_number);

double facePointDist(std::vector<full_object_detection> shapes, int p1, int p2, bool DISPLAY_RESULT = true);

double getDistRatioH(std::vector<full_object_detection> shapes, int p1, int p2, bool DISPLAY_RESULT = true); // For Horizontal Lines

double getDistRatioV(std::vector<full_object_detection> shapes, int p1, int p2, bool DISPLAY_RESULT = true); // For Verticaal Lines

double getAlpha(std::vector<full_object_detection> shapes);

Mat getLabelMat(int no_of_mats, std::vector<int> vector_of_lables, int repeat_count = 100);
