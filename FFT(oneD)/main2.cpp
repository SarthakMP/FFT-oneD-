#include<iostream>
#include<opencv2/opencv.hpp>
#define CVPLOT_HEADER_ONLY 
#include"CvPlot/cvplot.h"
#include<math.h>

#define N 200
#define M_PI 3.14159265358979323846
/*
int main() {

	cv::Mat mat = cv::Mat::zeros(800,1200,CV_8UC3);
	std::vector<float> x(N);
	std::vector<float> sin(N), cos(N),Combo(N);
	for (int i = 0; i < N; i++) {
		x[i] = (2*M_PI * i / N);
		sin[i] = std::sin(5 * x[i]);
		cos[i] = std::cos(5 * x[i]);

		Combo[i] = sin[i] + cos[i];
	} 

	cv::Mat xMat(x), sinMat(sin), cosMat(cos);

	cv::Rect roi1(0, 0, 600, 400);
	auto axes1 = CvPlot::makePlotAxes();
	axes1.create<CvPlot::Series>(xMat, sinMat, "-b");

	cv::Mat plot1 = axes1.render(roi1.height, roi1.width);
	plot1.copyTo(mat(roi1));

	
	cv::Rect roi2(0, 400, 600, 400);
	auto axes2 = CvPlot::makePlotAxes();
	axes2.create<CvPlot::Series>(xMat, cosMat, "-r");

	cv::Mat plot2 = axes2.render(roi2.height, roi2.width);
	plot2.copyTo(mat(roi2));


	cv::Rect roi3(600, 0, 600, 400);
	auto axes3 = CvPlot::makePlotAxes();
	axes3.create < CvPlot::Series>(xMat, Combo, "-g");

	cv::Mat plot3 = axes3.render(roi3.height, roi3.width);
	plot3.copyTo(mat(roi3));

	cv::imshow("Sine & Cosine Plots", mat);
	cv::waitKey(0);
	return 0;
}*/