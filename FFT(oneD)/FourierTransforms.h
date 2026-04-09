#include<vector>
#include<complex>
#define M_PI 3.14159265358979323846

#pragma once
class FourierTransforms
{
#pragma region DFT



	void DFT(std::vector<double> x, std::vector<double> y, int _NPoints, std::vector < std::complex<double >> & fft) {
		for (int k = 0; k < x.size(); k++) {
			std::complex<double> sum = 0;
			for (int n = 0; n < x.size(); n++) {
				double angle = -2.0 * M_PI * k * n / _NPoints;
				std::complex<double> W = std::polar(1.0, angle);

				sum += y[n] * W;
			}

			fft[k] = sum;
		}
	}
#pragma endregion

#pragma region FFT

	std::vector<std::complex<double>> FFT(std::vector<std::complex<double>> arr) {
		if (arr.size() == 1) {
			return arr;
		}

		std::vector < std::complex<double>> even(arr.size() / 2), odd(arr.size() / 2);


		for (int m = 0; m < arr.size() / 2; m++) {
			even[m] = arr[2 * m];
			odd[m] = arr[2 * m + 1];

		}

		std::vector< std::complex<double>> E = FFT(even);
		std::vector< std::complex<double>> O = FFT(odd);

		std::vector< std::complex<double>> X(arr.size());

		for (int k = 0; k < arr.size() / 2; k++) {
			std::complex<double> W = std::polar(1.0, -2.0 * M_PI * k / arr.size());

			std::complex<double> t = W * O[k];

			X[k] = E[k] + t;
			X[k + arr.size() / 2] = E[k] - t;

		}

		return X;

	}

#pragma endregion
};

