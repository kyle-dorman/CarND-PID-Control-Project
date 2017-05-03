#ifndef TWIDDLE_H
#define TWIDDLE_H

#include <vector>

class Twiddle {
	enum NextAction { reset, decrement };
public:
	/*
  * Constructor
  */
  Twiddle();

  /*
  * Destructor.
  */
  virtual ~Twiddle();

	void Init(std::vector<double>& _coefficients,
		int _max_iterations,
		int _start_after_iterations,
		double _tolerance);

	void UpdateError(double error);

	std::vector<double> Coefficients();

private:

	std::vector<double> coefficients;

	std::vector<double> coefficients_delta;

	double tolerance;

	double best_error;

	double total_error;

	int iteration;

	int max_iterations;

	int start_after_iterations;

	Twiddle::NextAction next;

	unsigned int index;

	void Reset();

	void Decrement();

	void PrintCoefficients();

	void PrintBestScore();

};

#endif /* TWIDDLE_H */