#include<iostream>
#include<math.h>
#include"opencv2/opencv.hpp"
#define CVPLOT_HEADER_ONLY 
#include"CvPlot/cvplot.h"

#include"Interpolation_1D.h"
#include"Interpolation_2D.h"

#define N 32768 // Number of samples
#define M_PI 3.14159265358979323846

std::vector<double> x(N); // Xs output array
std::vector<double> y(x.size()); // Y output array
std::vector<double> freq(N);
std::vector < std::complex<double>> input(N);

#define Switch 2


void CombinationOfSinAndCosWaves(int WaveSamplePoints/*X size*/) {

	// Create a combination of sin and cos waves
	for (int n = 0; n < WaveSamplePoints; ++n) {
		x[n] = (2.0 * M_PI * 6 * n / WaveSamplePoints); // X values from 0 to 2*PI
		y[n] = 18.0 * cos(x[n]) + 9.0 * sin(x[n] * 2.0);
		freq[n] = static_cast<double>(n);
		input[n] = std::complex<double>(y[n], 0.0);
	}
}





int main() {


#pragma region OpenCV/CvPlot Setup

	cv::namedWindow("Signal", cv::WINDOW_AUTOSIZE);

#pragma endregion 



#pragma region Interpolation
#define N_Point 10
	Interpolation_1D interp_1d;
	Interpolation_2D interp_2d;

#if Switch == 1
#pragma region Making Point


	cv::Mat canvas = cv::Mat::zeros(1000, 500, CV_8UC3);
	auto Axes = CvPlot::makePlotAxes();
	Axes.xLabel("X");
	Axes.yLabel("Y");

	//Axes.setXLim(std::pair<double,double>(-1, 1));

	std::vector<cv::Point2d> Points;
	Points.reserve(N_Point);

	for (double i = -N_Point / 2.0; i < N_Point / 2.0; i += M_PI / 4) {

		double x = (i * M_PI / 2);
		double y = 5 * std::sin(x / 10) + 2 * std::cos(x / 4);
		Points.emplace_back(x, y);
	}

	int new_size = (Points.size() - 1) * N_Point + 1;


	cv::Rect roiPoints(0, 0, 500, 500);



#pragma endregion
#endif

#pragma region Test Interpolation

	cv::Mat Image = cv::imread("./Images/butterfly.png", CV_8UC1);
	cv::Mat Lanczos_sampled_img =  interp_2d.Lanczos_Interpolation_2d(Image,3);

	cv::imshow("Signal", Lanczos_sampled_img);
	cv::imwrite("./Images/interpolated.png", Lanczos_sampled_img*255);
	

#pragma endregion


#pragma endregion


	cv::waitKey(0);
	return 0;
}


