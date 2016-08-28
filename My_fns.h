
#ifndef MY_FNS_CA5h
#define MY_FNS_CA5h
//#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <deque>
#include <vector>
#include <cmath>
#include<string>
#include <sstream>

using namespace std;
using namespace cv;
void Delay(double DELAY_TIME, string MESSAGE);
	
void Thresholding(const Mat& I, Mat& X, int numB,const uchar*boundary,const uchar* level, uchar channel);

void Resize_pix( Mat I, int win_a, int  win_b, const int =320);


void Resize_pix(Mat I, int win_a, int  win_b,int lower_threshold,int upper_threshold, const int = 320);


struct LipPoints
{
	LipPoints(int u = 10, int v = 10)
	{
		x = u;
		y = v;
	}

	void setLipPoints(int u , int v )
	{
		x = u;
		y = v;
	}

	int x;
	int y;

};


class LipFeatures
{
	public:
		LipFeatures ();
	void setElementSize(int buffer_size = 5,int ratio_vector_size=4,int diff_vector_size=4);
	void updatePoint(int point_num,int x_val=DEFAULT,int y_val=DEFAULT);
	void setNumOfPoints(int num=7);
	void setRatioFeaturesSize(int sample_size,int vector_size);
	void setDiffFeatureSize(int sample_size,int vector_size);
	void calcFeatureVectorsDiff();
	void calcFeatureVectorsRatio();
	double mod(int i=0, int j=0);
	int diffX(int i = 0, int j = 0);
	int diffY(int i = 0, int j = 0);
	 void showRatioVector(int i);
	 void clearMessageMat();
	 void resetEverything(int num_of_points,int buffer_size , int ratio_vector_size , int diff_vector_size );
	 void resetEverything();
	 void addVectorToTD();
	 

	 void showParams();
	static void noteDiscard(int , void*);
	static void showSamples(int, void*);
	void addVectorToTD(int trn_mat_row, int sample_row);
	void createTrainingDataForOneClass();
	void updateIMGLOC (string extension_str);
	static void haltFunction(int, void*);
	void getCurrentVectorMat(cv::Mat& dest_mat, int mat_id);
	void calcVectorsAuxi_1();
	void calcAllVectors();
	void showAuxiVector_1(int last_n_vectors);
	//inline void setDFeatures();
	//inline void setRFeatures();

	static int DEFAULT;
	static int LIP_POINT_DEFAULT;

	std::vector<LipPoints>  points_array;
	deque<std::vector<double> > ratio_features;
	deque<std::vector<double> > ratio_features_hidden;//stores all the ratio_features' vectors calculated so far.
	deque<std::vector<double> > ratio_features_auxi_1;
	//modified by: setElementSize(),
	deque<std::vector <int> > diff_features;
	bool first_initialization;
	bool first_call_setNumOfPoints;
	stringstream ss;
	string temp_message;
	cv::Mat message_mat;
	cv::Mat original_mat;
	
	cv::Mat uniclass_vector_data;
	cv::Mat training_mat;
	cv::Mat classes; // analogous to labels
	cv::Mat labels;//sized=(num_samples_per_class*num_classes) x num_op_elements (differnt op vectors)

	cv::Mat training_mat_auxi_1;
	cv::Mat current_fv_mat;
	cv::Mat current_fv_mat_auxi_1;
	cv::Mat recent_AuxiMat_1;
	static int DEFAULT_NUM_CLASSES;
	static int DEFAULT_NUM_ELEMENTS;
	static int DEFAULT_NUM_SAMPLES_PER_CLASS;
	static int DEFAULT_NUM_OP_ELEMENTS;

	static int discard_count;
	static int sample_number;
	static std::vector<int> discard_samples;
	static string my_temp_str;
	static int halt_flag;
	static int NUM_OF_EL_AUXI_1;//set only in cnstr(do not modify it in any other function):number of elements in auxilliary vector 1.
	static unsigned int sampling_factor; // default:2; sampling_factor
private:
int num_samples_per_class;
int num_classes;
int num_elements;
int num_op_elements;//nodes in output layer (for SVM =1)
int current_BS;
int current_DiffS;
int current_RatioS;//same as num_elements
static string img_loc;// C:/Users/shubham/Desktop/test/vid/img_num_
static string img_vect_loc;// C:/Users/shubham/Desktop/test/vid/img_vect_num_
//static stringstream sss;
static string my_image_window;
static string vector_window;
double alpha_vh;

//used in showsample()

};


 
#endif