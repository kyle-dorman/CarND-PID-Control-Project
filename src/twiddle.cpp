#include "twiddle.h"
#include "vector"
#include <iostream> // std::cout
#include <cmath> // std::abs

using namespace std;

void exit() {
	cout << "Found error below tolerance. Exiting..." << endl;
	std::vector<int> v;
	v[0];
	return;
}

Twiddle::Twiddle() {}

Twiddle::~Twiddle() {}

void Twiddle::Init(vector<double>& _coefficients,
		int _max_iterations,
		int _start_after_iterations,
		double _tolerance) 
{
	coefficients = _coefficients;

	coefficients_delta = { 0.3, 2.0, 0.01 };

	tolerance = _tolerance;

	best_error = numeric_limits<double>::max();

	total_error = 0.0;

	iteration = 0;

	max_iterations = _max_iterations;

	start_after_iterations = _start_after_iterations;

	next = reset;

	index = coefficients.size() - 1;

	// reset will increment coefficients[0], so decrement it beforehand.
	coefficients[0] -= coefficients_delta[0];

	Reset();
}

vector<double> Twiddle::Coefficients() {
	return coefficients;
}

void Twiddle::UpdateError(double cte) {
	iteration++;
	if (iteration < start_after_iterations) {
		return;
	}

	total_error += abs(cte);

	if (iteration >= max_iterations) {
		double delta = best_error - total_error;
		if (delta > 0) {
			best_error = total_error;
			coefficients_delta[index] *= 1.1;
			PrintBestScore();
			if (delta < tolerance && index == coefficients.size() - 1) {
				exit();
			} else {
				Reset();	
			}
		} else if (next == decrement) {
			Decrement();
		} else {
			coefficients[index] += coefficients_delta[index];
      coefficients_delta[index] *= 0.9;
      Reset();
		}
	}
}

void Twiddle::Reset() {
	index = (index + 1) % coefficients.size();
	coefficients[index] += coefficients_delta[index];
	cout << "Total Error: " << total_error << endl;
	cout << "Twiddle::Reset new index " << index << " coefficient is " << coefficients[index] << endl;
	next = decrement;
	total_error = 0.0;
	iteration = 0;
	PrintCoefficients();
}

void Twiddle::Decrement() {
	cout << "Total Error: " << total_error << endl;
	cout << "Twiddle::Decrement " << index << " from " << coefficients[index] << " by " << 2 * coefficients_delta[index] << endl;
	coefficients[index] -= 2 * coefficients_delta[index];
	next = reset;
	total_error = 0.0;
	iteration = 0;
	PrintCoefficients();
}

void Twiddle::PrintCoefficients() {
	cout << "Coefficients:" << endl;
	for (size_t i = 0; i < coefficients.size(); ++i) {
		cout << i << ": " << coefficients[i] << endl;
	}
}

void Twiddle::PrintBestScore() {
	cout << "**************************" << endl;
	cout << "New best_error: " << best_error << endl;
	PrintCoefficients();
	cout << "**************************" << endl;
}
