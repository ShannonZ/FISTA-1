#include <iostream>
#include <armadillo>

#include "fista.hpp"

using namespace std;
using namespace arma;



void test_norm1() {
	mat X(100, 70);
	X.fill(3.0);
	double result = norm1(X);
	cout << "Testing norm1: " << result << endl;
}

void test_proj_l1() {
	mat U(100, 70);
	double lambda = 0.78;
	bool pos = true;
	mat result = proj_l1(U, lambda, pos);
}



void test_Gradient() {
	mat D(300, 100);
	mat Y(300, 70);
	mat X(100, 70);
	D.fill(3.0);
	Y.fill(15.0);
	X.fill(1.0);

	mat DtD = CalcXtY(D, D);
	mat DtY = CalcXtY(D, Y);

	mat grad = Gradient(X, DtD, DtY);
	grad.print("Gradient:");
}


void test_CalcXtY() {
	mat A(2, 3);
	mat B(2, 3);
	A.fill(3.0);
	B.fill(15.0);
	mat C = CalcXtY(A, B);
	C.print("C:");
}


void test_lasso() {

	using milli = std::chrono::milliseconds;
	auto start = std::chrono::high_resolution_clock::now();
	
	for (int i = 0; i < 100; i++) {
		// d = 10; 	% data dimension
		//	N = 20; 	% number of samples
		//	k = 30; 	% dictionary size
		//	lambda = 0.01;
		// Y = normc(rand(d, N));
		// D = normc(rand(d, k));

		int d = 10; 	// % data dimension
		int N = 20; 	// % number of samples
		int k = 30; 	// % dictionary size
		double lambda = 0.01;

		mat a = randu<mat>(d, N);
		mat b = randu<mat>(d, k);

		mat Y = normalise(a);
		mat D = normalise(b);

		lasso_options opt;

		opt.lambda = 0.01;
		opt.max_iterations = 500;
		opt.tolerance = 1e-8;

		fista_lasso(Y, D, opt);

	}
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "fista_lasso() took "
		<< std::chrono::duration_cast<milli>(finish - start).count()
		<< " milliseconds\n";

}

void test_all() {
	//test_CalcXtY();
	//test_Gradient();
	//test_proj_l1();
	//test_norm1();
	test_lasso();
}


int
main(int argc, char** argv)
{
	test_all();
	return 0;
}

