#include<iostream>
#include<math.h>
#include"opencv2/opencv.hpp"
#define CVPLOT_HEADER_ONLY 
#include"CvPlot/cvplot.h"

#define N 32768 // Number of samples
#define M_PI 3.14159265358979323846

std::vector<double> x(N); // Xs output array
std::vector<double> y(x.size()); // Y output array
std::vector<double> freq(N);
std::vector < std::complex<double>> input(N);


void CombinationOfSinAndCosWaves(int WaveSamplePoints/*X size*/) {

	// Create a combination of sin and cos waves
	for (int n = 0; n < WaveSamplePoints; ++n) {
		x[n] = ( 2.0*M_PI* 6 * n / WaveSamplePoints); // X values from 0 to 2*PI
		y[n] = 18.0 * cos(x[n]) + 9.0 * sin(x[n]*2.0);
		freq[n] = static_cast<double>(n);	
		input[n] = std::complex<double>(y[n], 0.0);
	}
}

#pragma region FFT

std::vector<std::complex<double>> FFT(std::vector<std::complex<double>> arr) {
	if (arr.size() == 1) {
		return arr;
	}
	
	std::vector < std::complex<double>> even(arr.size()/2), odd(arr.size() / 2);


	for (int m = 0; m < arr.size() / 2; m++) {
		even[m] = arr[2 * m];
		odd [m] = arr[2 * m + 1];

	}

	std::vector< std::complex<double>> E = FFT(even);
	std::vector< std::complex<double>> O = FFT( odd);

	std::vector< std::complex<double>> X(arr.size());

	for (int k = 0; k < arr.size() / 2; k++) {
		std::complex<double> W = std::polar(1.0, - 2.0 * M_PI * k / arr.size());

		std::complex<double> t = W * O[k];

		X[k] = E[k] + t;
		X[k + arr.size()/2] = E[k] - t;

	}

	return X;

}

#pragma endregion


double BellCurve(double in) {
	return std::exp(-(in * in)*0.001);
}


double Interpolation(uchar p1, uchar p2, double t) {
	return static_cast<double>(p1 + (p2 - p1) * t);
}

double CubicInterp(double p[4], double x) {

	double a = -0.5 * p[0] + 1.5 * p[1] - 1.5 * p[2] + 0.5 * p[3];
	double b = p[0] - 2.5 * p[1] + 2 * p[2] - 0.5 * p[3];
	double c = -0.5 * p[0] + 0.5*p[2];
	double d = p[1];

	return (((a * x + b) * x + c) * x + d);
}

 int main() {

#pragma region OpenCV/CvPlot Setup

	cv::namedWindow("Signal", cv::WINDOW_AUTOSIZE); 

	/*
	for (size_t i = 0; i < x.size(); i++) {
		x[i] = i * CV_2PI / x.size();
		y[i] = std::sin(x[i]);
	}*/

	//INPUT as Combination of Sin and Cos Waves
	//CombinationOfSinAndCosWaves(N);
	//auto axes1 = CvPlot::makePlotAxes();
	//axes1.create<CvPlot::Series>(x, y, "-g");
	//CvPlot::show("Signal", axes1);
#pragma endregion 
	
	//std::vector< std::complex<double>> fft(N);
	//fft = FFT(input);
	
	
#pragma region DFT
		/*
	for (int k = 0; k < x.size(); k++) {
		std::complex<double> sum = 0;
		for (int n = 0; n < x.size(); n++) {
			double angle = -2.0 * M_PI * k * n / N;
			std::complex<double> W = std::polar(1.0, angle);
			
			sum += y[n] * W;
		}
		
		fft[k] = sum;
	}
	*/

#pragma endregion 

#pragma region Interpolation
#define N_Point 61
#pragma region Making Point
	
	
	cv::Mat canvas =cv::Mat::zeros(500, 500, CV_8UC3);
	auto Axes = CvPlot::makePlotAxes();
	Axes.xLabel("X");
	Axes.yLabel("Y");
	
	Axes.setXLim(std::pair<double,double>(- 30, 30));

	std::vector<cv::Point2d> Points;
	Points.reserve(N_Point);

	for (int i = -30; i < N_Point-30; i++) {
		double x = static_cast<double>(i);
		double y = BellCurve(x);

		Points.emplace_back(x, y);
	}


	cv::Rect roiPoints(0, 0, 500, 500);
	//Axes.create<CvPlot::Series>(Points,"o r");
	
	

#pragma endregion

#pragma region Nearest_interpolation
	/*
	std::vector<cv::Point2d > New_Points;
	New_Points.reserve(N_Point *2);
	for (int i = 1; i < N_Point; i++) {
		cv::Point2d Point1(Points[i - 1]), Point2(Points[i]);
		
		New_Points.push_back(Point1);

		double half = (Point1.x + Point2.x) / 2;
		cv::Point2d mid1(half,Point1.y), mid2(half, Point2.y);
		New_Points.push_back(mid1);
		New_Points.push_back(mid2);

		New_Points.push_back(Point2);
	}
	
	Axes.create<CvPlot::Series>(New_Points, "-r");
	*/

#pragma endregion

#pragma region Linear_interpolation
	
	/*
	std::vector<cv::Point2d > New_Points;
	New_Points.reserve(N_Point * 4);
	for (int i = 1; i < N_Point; ++i) {
		cv::Point2d Point1(Points[i-1]), Point2(Points[i]);
		
		New_Points.push_back(Point1);
		
		for (double j = Points[i - 1].x; j < Points[i].x; j+=0.25) {
			
			double y = Point1.y + (Point2.y - Point1.y) * (j - Point1.x) / (Point2.x - Point1.x);
			cv::Point2d tmp_points(j, y);
			New_Points.push_back(tmp_points);
		
		}
		New_Points.push_back(Point2);

	}
	
	Axes.create<CvPlot::Series>(New_Points, "-r");
	
	*/
#pragma endregion

#pragma region Cubic_interpolation
	/*
	std::vector<cv::Point2d> new_points;
	new_points.reserve(Points.size() * 8);
	int count = 0;
	new_points.emplace_back(Points[0].x, Points[0].y);
	for (int i = 1; i < Points.size()-2; i++) {

		double x0 = Points[i - 1].x;
		double x1 = Points[i ].x;
		double x2 = Points[i + 1].x;
		double x3 = Points[i + 2].x;

		double y0 = Points[i - 1].y;
		double y1 = Points[i].y;
		double y2 = Points[i + 1].y;
		double y3 = Points[i + 2].y;

		new_points.emplace_back(Points[i].x, Points[i].y);
		double fx = x2 - x1;
		if (fx <= 0) continue;

		for (double t = 0; t < 1.0; t += 0.2) {
			
			double ex_y[4] = { y0, y1,y2 ,y3 };
				
			double interp_y = CubicInterp(ex_y, t);
			
			double interp_x = x1 + t * fx;
			new_points.emplace_back(interp_x, interp_y);

		}


	}

	if (!Points.empty()) {
		new_points.emplace_back(Points.back().x, Points.back().y);
		
	}

	std::cout << "Count: " << count;
	
	Axes.create<CvPlot::Series>(new_points, "-r");
	*/

	/*
	cv::Mat plot = Axes.render(roiPoints.height, roiPoints.width);
	plot.copyTo(canvas(roiPoints));
	cv::imshow("Signal", plot);
	*/
#pragma endregion

#pragma endregion

	/*
	cv::Mat plot1 = Axes.render(roiPoints.height, roiPoints.width);
	plot1.copyTo(canvas(roiPoints));

	cv::imshow("Signal", canvas);
	*/

	/*
	double max_mag = 0;
	int peak_bin = 0;
	
	std::vector<double> mag(N),phase(N);
	for (int k = 0; k < N/2; k++) {
		mag[k] = std::abs(fft[k])/N;
		if (mag[k] > max_mag) {
			max_mag = mag[k];
			peak_bin = k;
		}
		phase[k] = std::abs(fft[k]);
	}
	
	std::cout << "Strongest peak " << peak_bin << " With mag " << max_mag << std::endl;
	*/
	
	/*
	cv::Mat mat = cv::Mat::zeros(800, 1200, CV_8UC3);
	
	cv::Rect roiTime(0, 400, 1200, 400);
	auto axes1 = CvPlot::makePlotAxes();
	axes1.create<CvPlot::Series>(x, y, "-r");
	cv::Mat plot3 = axes1.render(roiTime.height, roiTime.width);
	plot3.copyTo(mat(roiTime));


	
	cv::Rect roiMag(0, 0, 600, 400);
	auto axes2 = CvPlot::makePlotAxes();
	axes2.create<CvPlot::Series>(freq, mag, "-b");
	cv::Mat plot1 = axes2.render(roiMag.height, roiMag.width);
	plot1.copyTo(mat(roiMag));

	

	cv::Rect roiPhase(600, 0, 600, 400);
	auto axes3 = CvPlot::makePlotAxes();
	axes3.create<CvPlot::Series>(freq, phase, "-r");
	cv::Mat plot2 = axes3.render(roiPhase.height, roiPhase.width);
	plot2.copyTo(mat(roiPhase));
	*/

	/*
#pragma region Bilinear
	cv::Mat image = cv::imread("./Images/baby.png");
	cv::Mat Converted_image;
	image.convertTo(Converted_image, CV_8UC3);
	int h_old = image.rows, w_old = image.cols;
	cv::Mat Gray_image(h_old, w_old, CV_8UC1);
	
	double t = 0.5;
	
	for (int r = 0; r < h_old; r++) {
		cv::Vec3b* rowptr = Converted_image.ptr<cv::Vec3b>(r);
		uchar* dst = Gray_image.ptr<uchar>(r);
		for (int c = 0; c < w_old; c++) {
			int r = rowptr[c][0];
			int g = rowptr[c][1];
			int b = rowptr[c][2];
			
			dst[c] = (r + g + b) / 3;
		}
	}
	
	cv::Mat BilinearUpScaled = cv::Mat::zeros(2 * h_old, 2 * w_old, CV_8UC1);
	
	int upscaled_height = BilinearUpScaled.rows, upscaled_width = BilinearUpScaled.cols;

	for (int i = 0; i < h_old - 1; i++) {

		uchar* row0 = Gray_image.ptr<uchar>(i);
		uchar* row1 = Gray_image.ptr<uchar>(i+1);

		uchar* up_row0 = BilinearUpScaled.ptr<uchar>(2*i);
		uchar* up_midrow = BilinearUpScaled.ptr<uchar>(2*i + 1);

		for (int j = 0; j < w_old - 1; j++) {

			uchar p1 = row0[j];
			uchar p2 = row0[j+1];

			uchar p3 = row1[j];
			uchar p4 = row1[j+1];

			uchar top = Interpolation(p1, p2, t);
			uchar bottom = Interpolation(p3, p4, t);

			uchar mid = Interpolation(top, bottom, t);

			uchar left = Interpolation(p1, p3, t);
			uchar right = Interpolation(p2, p4, t);

			up_row0[2*j] = p1;
			up_row0[2*j + 1] = top;
			up_row0[2*j + 2] = p2;

			up_midrow[2*j] = left;
			up_midrow[2*j + 1] = mid;
			up_midrow[2*j + 2] = right;

		}

	}

	cv::imwrite("./Images/Image1.png", BilinearUpScaled);
	cv::imshow("Signal", BilinearUpScaled);
#pragma endregion
*/
	
	#pragma region Bicubic

//cv::Mat image = cv::imread("./Images/baby.png");
cv::Mat Converted_image= cv::imread("./Images/baby.png", cv::IMREAD_GRAYSCALE);;

int h_old = Converted_image.rows, w_old = Converted_image.cols;

cv::Mat new_Img = cv::Mat::zeros(h_old * 2, w_old*2, CV_8UC1);

int h_new = h_old * 2, w_new = w_old * 2;



for (int dst_y = 0; dst_y < h_new; dst_y++) {
	uchar* new_img_row_ptr = new_Img.ptr<uchar>(dst_y);
	for (int dst_x = 0; dst_x < w_new; dst_x++) {
		
		double src_x = (dst_x + 0.5) * (static_cast<double>(w_old)  / w_new) - 0.5;
		double src_y = (dst_y + 0.5) * (static_cast<double>(h_old) / h_new) - 0.5;

		int x0 = static_cast<int>(std::floor(src_x)) - 1;
		int y0 = static_cast<int>(std::floor(src_y)) - 1;

		double fx = src_x - std::floor(src_x);
		double fy = src_y - std::floor(src_y);
		
		double tmp[4] = { 0.0};
		
		for (int r = 0; r < 4; r++) {
			double p[4];
			int py = std::clamp(r + y0, 0, h_old - 1);
			uchar* row_ptr = Converted_image.ptr<uchar>(py);
			
			for (int c = 0; c < 4; c++) {
				int px = std::clamp(c + x0, 0, w_old - 1);
				
				p[c] = row_ptr[px];

			}
			tmp[r] = CubicInterp(p, fx);
			
			
		}

		double final_val = CubicInterp(tmp, fy);
		new_img_row_ptr[dst_x] = cv::saturate_cast<uchar>(final_val);

	}

}
	
cv::imshow("Signal", new_Img);

#pragma endregion

	

	
	cv::waitKey(0);
	return 0;
}


