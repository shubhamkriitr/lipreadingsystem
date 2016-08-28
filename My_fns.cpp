#include "stdafx.h"
#include"My_fns.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <deque>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;
//using namespace cv;
//void Delay(double DELAY_TIME);

//void Thresholding(const Mat& I, Mat& X, int numB, const uchar*boundary, const uchar* level, uchar channel);

void Delay(double DELAY_TIME,string MESSAGE)
{
	double t = (double) cv::getCPUTickCount();
	double f = (double)cv::getTickFrequency();
	//cout << "t=" << t << endl;
	//cout << "f=" << f << endl;
	cout <<"Info:"<< MESSAGE << endl;
	//cout <<"t/f="<< t / f << endl;
	cout << "-------Wait for " << DELAY_TIME << " secs------" << endl;
	
	while (((cv::getCPUTickCount() - t) / f) < DELAY_TIME)
	{

	}
}
//numB =4 for boundary={0,50,100,255}// 0 and 255 are fixed; level size is assumed to be numB-1;

void Thresholding(cv:: Mat& I, cv::Mat& X,int numB,const uchar*boundary,const uchar*level,uchar channel)
{
	int nch = I.channels();
	if (nch == 3) {
		cout << "Exc" << endl;
		if (channel == 1)
			for (int i = 0; i < I.rows; ++i)
				for (int j = 0; j < I.cols; j += 1)
				{
					for (int k = 0; k < (numB - 1); k++)
						if (((I.at<uchar>(i, (j - 1)*nch + 3)) >= boundary[k ]) &&
							((I.at<uchar>(i, (j - 1)*nch + 3)) < boundary[k + 1]))

							X.at<uchar>(i, j) = level[k];

				}
		else if (channel == 2)
			for (int i = 0; i < I.rows; ++i)
				for (int j = 0; j < I.cols; j += 1)
				{
					for(int k = 0; k < (numB - 1); k++)
						if (((I.at<uchar>(i, (j - 1)*nch + 4)) >= boundary[k ]) &&
							((I.at<uchar>(i, (j - 1)*nch + 4)) < boundary[k + 1]))

							X.at<uchar>(i, j) = level[k];

				}
		else if (channel == 3)
			for (int i = 0; i < I.rows; ++i)
				for (int j = 0; j < I.cols; j += 1)
				{
					for (int k = 0; k < (numB - 1); k++)
						if (((I.at<uchar>(i, (j - 1)*nch + 5)) >= boundary[k ]) &&
							((I.at<uchar>(i, (j - 1)*nch + 5)) < boundary[k + 1]))

							X.at<uchar>(i, j) = level[k];

				}
		else cout << "INVALID CHANNEL NUMBER" << endl;
	}
	else cout << "Number of source channels is not 3"<<endl;
}


void Resize_pix( cv::Mat I, int win_a, int  win_b,int D_WIDTH)
{
	if (I.channels() == 1)
	{
		if (I.rows > 0 && I.cols > 0)
		{
			int temp_pix = 0;
			int box_i = 0;
			int box_j = 0;
			int win_area = win_a*win_b;
			//static Mat In_I;
			cv::Mat In_I;
			//Mat small_I;
			//cout << "depth of I=" << I.depth() << endl;


			//int Box_rows = (I.rows - (I.rows)%win_a)/win_a;
			//int Box_cols = (I.cols - (I.cols) % win_b)/win_b;


			int Box_rows = I.rows / win_a;

			//float B_r = (float)(I.rows /win_a);

			int Box_cols = I.cols / win_b;

			//cout << "B_r=" <<B_r<< endl;

			//cout << "Box_r=" << Box_rows << endl;
			//cout << "Box_cols=" << Box_cols << endl;

			int Box_area = Box_rows*Box_cols;
			cv::Mat Box(Box_rows,Box_cols,CV_8UC1,cv::Scalar(0));
			//Delay(5,"Fnished Initialization");
			/*TO RESIZE
			float scale_up = (float)D_WIDTH / (I.cols);

			unsigned int scale_height = 1;
			if (I.cols > D_WIDTH)
			{
				scale_height = cvRound((I.rows)*scale_up);
				resize(I, small_I, Size(D_WIDTH, scale_height));
			}*/

			integral(I, In_I, -1);

			

			//Box Pixel Values
			
			for(int i=0;i<Box.rows;i++)
				for (int j = 0; j < Box.cols; j++)
				{
					box_i = i*win_a;
					box_j = j*win_b;
					temp_pix = (In_I.at<int>(box_i + win_a, box_j + win_b) + In_I.at<int>(box_i, box_j)
						- In_I.at<int>(box_i, box_j + win_b) - In_I.at<int>(box_i + win_a, box_j)) / (win_area);

					Box.at<uchar>(i, j) = (uchar)temp_pix;
				}
			//cout << "In_I=" << endl << In_I << endl;
			//cout << "Box=" << endl << Box << endl;

			for (int i = 0; i<Box.rows; i++)
				for (int j = 0; j < Box.cols; j++)
				{
					box_i = i*win_a;
					box_j = j*win_b;

					for (int k = box_i; k < (box_i + win_a); k++)
						for (int l = box_j; l < (box_j + win_b); l++)
							I.at<uchar>(k, l) = Box.at<uchar>(i, j);


				}
			//imshow("In_I", In_I);
			//imshow("Box", Box);
			cv::imshow("I_edited", I);

			
		}
		else cout << "Row or Col ==0" << endl;
		//Delay(10, "err");
	}
	else cout << "Number of Channels!=1" <<I.channels()<< endl;
	//Delay(10, "err");
	//Delay(1000, "view");
}

void Resize_pix( cv::Mat I, int win_a, int  win_b, int lower_threshold, int upper_threshold, const int D_WIDTH )
{
	if (I.channels() == 1)
	{
		if (I.rows > 0 && I.cols > 0)
		{
			int temp_pix = 0;
			int box_i = 0;
			int box_j = 0;
			int win_area = win_a*win_b;
			//static Mat In_I;
			Mat In_I;
			//Mat small_I;
			//cout << "depth of I=" << I.depth() << endl;


			//int Box_rows = (I.rows - (I.rows)%win_a)/win_a;
			//int Box_cols = (I.cols - (I.cols) % win_b)/win_b;


			int Box_rows = I.rows / win_a;

			//float B_r = (float)(I.rows /win_a);

			int Box_cols = I.cols / win_b;

			//cout << "B_r=" <<B_r<< endl;

			//cout << "Box_r=" << Box_rows << endl;
			//cout << "Box_cols=" << Box_cols << endl;

			int Box_area = Box_rows*Box_cols;
			Mat Box(Box_rows, Box_cols, CV_8UC1, Scalar(0));
			//Delay(5,"Fnished Initialization");
			/*TO RESIZE
			float scale_up = (float)D_WIDTH / (I.cols);

			unsigned int scale_height = 1;
			if (I.cols > D_WIDTH)
			{
			scale_height = cvRound((I.rows)*scale_up);
			resize(I, small_I, Size(D_WIDTH, scale_height));
			}*/

			integral(I, In_I, -1);



			//Box Pixel Values

			for (int i = 0; i<Box.rows; i++)
				for (int j = 0; j < Box.cols; j++)
				{
					box_i = i*win_a;
					box_j = j*win_b;
					temp_pix = (In_I.at<int>(box_i + win_a, box_j + win_b) + In_I.at<int>(box_i, box_j)
						- In_I.at<int>(box_i, box_j + win_b) - In_I.at<int>(box_i + win_a, box_j)) / (win_area);
					if ((temp_pix > lower_threshold) && (temp_pix < upper_threshold))
						Box.at<uchar>(i, j) = 0;
					else
						Box.at<uchar>(i, j) = (uchar)temp_pix;

				}
			//cout << "In_I=" << endl << In_I << endl;
			//cout << "Box=" << endl << Box << endl;

			for (int i = 0; i<Box.rows; i++)
				for (int j = 0; j < Box.cols; j++)
				{
					box_i = i*win_a;
					box_j = j*win_b;

					for (int k = box_i; k < (box_i + win_a); k++)
						for (int l = box_j; l < (box_j + win_b); l++)
							I.at<uchar>(k, l) = Box.at<uchar>(i, j);


				}
			//imshow("In_I", In_I);
			//imshow("Box", Box);
			imshow("I_edited", I);


		}
		else cout << "Row or Col ==0" << endl;
		//Delay(10, "err");
	}
	else cout << "Number of Channels!=1" << I.channels() << endl;
	//Delay(10, "err");
	//Delay(1000, "view");
}



//LIPFEATURES CLASS FUNCTIONS & static members
//------------------------------Static Vars--------------------------------------------------------
int LipFeatures::DEFAULT=12;//number of points t consider
int LipFeatures::LIP_POINT_DEFAULT=10;//initial value of x and y of the points
int LipFeatures::DEFAULT_NUM_CLASSES=5;//actual number of partitions
int LipFeatures::DEFAULT_NUM_ELEMENTS=10;// num of feature vector elements
int LipFeatures::DEFAULT_NUM_SAMPLES_PER_CLASS=100;//samples to be taken for each partition
int LipFeatures::DEFAULT_NUM_OP_ELEMENTS = 1;//num of elements in output layer for SVM it is (psuedo) 1 ;
int LipFeatures::NUM_OF_EL_AUXI_1 = 10;
unsigned int LipFeatures::sampling_factor=2;
//-----------------------------------------------------------------------------------------------------
string LipFeatures::img_loc="C:/Users/shubham/Desktop/test/vid";         // C:/Users/shubham/Desktop/test/vid
string LipFeatures::img_vect_loc="C:/Users/shubham/Desktop/test/vid";    // C:/Users/shubham/Desktop/test/vid
int LipFeatures::discard_count = 0;
int LipFeatures::sample_number = 0;
 string LipFeatures::my_image_window="Image";
 string LipFeatures::vector_window="Vector";
 vector<int > LipFeatures::discard_samples(0);
 string LipFeatures::my_temp_str = "shubham";
 int LipFeatures::halt_flag = 0;
 //---------------------------------------------static vars end--------------------------------------------



LipFeatures::LipFeatures() :message_mat(650, 1000, CV_8UC1, cv::Scalar(0)),original_mat(650, 1000, CV_8UC1, cv::Scalar(0)),
                         uniclass_vector_data(DEFAULT_NUM_SAMPLES_PER_CLASS,DEFAULT_NUM_ELEMENTS,CV_32FC1),
	                    training_mat(DEFAULT_NUM_SAMPLES_PER_CLASS, DEFAULT_NUM_ELEMENTS, CV_32FC1),
	                    classes(DEFAULT_NUM_CLASSES, DEFAULT_NUM_OP_ELEMENTS,CV_32SC1),
	                    labels(DEFAULT_NUM_SAMPLES_PER_CLASS*DEFAULT_NUM_CLASSES, DEFAULT_NUM_OP_ELEMENTS, CV_32SC1),
	                    training_mat_auxi_1(DEFAULT_NUM_SAMPLES_PER_CLASS,NUM_OF_EL_AUXI_1, CV_32FC1),
	current_fv_mat(1, DEFAULT_NUM_ELEMENTS, CV_32FC1), current_fv_mat_auxi_1(1, NUM_OF_EL_AUXI_1, CV_32FC1)
{
	alpha_vh = 2.9;//varies with person.
	first_call_setNumOfPoints = false;
	setElementSize(DEFAULT_NUM_SAMPLES_PER_CLASS*sampling_factor,DEFAULT_NUM_ELEMENTS,DEFAULT_NUM_ELEMENTS);
	setNumOfPoints(DEFAULT);
	cv::putText(original_mat, "Feature Vectors", cv::Point(20,20), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255), 1);
	temp_message = "NULL";
	ss << temp_message;
	
	
	//training data params
	num_samples_per_class= DEFAULT_NUM_SAMPLES_PER_CLASS;
	num_classes= DEFAULT_NUM_CLASSES;
	num_elements= DEFAULT_NUM_ELEMENTS;
	num_op_elements= DEFAULT_NUM_OP_ELEMENTS;
	discard_count = 0;
	string tb_window = "FEAT";
	namedWindow(tb_window,WINDOW_FREERATIO);
	cv::Mat tb_win_img(200, 500, CV_8UC1, Scalar(255));
	//cv::imshow(tb_window, tb_win_img);
	createTrackbar("Discard",tb_window,&discard_count,1,noteDiscard);
	createTrackbar("Pic_Num", tb_window, &sample_number, DEFAULT_NUM_SAMPLES_PER_CLASS*2, showSamples);
	createTrackbar("Halt", tb_window, &halt_flag,1,haltFunction);
	cv::imshow(tb_window, tb_win_img);
	vector<double> temp_ratio_features_hidden(DEFAULT_NUM_ELEMENTS, 0);
	vector<double> temp_ratio_features_auxi_1(NUM_OF_EL_AUXI_1, 0);
	ratio_features_hidden.resize(current_BS, temp_ratio_features_hidden);
	ratio_features_auxi_1.resize(current_BS, temp_ratio_features_auxi_1);
	waitKey(5);
	

}



void LipFeatures::setElementSize(int buffer_size , int ratio_vector_size , int diff_vector_size )
{
	setRatioFeaturesSize(buffer_size, ratio_vector_size);
	setDiffFeatureSize(buffer_size, diff_vector_size);
	current_BS = buffer_size;
	current_DiffS = diff_vector_size;
	current_RatioS = ratio_vector_size;
	
	
	
}

void LipFeatures::updatePoint(int point_num, int x_val, int y_val)
{
	points_array[point_num].setLipPoints(x_val, y_val);
}

void LipFeatures::setNumOfPoints(int num)
{
	points_array.resize(num, LipPoints(LIP_POINT_DEFAULT, LIP_POINT_DEFAULT));
}

void LipFeatures::setRatioFeaturesSize(int sample_size,int vector_size)
{
	vector<double> temp_ratio(vector_size, 1.0);
	ratio_features.resize(sample_size);
	for (int i = 0; i < sample_size; i++)
		ratio_features[i] = temp_ratio;
	current_RatioS = vector_size;
	num_elements = vector_size;
	
}

void LipFeatures::setDiffFeatureSize(int sample_size,int vector_size)
{
	vector<int> temp_diff(vector_size, 1);
	diff_features.resize(sample_size);
	for (int i = 0; i < sample_size; i++)
	diff_features[i]=temp_diff;
	current_DiffS = vector_size;

}

 
 void LipFeatures::calcFeatureVectorsDiff()
{
	diff_features.pop_back();
	vector<int > temp_vector;

	temp_vector.push_back(diffX(2,5));
	temp_vector.push_back(diffX(1,6));
	temp_vector.push_back(diffY(4,0));
	temp_vector.push_back(diffY(3,1));
	if (temp_vector.size() != current_RatioS)
		temp_vector.resize(current_RatioS, 0);
	diff_features.push_front(temp_vector);

}

 //ADD FNS TO CALCULATE ELS OF AUXILLIARY DEQUES
 void LipFeatures::calcFeatureVectorsRatio()
 {
	 ratio_features.pop_back();
	 vector<double> temp;
	 temp.push_back((mod(1,11 ) )  );//v0
	 temp.push_back((mod(2, 10)));//v1
	 temp.push_back((mod(3, 9)));//v2
	 temp.push_back((mod(4, 8)));//v3
	 temp.push_back((mod(5, 7)));//v4
	 temp.push_back((mod(8, 10)));//v5
	 temp.push_back((mod(7, 11)));//v6
	 temp.push_back((mod(0, 6)));//v7
	 temp.push_back((mod(1, 5)));//v8
	 temp.push_back((mod(2, 4)));//v9

	
	 if (temp.size() != current_RatioS)
		 temp.resize(current_RatioS, 0);

	 ratio_features.push_front(temp);
	 
 }



 double LipFeatures::mod(int i,int j)
 {
	// int delta_x= abs(points_array[i].x - points_array[j].x);

	//int delta_y=	abs( points_array[i].y - points_array[j].y);
	 double delta_x = (double)(points_array[i].x - points_array[j].x);

	 double delta_y=	(double)( points_array[i].y - points_array[j].y);
	//return (double(delta_x + delta_y - ((double)delta_x)*delta_y / (delta_x + delta_y)));
	 return (std::sqrt(delta_x*delta_x + delta_y*delta_y));

 }



 int LipFeatures::diffX(int i, int j)
 {
	 return(points_array[i].x - points_array[j].x);
 }




 int LipFeatures::diffY(int i, int j)
 {
	 return(points_array[i].y - points_array[j].y);
 }


 //Don't use::to be modified
 void LipFeatures::showRatioVector(int i)
 {
	
	 if (i < current_BS)
	 {
		 
		 ss.clear();
		 ss<< i <<":..";
		
		for (int j = 0; j < current_RatioS; j++)
		{
		
			try { ss << (ratio_features[i].at(j)) << ".."; }

			catch (exception& e)
			{
				cout << "Err" << endl;
			    cout << e.what() << endl;
				Delay(10, "LOOK");

			}


		}
		
		//TRAINING
		ss << "AUXI:";
		if(ratio_features_auxi_1.size()>i)
		for (int j = 0; j < NUM_OF_EL_AUXI_1; j++)
		{

			try { ss << (ratio_features_auxi_1[i].at(j)) << ".."; }

			catch (exception& e)
			{
				cout << "Err in show ratio vector auxi show" << endl;
				cout << e.what() << endl;
				Delay(10, "LOOK");

			}


		}


		 ss >> temp_message;
		// cout <<"temp_message" <<temp_message<< endl;
		if((i+1)*50 < message_mat.cols)
   cv::putText(message_mat, temp_message, cv::Point(0, (i + 1) * 50), cv::FONT_HERSHEY_PLAIN, 0.8, Scalar(255), 1);
		
		 
	 }
	 else cout << "Index Out Of range" << endl;
	// cout << endl;
 }

 void LipFeatures::clearMessageMat()
 {
	 //message_mat = original_mat.clone();
	 original_mat.copyTo(message_mat);
 
 }

 //Don't use::to be modified
 void LipFeatures::resetEverything(int num_of_points, int buffer_size, int ratio_vector_size, int diff_vector_size)
 {
	 cout << "x_resetEverything" << endl;
 
 }



 void LipFeatures::resetEverything()
 {
	 
	 setNumOfPoints(DEFAULT);
	 discard_samples.clear();
	 first_call_setNumOfPoints = false;
	 setElementSize(DEFAULT_NUM_SAMPLES_PER_CLASS *sampling_factor, DEFAULT_NUM_ELEMENTS, DEFAULT_NUM_ELEMENTS);
	 num_samples_per_class = DEFAULT_NUM_SAMPLES_PER_CLASS;
	 num_classes = DEFAULT_NUM_CLASSES;
	 num_elements = DEFAULT_NUM_ELEMENTS;
	 num_op_elements = DEFAULT_NUM_OP_ELEMENTS;
	 current_RatioS = num_elements;
	 discard_count = 0;
	
	 ratio_features_hidden.clear();
	 ratio_features_auxi_1.clear();
	 vector<double> temp_ratio_features_hidden(DEFAULT_NUM_ELEMENTS, 0);
	 vector<double> temp_ratio_features_auxi_1(NUM_OF_EL_AUXI_1, 0);
	 ratio_features_hidden.resize(current_BS, temp_ratio_features_hidden);//note:currentBS is set by setElementSize();
	 ratio_features_auxi_1.resize(current_BS, temp_ratio_features_auxi_1);


 }


 //it has one overload below : USE THAT OVERLOAD
 void LipFeatures::addVectorToTD()
 {
	 static int avttd_count = 0;

	 for (int j = 0; j < num_elements; j++)
	 {
		 LipFeatures::training_mat.at<float>(avttd_count, j) = (float)(ratio_features_hidden[avttd_count].at(j));
		 //cout << training_mat << endl << "-------------------------" << endl;
		// Delay(15, "see");
	 }
	 avttd_count += 1;

	 cout << "avttd_count=" << avttd_count << endl;

	 
 }
 


 void LipFeatures::showParams()
 {
	 cout << "-------vars------" << endl;
	  cout<<"num_samples_per_class= "<<num_samples_per_class<<endl;
	  cout << "num_classes= " << num_classes << endl;
	  cout << "num_elements= "<<num_elements<<endl;
	  cout << "num_op_elements= "<<num_op_elements<<endl;//nodes in output layer (for SVM =1)
	  cout << "current_BS= "<< current_BS<<endl;
	  cout << "current_DiffS= " << current_DiffS << endl;
	  cout << "current_RatioS= "<<current_RatioS<<endl;
	  cout << "-----------------vectors and Mats----------------------" << endl;
	  cout << "ratio_features.size()= " <<ratio_features.size() << endl;
	  cout << "ratio_features[0].size()= " << ratio_features[0].size() << endl;
	  cout << "diff_features.size()= " << diff_features.size() << endl;
	  cout << "diff_features[0].size()= " << diff_features[0].size() << endl;
	  cout << "uniclass_vector_data.rows= " << uniclass_vector_data.rows << endl;
	  cout << "uniclass_vector_data.cols= " << uniclass_vector_data.cols << endl;
	  cout << "training_mat.rows= " << training_mat.rows << endl;
	  cout << "training_mat.cols= " << training_mat.cols << endl;
	  cout << "training_mat_auxi_1.rows= " << training_mat_auxi_1.rows << endl;
	  cout << "training_mat_auxi_1.cols= " << training_mat_auxi_1.cols << endl;
	  cout << "classes.rows=" << classes.rows << endl;
	  cout << "classes.cols=" << classes.cols << endl;
	  cout << "labels.rows=" << labels.rows << endl;
	  cout << "labels.cols=" << labels.cols << endl;
	  cout << " current_fv_mat.rows=" << current_fv_mat.rows << endl;
	  cout << " current_fv_mat.cols=" << current_fv_mat.cols << endl;
	  cout <<" current_fv_mat_auxi_1.rows="<< current_fv_mat_auxi_1.rows << endl;
	  cout << " current_fv_mat_auxi_1.cols=" << current_fv_mat_auxi_1.cols << endl;
	  cout << "S: img_loc= " << img_loc << endl;
	  cout << "S: img_vect_loc" << img_vect_loc << endl;
	  cout << "S: discard_samples.size()= " << discard_samples.size() << endl;
	  cout << "S: discard_count-1= " << (discard_count - 1) << endl;
	  cout << "S: sample_number-1 =" << (sample_number - 1) << endl;

 }

 //don't use
 void LipFeatures::noteDiscard(int, void*)
 {
	 //discard_count += 1;
	 if (halt_flag == 0)
	 {
		 if (sample_number > 0)
			 discard_samples.push_back(sample_number - 1);
		 int flag = 0;
		 if (discard_samples.size() > 0)
			 for (int i = 0; i < (discard_samples.size() - 1); i++)
			 {
				 if (discard_samples[i] == (sample_number - 1))
				 {
					 discard_samples.pop_back();
					 //discard_count -= 1;
					 break;
				 }
			 }
		 else cout << "Discard Samples is still empty" << endl;


		 cout << "discard_count-1= " << (discard_count - 1) << endl;
		 cout << "discard_samples.size()= " << discard_samples.size() << endl;
	 }
	 else cout << "IMP:: Set Halt Flag to Zero after bringing Discard Count\n and SampleCount to ZERO" << endl;
 }


 //don't use
 void LipFeatures::showSamples(int, void*)
 {
	 //sss.clear();
	 if (halt_flag == 0 && sample_number>0 )

	 {
		 stringstream sss;
		 my_temp_str.clear();
		 cout << my_temp_str;
		 sss << img_loc;
		 sss << "/img_num_";
		 sss << (sample_number - 1) << ".jpg";//potential off by one error
		 sss >> my_temp_str;
		 cout << "temp= " << my_temp_str << endl;
		 //Delay(5, "about to call imread()");
		 Mat temp_mat = imread(my_temp_str);
		 //Delay(5, "afterimread");

		 imshow(my_image_window, temp_mat);
		 waitKey(30);

		// sss.clear();
		// my_temp_str.clear();
		 //string temp = "NULL";
		// sss << img_vect_loc;
		// sss << "/img_vect_num_";
		// sss << (sample_number - 1) << ".jpg";//potential off by one error
		// sss >> my_temp_str;
		// temp_mat = imread(my_temp_str);
		// imshow(vector_window, temp_mat);
		 waitKey(30);
	//	 cout << my_temp_str << endl;

		 cout << "sample_number-1 =" << (sample_number - 1) << endl;
		 sss.clear();
	 }
	 else cout << "IMPORTANT:: Set Halt Flag to Zero after bringing Discard Count\n and SampleCount to ZERO" << endl;

 }


 
 void LipFeatures::addVectorToTD(int trn_mat_row, int sample_row)
 {
	 for (int p= 0; p < num_elements; p++)
	 {
		 LipFeatures::training_mat.at<float>(trn_mat_row,p) = (float)(ratio_features_hidden[sample_row].at(p));
		 //cout << training_mat << endl << "-------------------------" << endl;
		 // Delay(15, "see");
	 }

	 for (int p = 0; p < NUM_OF_EL_AUXI_1; p++)
	 {
		 LipFeatures::training_mat_auxi_1.at<float>(trn_mat_row, p) = (float)(ratio_features_auxi_1[sample_row].at(p));
		 //cout << training_mat << endl << "-------------------------" << endl;
		 // Delay(15, "see");
	 }


 }

 //don't use
 void LipFeatures::createTrainingDataForOneClass()
 {
	 
	 
	 if (discard_samples.size() > 0)
	 {
		 
		 bool no_more_discarded_vectors = false;
		 //Arrange
		 int arrg_temp = 0;
		 for (int i = 0; i <(discard_samples.size() - 1); i++)
		 for (int k = 0; k <( discard_samples.size()-1); k++)
		 {
			 if (discard_samples[k] > discard_samples[k+ 1])
			 {
				 arrg_temp = discard_samples[k];
				 discard_samples[k] = discard_samples[k + 1];
				 discard_samples[k + 1] = arrg_temp;
			 }
		 }
		 cout << "You discarded these samples." << endl;
		 for (int k = 0; k < discard_samples.size(); k++)
			 cout << "DS[" << k << "]= " << discard_samples[k] << endl;
		 //end arranging

		 int note_id = 0;
		 //number of elements in discard samples must be at least 1;

		 int DS_index = discard_samples[note_id];
		 int j = 0;
		 for (int i = 0; i < num_samples_per_class; i++)
		 {
			 while (j == DS_index && (!no_more_discarded_vectors))
			 {
				 cout << "skipped js=" << j << endl;
				 note_id += 1;
				 if (note_id == discard_samples.size())
				 {
					 j += 1;
					 no_more_discarded_vectors = true;
					 cout << "end of discard_samples reached." << endl;
					 break;
				 }

				 DS_index = discard_samples[note_id];
				 j += 1;
				 cout << "note_id= " << note_id << endl;
				 cout << "j=" << j << endl;
				// Delay(5, "see params above");
			 }

			 if (j < ratio_features.size())
				 addVectorToTD(i, j);
			 else cout << "In createTrainingDataForOneClass j exceeds the max index of ratio_features" << endl;
			 j += 1;
		 }
	 }
	 else
	 {
		 cout << "No sample to discard" << endl;
		 for (int i = 0; i < num_samples_per_class; i++)

		 {
			 if (i < ratio_features.size())
				 addVectorToTD(i, i);
			 else cout << "In createTrainingDataForOneClass j exceeds the max index of ratio_features" << endl;
		 }
	 }


	 cout << "Enter Filename to store this mat(eg. sample_0.exr)";
	 string ctdfoc_str;
	 cin >> ctdfoc_str;
	// ctdfoc_str = ctdfoc_str + ".jpg";
	 imwrite(ctdfoc_str, training_mat);
	
	 //AUXI_1 MAT
	 cout << "Enter Filename to store auxi_1 mat(eg. auxi_0.exr)";
	 cin >> ctdfoc_str;
	 // ctdfoc_str = ctdfoc_str + ".jpg";
	 imwrite(ctdfoc_str, training_mat);

 }



 void LipFeatures::updateIMGLOC(string extension_str)
 {
	 img_loc = extension_str;
		 img_vect_loc = extension_str;
		 cout << "Updated class Location Values:\nimg_loc: " << img_loc << endl;
		 cout << "img_vect_loc: " << img_vect_loc << endl;
 
 }


 void LipFeatures::haltFunction(int, void*)
 {}


 void LipFeatures::getCurrentVectorMat(cv::Mat& dest_mat, int mat_id)
 {//mat_id: 0:absolute
	 //1: auxi_1for (int p= 0; p < num_elements; p++)
	 switch (mat_id)
	 {case 0 :
		 for (int p = 0; p < DEFAULT_NUM_ELEMENTS; p++)
		 {
			 LipFeatures::current_fv_mat.at<float>(0, p) = (float)(ratio_features[0].at(p));
			 //cout << training_mat << endl << "-------------------------" << endl;
			 // Delay(15, "see");
		 }
		 current_fv_mat.copyTo(dest_mat);
		 break;
	 case 1:
		 for (int p = 0; p < NUM_OF_EL_AUXI_1; p++)
		 {
			 LipFeatures::current_fv_mat_auxi_1.at<float>(0, p) = (float)(ratio_features_auxi_1[0].at(p));
			 //cout << training_mat << endl << "-------------------------" << endl;
			 // Delay(15, "see");
			 
		 }
		// cout << "auxi_1" << endl;
		// cout<<current_fv_mat_auxi_1<<endl;
		 current_fv_mat_auxi_1.copyTo(dest_mat);
		 break;

	 default:
		 for (int p = 0; p < NUM_OF_EL_AUXI_1; p++)
		 {
			 LipFeatures::current_fv_mat_auxi_1.at<float>(0, p) = (float)(ratio_features_auxi_1[0].at(p));
			 //cout << training_mat << endl << "-------------------------" << endl;
			 // Delay(15, "see");
		 }

		 current_fv_mat_auxi_1.copyTo(dest_mat);
		 break;

	 }
 
 
 }



 void LipFeatures::calcVectorsAuxi_1()
 {
	 ratio_features_auxi_1.pop_back();
	 vector<double> temp_auxi_1;
	 temp_auxi_1.push_back(alpha_vh * 100 * (mod(1, 11)) / (mod(0, 6)));//v0
	 temp_auxi_1.push_back(alpha_vh * 100 * (mod(2, 10)) / (mod(0, 6)));//v1
	 temp_auxi_1.push_back(alpha_vh * 100 * (mod(3, 9)) / (mod(0, 6)));//v2
	 temp_auxi_1.push_back(alpha_vh * 100 * (mod(4, 8)) / (mod(0, 6)));//v3
	 temp_auxi_1.push_back(alpha_vh * 100 * (mod(5, 7)) / (mod(0, 6)));//v4


	 temp_auxi_1.push_back(100 * (mod(8, 10)) / (mod(3, 9)*alpha_vh));//v5
	 temp_auxi_1.push_back(100 * (mod(7, 11)) / (mod(3, 9)*alpha_vh));//v6
	 temp_auxi_1.push_back(100 * (mod(0, 6)) / (mod(3, 9)*alpha_vh));//v7
	 temp_auxi_1.push_back(100 * (mod(1, 5)) / (mod(3, 9)*alpha_vh));//v8
	 temp_auxi_1.push_back(100 * (mod(2, 4)) / (mod(3, 9)*alpha_vh));//v9

	 if (temp_auxi_1.size() != NUM_OF_EL_AUXI_1)
		 temp_auxi_1.resize(NUM_OF_EL_AUXI_1, 0);

	 ratio_features_auxi_1.push_front(temp_auxi_1);
 
 
 }


 void LipFeatures::calcAllVectors()
 {
	 LipFeatures::calcFeatureVectorsRatio();
	 LipFeatures::calcVectorsAuxi_1();
 
 }

 void LipFeatures::showAuxiVector_1(int last_n_vectors)
 {

	 //TRAINING
	 original_mat.copyTo(recent_AuxiMat_1);
	 ss.clear();
	 ss << "AUXI:";
	 if (ratio_features_auxi_1.size()>last_n_vectors)
		 for (int r = 0; r < last_n_vectors; r++)
		 {
			 for (int j = 0; j < NUM_OF_EL_AUXI_1; j++)
			 {

				 try { ss << (ratio_features_auxi_1[r].at(j)) << ".."; }

				 catch (exception& e)
				 {
					 cout << "Err in show ratio vector auxi show" << endl;
					 cout << e.what() << endl;
					 Delay(10, "LOOK");

				 }


			 }
			 ss >> temp_message;
			 if ((r + 1) * 50 < message_mat.cols)
				 cv::putText(recent_AuxiMat_1, temp_message, cv::Point(0, (r + 1) * 50), cv::FONT_HERSHEY_PLAIN, 0.8, Scalar(255), 1);


		 }

	 cv::imshow("Recent Auxi-Vectors_1", recent_AuxiMat_1);
	
	 // cout <<"temp_message" <<temp_message<< endl;
	 
 }




 //----------------CLASS L2-----------------------------
