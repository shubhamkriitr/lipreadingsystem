// 2_dlib tEST V2.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"


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
//#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_transforms.h>
//#include <dlib\all\source.cpp>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "My_fns.h"
#include "helper.h"
#include "VisemeAnalyzer.h"
//------------------dl---------------------------------
#include <dlib/svm_threaded.h>

#include <iostream>
#include <vector>

#include <dlib/rand.h>
using namespace dlib;
using namespace std;
const long NUM_EL = 10;
typedef matrix<double, NUM_EL, 1> sample_type;

//--------------------------dl-------------------
int RESIZE_ROWS = 321;
int RESIZE_COLS = RESIZE_ROWS*4/3;
int START_K = 48;
int STOP_K = 68;
int NUM_LM_MAX = 68;
int sampling_over = 0;
int discarding_over = 0;
int test_viseme_count = 0;

//---------temp stream----
ofstream outl("opp.txt");
template <typename T>
void showAnyVector(const std::vector<T> & vctr)
{
	cout << endl << "------SV------" << endl;
	for (int h = 0; h < vctr.size(); ++h)
		cout << h << ":" << vctr[h] << "; ";
	cout << endl << "----SV_over----" << endl;
}

void mergeMats(int num_of_mats, string file_loc_plus_init_name, std::vector<float> label_id ,
	Mat&merged_output, Mat&corresponding_label_mat)
{
	int NUM_VIS = num_of_mats;
	int viseme_rows = 0;
	int viseme_cols = 0;
	stringstream sspp;
	string img_loc_vis = file_loc_plus_init_name;  // "C:/Users/shubham/Desktop/test/vis/";
	string load_vis;
	std::vector <cv::Mat > VISEMES(NUM_VIS);
	//std::vector <int > label_id(NUM_VIS);
	for (int i = 0; i < NUM_VIS; i++)
	{
		sspp.clear();
		sspp << img_loc_vis;
		sspp << i;
		sspp << ".exr";
		sspp >> load_vis;
		VISEMES[i] = imread(load_vis, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
		//VISEMES.push_back(temp_mat_vis);
		viseme_rows += VISEMES[i].rows;

		viseme_cols = VISEMES[i].cols;
		

		std::cout << "VISEME[" << i << "]=" << endl << VISEMES[i] << endl;
	}
	Mat compiled_mat(viseme_rows, viseme_cols, CV_32FC1);
	Mat compiled_label_mat(viseme_rows, 1, CV_32FC1);
	int curr_row = 0;
	for (int i = 0; i < NUM_VIS; i++)
	{
		for (int j = 0; j < VISEMES[i].rows; j++)
		{

			for (int k = 0; k < viseme_cols; k++)
			{
				compiled_mat.at<float>(curr_row, k) = VISEMES[i].at<float>(j, k);
			}

			compiled_label_mat.at<float>(curr_row, 0) = label_id[i];
			curr_row += 1;

		}
	}
	std::cout << "Compiled Label Mat:" << endl << compiled_label_mat << endl;

	std::cout << "Compiled Mat" << endl << compiled_mat << endl;

	compiled_mat.copyTo(merged_output);
	compiled_label_mat.copyTo(corresponding_label_mat);


	//Mat lab;
	//Mat vis;
	//string needed_loc = "C:/Users/shubham/Desktop/test/vis/";
	//std::vector<int > labels_vis(1);
	//labels_vis[0] = 5;
	////labels_vis[1] = 11;

	//mergeMats(labels_vis.size(), needed_loc, labels_vis, vis, lab);

	//std::cout << "--------------------------------------------------------" << endl;

	//std::cout << "VIS\n" << vis << endl;
	//cout << "LAB\n" << lab << endl;

	//-----------------------------------------------------

}

// ----------------------------------------------------------------------------------------
void tb_ResizeRows(int, void*)
{
	if (RESIZE_ROWS < 15)
		RESIZE_ROWS = 15;

	RESIZE_COLS = RESIZE_ROWS * 4 / 3;



}

void tb_ResizeCols(int, void*)
{
	if (RESIZE_COLS < 20)
		RESIZE_COLS = 20;
	RESIZE_ROWS = RESIZE_COLS *3/4;
}

void tb_SetStartK(int, void*)
{}


void tb_SetStopK(int, void*)
{
	if (STOP_K < 1)
		STOP_K = 1;
}

void tb_SamplingStatus(int, void*)
{
}
void tb_DiscardingStatus(int, void*)
{
}



int main(int argc, char** argv)
{


#if 1
	ofstream note_visemes("C:/Users/shubham/Desktop/test/resources/output/note_visemes.txt");
	VisemeAnalyzer VA;
	//VA.mergeTextChunks(11);
	VA.loadDictionary("C:/Users/shubham/Desktop/test/resources/LOADFILE.txt");






	//------------------------------------------------------
	LipFeatures feat;
	Ptr<SVM> svm = Algorithm::load<SVM>("C:/Users/shubham/Desktop/test/xml/FullTrainedSVM_LINEAR_100i.xml");
	char next_viseme_choice = 'y';
	int so_flag = 1;
	int do_flag = 1;
	bool  flag_gil = false;//flag_to_get_out_of_inner_loop
	string default_vid_folder = "C:/Users/shubham/Desktop/test/vid";
	const string CONST_DEFAULT_VID_FOLDER = "C:/Users/shubham/Desktop/test/vid";
	int folder_updater = 0;
	int frame_count = 0;
	int detected_frame_count = 0;
	string original_image = "/img_num_";
	string vector_mat = "/img_vect_num_";
	string writefile;
	stringstream sw;

	stringstream ss;
	string frame_message = "0";
	unsigned long frames_elapsed = 0;
	string input_file_name = "c";
	string default_folder = "C:/Users/shubham/Desktop/test/";

	cout << "type filename (e.g. vid1.mp4) or c , for using web cam" << endl;
	cv::VideoCapture vid;
	cin.clear();
	//cin.ignore(500, '\n');
	cin >> input_file_name;
	if (input_file_name == "c")
	{
		vid.open(0);

	}
	else
	{
		ss << default_folder;
		ss << input_file_name;
		ss >> input_file_name;
		vid.open(input_file_name);
		cout << "  Location:" << input_file_name << endl;
	}




	cv::Mat I;
	cv::Mat dummy_mat(1, 500, CV_8UC1, cv::Scalar(255));
	long t = 0;
	long num_landmarks = 1;
	cv::namedWindow("Parameters", WINDOW_FREERATIO);
	cv::createTrackbar("Rows", "Parameters", &RESIZE_ROWS, 1080, tb_ResizeRows);
	cv::createTrackbar("Cols", "Parameters", &RESIZE_COLS, 1080, tb_ResizeCols);
	cv::createTrackbar("START_K", "Parameters", &START_K, NUM_LM_MAX, tb_SetStartK);
	cv::createTrackbar("STOP_K", "Parameters", &STOP_K, NUM_LM_MAX, tb_SetStopK);
	//cv::createTrackbar("Sampling_over", "Parameters", &sampling_over, 1, tb_SamplingStatus);
	//cv::createTrackbar("Discarding_over", "Parameters", &discarding_over, 1, tb_DiscardingStatus);
	cv::imshow("Parameters", dummy_mat);

	cv::Mat fv_at_present;

	try
	{
		string fn = "C:/Users/shubham/Desktop/test/resources/shape_predictor_68_face_landmarks.dat";

		// We need a face detector.  We will use this to get bounding boxes for
		// each face in an image.
		frontal_face_detector detector = get_frontal_face_detector();
		shape_predictor sp;
		//deserialize(argv[1]) >> sp;
		deserialize(fn) >> sp;
		//VIDEO EXTRACTION
		while (next_viseme_choice != 'n')

		{

			//---------update main params-----------
			ss.clear();
			ss << CONST_DEFAULT_VID_FOLDER;
			ss << folder_updater;
			ss >> default_vid_folder;

			feat.updateIMGLOC(default_vid_folder);


			feat.resetEverything();

			feat.showParams();
			Delay(1, "wait");
			while (waitKey(30) != 's')
			{
				vid >> I;
				flip(I, I, 1);
				cv::resize(I, I, cv::Size(RESIZE_COLS, RESIZE_ROWS));
				imshow("I", I);

			}





			int key;
			while (key = cv::waitKey(30))
			{
				if (key == 'p') {
					while (waitKey(0) != 'p') {}
				}
				frames_elapsed += 1;
				frame_count += 1;

				array2d<rgb_pixel> img;

				vid >> I;


				cv::resize(I, I, cv::Size(RESIZE_COLS, RESIZE_ROWS));

				cv::flip(I, I, 1);
				assign_image(img, cv_image<bgr_pixel>(I));

				// Now tell the face detector to give us a list of bounding boxes
				// around all the faces in the image.
				std::vector<dlib::rectangle> dets = detector(img);
				//cout << "Number of faces detected: " << dets.size() << endl;
				// Now we will go ask the shape_predictor to tell us the pose of
				// each face we detected.
				std::vector<full_object_detection> shapes;
				std::vector<full_object_detection> shapes_p;
				full_object_detection shape_temp;


				for (unsigned long j = 0; j < (dets.size()); ++j)
				{
					full_object_detection shape = sp(img, dets[j]);

					num_landmarks = shape.num_parts();


					for (int k = START_K; k < STOP_K; k++)
					{
						cv::circle(I, cv::Point(shape.part(k).x(), shape.part(k).y()), 2, cv::Scalar(255, 255, 0), -1, 8);
					}

					cv::line(I, cv::Point(shape.part(51).x(), shape.part(51).y()),
						cv::Point(shape.part(57).x(), shape.part(57).y()), cv::Scalar(255, 0, 0), 1);

					cv::line(I, cv::Point(shape.part(54).x(), shape.part(54).y()),
						cv::Point(shape.part(48).x(), shape.part(48).y()), cv::Scalar(0, 255, 0), 1);

					cv::line(I, cv::Point(shape.part(50).x(), shape.part(50).y()),
						cv::Point(shape.part(58).x(), shape.part(58).y()), cv::Scalar(0, 255, 0), 1);

					cv::line(I, cv::Point(shape.part(53).x(), shape.part(53).y()),
						cv::Point(shape.part(49).x(), shape.part(49).y()), cv::Scalar(0, 255, 0), 1);
					if (j == 0)
					{
						feat.updatePoint(0, shape.part(48).x(), shape.part(48).y());
						feat.updatePoint(1, shape.part(49).x(), shape.part(49).y());
						feat.updatePoint(2, shape.part(50).x(), shape.part(50).y());
						feat.updatePoint(3, shape.part(51).x(), shape.part(51).y());
						feat.updatePoint(4, shape.part(52).x(), shape.part(52).y());
						feat.updatePoint(5, shape.part(53).x(), shape.part(53).y());
						feat.updatePoint(6, shape.part(54).x(), shape.part(54).y());

						feat.updatePoint(7, shape.part(55).x(), shape.part(55).y());
						feat.updatePoint(8, shape.part(56).x(), shape.part(56).y());
						feat.updatePoint(9, shape.part(57).x(), shape.part(57).y());
						feat.updatePoint(10, shape.part(58).x(), shape.part(58).y());
						feat.updatePoint(11, shape.part(59).x(), shape.part(59).y());

						feat.calcAllVectors();
						feat.getCurrentVectorMat(fv_at_present, 1);

						//cout << "FV_MAT"<<fv_at_present<<endl;

						float response = (svm->predict(fv_at_present));
						test_viseme_count += 1;//remove later
						note_visemes << test_viseme_count << ": " << response << "  " << (int)response << endl;//remove later
						VA.addElementToLatestBuffer((int)response);
						VA.noteInRecordPadForPrediction();
						if (VA.flag == 1)
							//VA.showRecord();
						{

							VA.showRecordInConsole();
							if (VA.predict_flag == 1)
							{
								if (VA.crudeAnalysis_OPR())
								{
									/*cout << "Press g" << endl;
									while (waitKey(0) != 'g')
									{
									}*/
									Delay(2, "_");
								}
								VA.predict_flag = 0;
							}
							VA.clearRecord();

							VA.flag = 0;
						}



						//------show detected word----------------
						ss.clear();
						ss << (response);
						switch ((int)response)
						{
						case 0:
							ss << "_P";
							break;
						case 1:
							ss << "_k";
							break;
						case 2:
							ss << "_f";
							break;

						case 3:
							ss << "_Ae";
							break;

						case 4:
							ss << "_AA";
							break;

						case 5:
							ss << "_EE";
							break;

						case 6:
							ss << "_(aww)";
							break;

						case 7:
							ss << "_OO";
							break;

						default:
							ss << " oops";


						}




						ss >> frame_message;
						//cv::putText(I, frame_message, cv::Point(I.rows*0.3, I.cols*0.6), cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0,255,255),2);
						cv::putText(I, frame_message, cv::Point(I.rows*0.2, I.cols*0.7), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 2);




						/*ss.clear();
						ss << (response_dlib);
						switch ((int)response_dlib)
						{
						case 0:
						ss << "P";
						break;
						case 1:
						ss << "k";
						break;
						case 2:
						ss << "f";
						break;

						case 3:
						ss << "Ae";
						break;

						case 4:
						ss << "AA";
						break;

						case 5:
						ss << "EE";
						break;

						case 6:
						ss << "(aww)";
						break;

						case 7:
						ss << "OO";
						break;

						default:
						ss << " oops";


						}

						ss >> frame_message;
						//						cv::putText(I, frame_message, cv::Point(I.rows*0.6, I.cols*0.6), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 0, 255), 2);
						cv::putText(I, frame_message, cv::Point(I.rows*0.3, I.cols*0.6), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 255), 2);
						*/
						detected_frame_count += 1;

					}


				}


				//--------------------------------------------------------------
				ss.clear();
				ss << (detected_frame_count);
				ss >> frame_message;

				cv::putText(I, frame_message, cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0), 1);
				//---------------------------------------------------------------
				imshow("I", I);
				waitKey(10);

				if (flag_gil)
				{
					flag_gil = false;
					break;
				}


			}//inner while




			cout << "For collecting data for next WORD SET, type 'y' or 'n' to end. " << endl;
			cin >> next_viseme_choice;

			//---------update main params-----------
			//feat.sample_number = 0;
			//folder_updater += 1;
			//	detected_frame_count = 0;
			//frames_elapsed = 0;
			//	frame_count = 0;







		}//outer while








	}
	catch (exception& e)
	{
		cout << "\nexception thrown!" << endl;
		cout << e.what() << endl;
	}









	system("pause");
# endif
#if 0
	VisemeAnalyzer VT;
	/*std::vector<string> wl(1, "SHUBHAM");
	wl.push_back("KUMAR");
	std::vector<string> rkeys(5, "012222234445");
	rkeys.push_back("56788999");
	rkeys.push_back("89111011");
	VT.createTextChunk(wl, rkeys, false);
	//Delay(15, "see the file");
	std::vector<string> wl2(1, "POOL");
	wl2.push_back("GOOD");
	wl2.push_back("SAVE");
	wl2.push_back("WATER");
	std::vector<string> rkeys2(5, "01444522");
	rkeys2.push_back("11544677755");
	rkeys2.push_back("8911");
	VT.createTextChunk(wl2, rkeys2, true);
	//Delay(15, "see the file");
	VT.createTextChunk(wl, rkeys2, true);
	//Delay(15, "see the file");*/
	std::vector<string > vect1(0);
	std::vector<string > vect2(0);
	string tststr = "s";
	for (int k = 0; k < 6; ++k)
	{
		vect1.push_back(tststr);
		tststr = tststr + "s";
	}
	showAnyVector<string>(vect1);
	tststr = "ss";
	for (int k = 0; k < 3; ++k)
	{
		vect2.push_back(tststr);
		tststr = tststr + "s";
	}
	showAnyVector<string>(vect2);
	showAnyVector<string>(VT.mergeTwoStringVectors(vect1, vect2));

	showAnyVector<string>(VT.mergeTwoStringVectors(vect2, vect1));

	//VT.createDictionary("C:/Users/shubham/Desktop/test/resources/LOADFILE.txt");
	//VT.mergeTextChunks(2);
	VT.createBigChunk_2();

	system("pause");
# endif



# if 0
	//----------------------------------------------------------
	ofstream note_visemes("C:/Users/shubham/Desktop/test/resources/output/note_visemes.txt");
	VisemeAnalyzer VA;
	//VA.loadDictionary("C:/Users/shubham/Desktop/test/resources/lol.txt");
	/*
	int num_of_txt_to_be_merged = 0;
	cout << "Input Number of files to be merged." << endl;
	cin >> num_of_txt_to_be_merged;
	VA.mergeTextChunks(num_of_txt_to_be_merged);*/


	//------------------------------------------------------
	LipFeatures feat;
	Ptr<SVM> svm = Algorithm::load<SVM>("C:/Users/shubham/Desktop/test/xml/FullTrainedSVM_LINEAR_100i.xml");
	char next_viseme_choice = 'y';
	int so_flag = 1;
	int do_flag = 1;
	bool  flag_gil = false;//flag_to_get_out_of_inner_loop
	string default_vid_folder = "C:/Users/shubham/Desktop/test/vid";
	const string CONST_DEFAULT_VID_FOLDER = "C:/Users/shubham/Desktop/test/vid";
	int folder_updater = 0;
	int frame_count = 0;
	int detected_frame_count = 0;
	string original_image = "/img_num_";
	string vector_mat = "/img_vect_num_";
	string writefile;
	stringstream sw;

	stringstream ss;
	string frame_message = "0";
	unsigned long frames_elapsed = 0;
	string input_file_name = "c";
	string default_folder = "C:/Users/shubham/Desktop/test/";

	cout << "type filename (e.g. vid1.mp4) or c , for using web cam" << endl;
	cv::VideoCapture vid;
	cin.clear();
	//cin.ignore(500, '\n');
	cin >> input_file_name;
	if (input_file_name == "c")
	{
		vid.open(0);

	}
	else
	{
		ss << default_folder;
		ss << input_file_name;
		ss >> input_file_name;
		vid.open(input_file_name);
		cout << "  Location:" << input_file_name << endl;
	}




	cv::Mat I;
	cv::Mat dummy_mat(1, 500, CV_8UC1, cv::Scalar(255));
	long t = 0;
	long num_landmarks = 1;
	cv::namedWindow("Parameters", WINDOW_FREERATIO);
	cv::createTrackbar("Rows", "Parameters", &RESIZE_ROWS, 1080, tb_ResizeRows);
	cv::createTrackbar("Cols", "Parameters", &RESIZE_COLS, 1080, tb_ResizeCols);
	cv::createTrackbar("START_K", "Parameters", &START_K, NUM_LM_MAX, tb_SetStartK);
	cv::createTrackbar("STOP_K", "Parameters", &STOP_K, NUM_LM_MAX, tb_SetStopK);
	//cv::createTrackbar("Sampling_over", "Parameters", &sampling_over, 1, tb_SamplingStatus);
	//cv::createTrackbar("Discarding_over", "Parameters", &discarding_over, 1, tb_DiscardingStatus);
	cv::imshow("Parameters", dummy_mat);


	cv::Mat fv_at_present;

	try
	{
		string fn = "C:/Users/shubham/Desktop/test/resources/shape_predictor_68_face_landmarks.dat";

		// We need a face detector.  We will use this to get bounding boxes for
		// each face in an image.
		frontal_face_detector detector = get_frontal_face_detector();
		shape_predictor sp;
		//deserialize(argv[1]) >> sp;
		deserialize(fn) >> sp;
		//VIDEO EXTRACTION
		while (next_viseme_choice != 'n')

		{
			VA.setListOfWords();
			//---------update main params-----------
			ss.clear();
			ss << CONST_DEFAULT_VID_FOLDER;
			ss << folder_updater;
			ss >> default_vid_folder;

			feat.updateIMGLOC(default_vid_folder);


			feat.resetEverything();

			feat.showParams();
			Delay(1, "wait");
			while (waitKey(30) != 's')
			{
				vid >> I;
				flip(I, I, 1);
				cv::resize(I, I, cv::Size(RESIZE_COLS, RESIZE_ROWS));
				imshow("I", I);

			}





			//int key;
			while (true)
			{
				if (waitKey(10) == 'p') {
					while (waitKey(0) != 'o') {}
				}
				frames_elapsed += 1;
				frame_count += 1;

				array2d<rgb_pixel> img;

				vid >> I;


				cv::resize(I, I, cv::Size(RESIZE_COLS, RESIZE_ROWS));

				cv::flip(I, I, 1);
				assign_image(img, cv_image<bgr_pixel>(I));

				// Now tell the face detector to give us a list of bounding boxes
				// around all the faces in the image.
				std::vector<dlib::rectangle> dets = detector(img);
				//cout << "Number of faces detected: " << dets.size() << endl;
				// Now we will go ask the shape_predictor to tell us the pose of
				// each face we detected.
				std::vector<full_object_detection> shapes;
				std::vector<full_object_detection> shapes_p;
				full_object_detection shape_temp;


				for (unsigned long j = 0; j < (dets.size()); ++j)
				{
					full_object_detection shape = sp(img, dets[j]);

					num_landmarks = shape.num_parts();


					for (int k = START_K; k < STOP_K; k++)
					{
						cv::circle(I, cv::Point(shape.part(k).x(), shape.part(k).y()), 2, cv::Scalar(255, 255, 0), -1, 8);
					}

					cv::line(I, cv::Point(shape.part(51).x(), shape.part(51).y()),
						cv::Point(shape.part(57).x(), shape.part(57).y()), cv::Scalar(255, 0, 0), 1);

					cv::line(I, cv::Point(shape.part(54).x(), shape.part(54).y()),
						cv::Point(shape.part(48).x(), shape.part(48).y()), cv::Scalar(0, 255, 0), 1);

					cv::line(I, cv::Point(shape.part(50).x(), shape.part(50).y()),
						cv::Point(shape.part(58).x(), shape.part(58).y()), cv::Scalar(0, 255, 0), 1);

					cv::line(I, cv::Point(shape.part(53).x(), shape.part(53).y()),
						cv::Point(shape.part(49).x(), shape.part(49).y()), cv::Scalar(0, 255, 0), 1);
					if (j == 0)
					{
						feat.updatePoint(0, shape.part(48).x(), shape.part(48).y());
						feat.updatePoint(1, shape.part(49).x(), shape.part(49).y());
						feat.updatePoint(2, shape.part(50).x(), shape.part(50).y());
						feat.updatePoint(3, shape.part(51).x(), shape.part(51).y());
						feat.updatePoint(4, shape.part(52).x(), shape.part(52).y());
						feat.updatePoint(5, shape.part(53).x(), shape.part(53).y());
						feat.updatePoint(6, shape.part(54).x(), shape.part(54).y());

						feat.updatePoint(7, shape.part(55).x(), shape.part(55).y());
						feat.updatePoint(8, shape.part(56).x(), shape.part(56).y());
						feat.updatePoint(9, shape.part(57).x(), shape.part(57).y());
						feat.updatePoint(10, shape.part(58).x(), shape.part(58).y());
						feat.updatePoint(11, shape.part(59).x(), shape.part(59).y());

						feat.calcAllVectors();
						feat.getCurrentVectorMat(fv_at_present, 1);

						//cout << "FV_MAT"<<fv_at_present<<endl;

						float response = (svm->predict(fv_at_present));
						test_viseme_count += 1;//remove later
						note_visemes << test_viseme_count << ": " << response << "  " << (int)response << endl;//remove later
						VA.addElementToLatestBuffer((int)response);
						VA.noteInRecordPad();
						//std::vector<string> list_of_keys_OPR(0);


						if (VA.flag == 1)
							//VA.showRecord();
						{
							////list_of_keys_OPR.clear();
							// = VA.reduceByOPR(VA.record_pad);
							//cout << "\nList of Keys\n";

							VA.showRecordInConsole();
							//VA.generateNewKeyOfWord();
							//VA.pushLongKey(); put inside addToBufferPad
							VA.clearRecord();

							VA.flag = 0;
						}

						if (VA.getBufferStatus())
						{
							VA.chooseDataForStorage();
							VA.parseBufferDataToFile();
							VA.storeLongKeys();
							VA.storeSelectedLongKeys();



							VA.clearBufferPad();

							flag_gil = true;
							VA.cleanDataMembers_1();
							break;
						}

						ss.clear();
						ss << (response);
						switch ((int)response)
						{
						case 0:
							ss << "_P";
							break;
						case 1:
							ss << "_k";
							break;
						case 2:
							ss << "_f";
							break;

						case 3:
							ss << "_Ae";
							break;

						case 4:
							ss << "_AA";
							break;

						case 5:
							ss << "_EE";
							break;

						case 6:
							ss << "_(aww)";
							break;

						case 7:
							ss << "_OO";
							break;

						default:
							ss << " oops";


						}




						ss >> frame_message;
						//cv::putText(I, frame_message, cv::Point(I.rows*0.3, I.cols*0.6), cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0,255,255),2);
						cv::putText(I, frame_message, cv::Point(I.rows*0.2, I.cols*0.7), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 2);




						detected_frame_count += 1;

					}


				}


				//--------------------------------------------------------------
				ss.clear();
				ss << (detected_frame_count);
				ss >> frame_message;

				cv::putText(I, frame_message, cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0), 1);
				//---------------------------------------------------------------
				cv::imshow("I", I);
				waitKey(10);

				if (flag_gil)
				{
					flag_gil = false;
					break;
				}


			}//inner while




			cout << "For collecting data for next WORD SET, type 'y' or 'n' to end. " << endl;
			cin >> next_viseme_choice;







		}//outer while








	}
	catch (exception& e)
	{
		cout << "\nexception thrown!" << endl;
		cout << e.what() << endl;
	}









	system("pause");






	//------------older end-------------------------








# endif












#if 0
	//testing functions
	VisemeAnalyzer VAT;
	
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
			cout << VisemeAnalyzer::RR_DATA[i][j] << " ";
		cout << endl;
	}
	Delay(1, "Mat");



	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			cout << "rel_s(" << i << "," << j << ")=" << VAT.rel_s(i, j) << endl;

	std::vector<string > test_keys(1, "13131555000121233");
	test_keys.push_back("3130055101011");
	test_keys.push_back("1313000515151088833");
	test_keys.push_back("135615556667778881813");
	test_keys.push_back("1313000022277776655522233");
	test_keys.push_back("1313130022266677227775556622133");
	for (int i = 0; i < test_keys.size(); i++)
	{
		cout << i << ":Key\n";
		VAT.test_vv = VAT.genRecordPad(test_keys[i]);
		VAT.showVectViseme(VAT.test_vv);
		cout << "After Removal" << endl;
		VAT.removeAE_K(VAT.test_vv);
		VAT.showVectViseme(VAT.test_vv);
	}
	std::vector <int > 	ve_int = VAT.groupVisemes_A(VAT.test_vv);
	for (int i = 0; i < ve_int.size(); i++)
	{
		cout << i << " : " << ve_int[i] << endl;
	}

	ve_int = VAT.groupVisemes_AB(VAT.test_vv);
	for (int i = 0; i < ve_int.size(); i++)
	{
		cout << i << " : " << ve_int[i] << endl;
	}
	
	ve_int.clear();
	for (int i = 0; i < VAT.test_vv.size(); ++i)
	{
		ve_int.push_back(VAT.weightOf(VAT.test_vv[i]));

		
	}
	cout << "weights" << endl;
	for (int i = 0; i < ve_int.size(); i++)
	{
		cout << i << " : " << ve_int[i] << endl;
	}
	ve_int = VAT.getIdxOfLeast(ve_int);
	for (int i = 0; i < ve_int.size(); i++)
	{
		cout << i << " : " << ve_int[i] << endl;
	}
	VAT.showVectViseme(VAT.test_vv);
	VAT.test_vv = VAT.reduceByIR(VAT.test_vv);
	VAT.showVectViseme(VAT.test_vv);

	string tst_key = "13000053334441617733";
	cout << " New Key=" << tst_key << endl;
	VAT.test_vv = VAT.genRecordPad(tst_key);
	VAT.showVectViseme(VAT.test_vv);
	VAT.removeAE_K(VAT.test_vv);
	VAT.showVectViseme(VAT.test_vv);
	VAT.test_vv = VAT.localReduceByRR(VAT.test_vv, 1, 4);
	VAT.showVectViseme(VAT.test_vv);

	string tst_key2 = "13222666632244666133";
	cout << " New Key=" << tst_key2 << endl;
	VAT.test_vv = VAT.genRecordPad(tst_key2);
	VAT.removeAE_K(VAT.test_vv);
	std::vector<int> partitions = VAT.groupVisemes_A(VAT.test_vv);
	showAnyVector<int>(partitions);
	VAT.test_vv = VAT.reduceByRR(VAT.test_vv,partitions);
	VAT.showVectViseme(VAT.test_vv);
	
	VAT.test_vv = VAT.reduceByIR(VAT.test_vv);
	VAT.showVectViseme(VAT.test_vv);


	partitions = VAT.groupVisemes_A(VAT.test_vv);
	showAnyVector<int>(partitions);
	VAT.test_vv = VAT.reduceByRR(VAT.test_vv, partitions);
	VAT.showVectViseme(VAT.test_vv);


	partitions = VAT.groupVisemes_A(VAT.test_vv);
	showAnyVector<int>(partitions);
	cout << "Direct through fn\n";
	VAT.test_vv = VAT.reduceByRR(VAT.test_vv, VAT.groupVisemes_A(VAT.test_vv));
	VAT.showVectViseme(VAT.test_vv);

	partitions = VAT.groupVisemes_A(VAT.test_vv);
	showAnyVector<int>(partitions);
	VAT.test_vv = VAT.reduceByRR(VAT.test_vv, partitions);
	VAT.showVectViseme(VAT.test_vv);
	
	VAT.test_vv = VAT.reduceByIR(VAT.test_vv);
	VAT.showVectViseme(VAT.test_vv);

	VAT.test_vv = VAT.reduceByIR(VAT.test_vv);
	VAT.showVectViseme(VAT.test_vv);

	VAT.test_vv = VAT.reduceByIR(VAT.test_vv);
	VAT.showVectViseme(VAT.test_vv);
	
	
	
	
	Delay(1500, "See");



# endif




























}

// ----------------------------------------------------------------------------------------
