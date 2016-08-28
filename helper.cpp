//#include "stdafx.h"
#include "helper.h"

void hDelay(double DELAY_TIME, string MESSAGE)
{
	double t = (double)cv::getCPUTickCount();
	double f = (double)cv::getTickFrequency();

	cout << "~~~ Waiting for " << DELAY_TIME << " secs ~~~ INFO: " << MESSAGE << endl;

	while (((cv::getCPUTickCount() - t) / f) < DELAY_TIME) {}

}

void SVMtest() {
	Mat trainingData = (Mat_<float>(120, 4) << 5.4, 3.7, 1.5, 0.2, 4.8, 3.4, 1.6, 0.2, 4.8, 3.0, 1.4, 0.1, 4.3, 3.0, 1.1, 0.1, 5.8, 4.0, 1.2, 0.2, 5.7, 4.4, 1.5, 0.4, 5.4, 3.9, 1.3, 0.4, 5.1, 3.5, 1.4, 0.3, 5.7, 3.8, 1.7, 0.3, 5.1, 3.8, 1.5, 0.3, 5.4, 3.4, 1.7, 0.2, 5.1, 3.7, 1.5, 0.4, 4.6, 3.6, 1.0, 0.2, 5.1, 3.3, 1.7, 0.5, 4.8, 3.4, 1.9, 0.2, 5.0, 3.0, 1.6, 0.2, 5.0, 3.4, 1.6, 0.4, 5.2, 3.5, 1.5, 0.2, 5.2, 3.4, 1.4, 0.2, 4.7, 3.2, 1.6, 0.2, 4.8, 3.1, 1.6, 0.2, 5.4, 3.4, 1.5, 0.4, 5.2, 4.1, 1.5, 0.1, 5.5, 4.2, 1.4, 0.2, 4.9, 3.1, 1.5, 0.2, 5.0, 3.2, 1.2, 0.2, 5.5, 3.5, 1.3, 0.2, 4.9, 3.6, 1.4, 0.1, 4.4, 3.0, 1.3, 0.2, 5.1, 3.4, 1.5, 0.2, 5.0, 3.5, 1.3, 0.3, 4.5, 2.3, 1.3, 0.3, 4.4, 3.2, 1.3, 0.2, 5.0, 3.5, 1.6, 0.6, 5.1, 3.8, 1.9, 0.4, 4.8, 3.0, 1.4, 0.3, 5.1, 3.8, 1.6, 0.2, 4.6, 3.2, 1.4, 0.2, 5.3, 3.7, 1.5, 0.2, 5.0, 3.3, 1.4, 0.2, 5.0, 2.0, 3.5, 1.0, 5.9, 3.0, 4.2, 1.5, 6.0, 2.2, 4.0, 1.0, 6.1, 2.9, 4.7, 1.4, 5.6, 2.9, 3.6, 1.3, 6.7, 3.1, 4.4, 1.4, 5.6, 3.0, 4.5, 1.5, 5.8, 2.7, 4.1, 1.0, 6.2, 2.2, 4.5, 1.5, 5.6, 2.5, 3.9, 1.1, 5.9, 3.2, 4.8, 1.8, 6.1, 2.8, 4.0, 1.3, 6.3, 2.5, 4.9, 1.5, 6.1, 2.8, 4.7, 1.2, 6.4, 2.9, 4.3, 1.3, 6.6, 3.0, 4.4, 1.4, 6.8, 2.8, 4.8, 1.4, 6.7, 3.0, 5.0, 1.7, 6.0, 2.9, 4.5, 1.5, 5.7, 2.6, 3.5, 1.0, 5.5, 2.4, 3.8, 1.1, 5.5, 2.4, 3.7, 1.0, 5.8, 2.7, 3.9, 1.2, 6.0, 2.7, 5.1, 1.6, 5.4, 3.0, 4.5, 1.5, 6.0, 3.4, 4.5, 1.6, 6.7, 3.1, 4.7, 1.5, 6.3, 2.3, 4.4, 1.3, 5.6, 3.0, 4.1, 1.3, 5.5, 2.5, 4.0, 1.3, 5.5, 2.6, 4.4, 1.2, 6.1, 3.0, 4.6, 1.4, 5.8, 2.6, 4.0, 1.2, 5.0, 2.3, 3.3, 1.0, 5.6, 2.7, 4.2, 1.3, 5.7, 3.0, 4.2, 1.2, 5.7, 2.9, 4.2, 1.3, 6.2, 2.9, 4.3, 1.3, 5.1, 2.5, 3.0, 1.1, 5.7, 2.8, 4.1, 1.3, 6.5, 3.2, 5.1, 2.0, 6.4, 2.7, 5.3, 1.9, 6.8, 3.0, 5.5, 2.1, 5.7, 2.5, 5.0, 2.0, 5.8, 2.8, 5.1, 2.4, 6.4, 3.2, 5.3, 2.3, 6.5, 3.0, 5.5, 1.8, 7.7, 3.8, 6.7, 2.2, 7.7, 2.6, 6.9, 2.3, 6.0, 2.2, 5.0, 1.5, 6.9, 3.2, 5.7, 2.3, 5.6, 2.8, 4.9, 2.0, 7.7, 2.8, 6.7, 2.0, 6.3, 2.7, 4.9, 1.8, 6.7, 3.3, 5.7, 2.1, 7.2, 3.2, 6.0, 1.8, 6.2, 2.8, 4.8, 1.8, 6.1, 3.0, 4.9, 1.8, 6.4, 2.8, 5.6, 2.1, 7.2, 3.0, 5.8, 1.6, 7.4, 2.8, 6.1, 1.9, 7.9, 3.8, 6.4, 2.0, 6.4, 2.8, 5.6, 2.2, 6.3, 2.8, 5.1, 1.5, 6.1, 2.6, 5.6, 1.4, 7.7, 3.0, 6.1, 2.3, 6.3, 3.4, 5.6, 2.4, 6.4, 3.1, 5.5, 1.8, 6.0, 3.0, 4.8, 1.8, 6.9, 3.1, 5.4, 2.1, 6.7, 3.1, 5.6, 2.4, 6.9, 3.1, 5.1, 2.3, 5.8, 2.7, 5.1, 1.9, 6.8, 3.2, 5.9, 2.3, 6.7, 3.3, 5.7, 2.5, 6.7, 3.0, 5.2, 2.3, 6.3, 2.5, 5.0, 1.9, 6.5, 3.0, 5.2, 2.0, 6.2, 3.4, 5.4, 2.3, 5.9, 3.0, 5.1, 1.8);
	Mat trainingClasses = (Mat_<int>(120, 1) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
	Mat testData = (Mat_<float>(30, 4) << 5.1, 3.5, 1.4, 0.2, 4.9, 3.0, 1.4, 0.2, 4.7, 3.2, 1.3, 0.2, 4.6, 3.1, 1.5, 0.2, 5.0, 3.6, 1.4, 0.2, 5.4, 3.9, 1.7, 0.4, 4.6, 3.4, 1.4, 0.3, 5.0, 3.4, 1.5, 0.2, 4.4, 2.9, 1.4, 0.2, 4.9, 3.1, 1.5, 0.1, 7.0, 3.2, 4.7, 1.4, 6.4, 3.2, 4.5, 1.5, 6.9, 3.1, 4.9, 1.5, 5.5, 2.3, 4.0, 1.3, 6.5, 2.8, 4.6, 1.5, 5.7, 2.8, 4.5, 1.3, 6.3, 3.3, 4.7, 1.6, 4.9, 2.4, 3.3, 1.0, 6.6, 2.9, 4.6, 1.3, 5.2, 2.7, 3.9, 1.4, 6.3, 3.3, 6.0, 2.5, 5.8, 2.7, 5.1, 1.9, 7.1, 3.0, 5.9, 2.1, 6.3, 2.9, 5.6, 1.8, 6.5, 3.0, 5.8, 2.2, 7.6, 3.0, 6.6, 2.1, 4.9, 2.5, 4.5, 1.7, 7.3, 2.9, 6.3, 1.8, 6.7, 2.5, 5.8, 1.8, 7.2, 3.6, 6.1, 2.5);
	Mat testClasses = (Mat_<int>(30, 1) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);


	Ptr<SVM> p = SVM::create();
	p->setType(SVM::Types::C_SVC);
	p->setKernel(SVM::KernelTypes::LINEAR);

	TermCriteria criteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 100, 1e-6);
	p->setTermCriteria(criteria);
	p->train(trainingData, ROW_SAMPLE, trainingClasses);
	int count = 0;
	for (int i = 0; i < 30; i++) {
		float response = p->predict(testData.row(i));
		cout << response << " ";
		if (response == (int)i / 10) {
			count++;
		}
	}
	cout << endl;

	float error = (30.0 - count) / 0.3;
	cout << "Error is " << error << "%" << endl;
}

void fakeTrackbar(int, void*) {}

void trainBySVM(Mat trainingData, Mat trainingClasses, int SVM_KERNEL_TYPE, int term_max_count, double term_epsilon) {
	//SVMtest();

	Ptr<SVM> p = SVM::create();
	p->setType(SVM::Types::C_SVC);
	p->setKernel(SVM_KERNEL_TYPE);

	TermCriteria criteria(TermCriteria::MAX_ITER | TermCriteria::EPS, term_max_count, term_epsilon);
	p->setTermCriteria(criteria);

	p->train(trainingData, ROW_SAMPLE, trainingClasses);
	p->save("trainBySVM.xml");
}

string type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

std::vector<float> predictBySVM(Mat testData, Mat testClasses, bool DISPLAY_RESULT)
{
	//SVMtest();
	Mat predictedM(testClasses.rows, testClasses.cols, CV_16SC1);
	std::vector<float> vf;
	try
	{
		Ptr<SVM> p = Algorithm::load<SVM>("trainBySVM.xml");
		for (int i = 0; i < testData.rows; i++) {
			float response = p->predict(testData.row(i));
			vf.push_back(response);
		}
	}
	catch (const char* e)
	{
		cerr << "Error: " << e << endl;
	}

	if (DISPLAY_RESULT) {
		std::vector<float> out = predictBySVM(testData, testClasses);
		cout << "[";
		for (float j : out) {
			cout << j << " ";
		}
		cout << "]" << endl;
	}
	return vf;
}

Point getPoint(std::vector<full_object_detection> shapes, int part_number) {
	Point p(shapes[0].part(part_number).x(), shapes[0].part(part_number).y());
	return p;
}

double facePointDist(std::vector<full_object_detection> shapes, int p1, int p2, bool DISPLAY_RESULT) {
	double d = cv::norm(getPoint(shapes, p1) - getPoint(shapes, p2));
	if (DISPLAY_RESULT) {
		cout << "[" << p1 << ", " << p2 << "] :: " << setprecision(3) << d << endl;
	}
	return d;
}

double getDistRatioH(std::vector<full_object_detection> shapes, int p1, int p2, bool DISPLAY_RESULT) {
	double d1 = cv::norm(getPoint(shapes, p1) - getPoint(shapes, p2));
	double d2 = cv::norm(getPoint(shapes, 51) - getPoint(shapes, 57));
	if (DISPLAY_RESULT) {
		cout << "Ratio of {" << p1 << ", " << p2 << " & " << 51 << ", " << 57 << "} = [" << setprecision(3) << d1 / d2 << "]" << endl;
	}
	return d1 / d2;
}

double getDistRatioV(std::vector<full_object_detection> shapes, int p1, int p2, bool DISPLAY_RESULT) {
	double d1 = cv::norm(getPoint(shapes, p1) - getPoint(shapes, p2));
	double d2 = cv::norm(getPoint(shapes, 48) - getPoint(shapes, 54));
	if (DISPLAY_RESULT) {
		cout << "Ratio of {" << p1 << ", " << p2 << " & " << 48 << ", " << 54 << "} = [" << setprecision(3) << d1 / d2 << "]" << endl;
	}
	return d1 / d2;
}

double getAlpha(std::vector<full_object_detection> shapes) {
	return getDistRatioH(shapes, 48, 54, false);
}

Mat getLabelMat(int no_of_mats, std::vector<int> vector_of_lables, int repeat_count) {

}