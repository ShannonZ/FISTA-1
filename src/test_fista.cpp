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
#ifdef OPTIMIZE
	mat result = mat();
	proj_l1(result, U, lambda, pos);
#else
	try
	{
		mat result = proj_l1(U, 1.5, pos);
		//throw 2;
	}
	catch (std::invalid_argument e)
	{
		cout << e.what() << '\n';
	}
#endif
}



void test_Gradient() {
	mat D(300, 100);
	mat Y(300, 70);
	mat X(100, 70);
	D.fill(3.0);
	Y.fill(15.0);
	X.fill(1.0);



#ifdef OPTIMIZE
	mat grad = mat();
	mat DtD = mat();
	mat DtY = mat();
	CalcXtY(DtD, D, D);
	CalcXtY(DtY, D, Y);

	Gradient(grad, X, DtD, DtY);
	grad.print("Gradient:")
#else
	mat DtD = CalcXtY(D, D);
	mat DtY = CalcXtY(D, Y);
	mat grad = Gradient(X, DtD, DtY);
	grad.print("Gradient:")
#endif
;
}


void test_CalcXtY() {
	mat A(2, 3);
	mat B(2, 3);
	A.fill(3.0);
	B.fill(15.0);
#ifdef OPTIMIZE
	mat C = mat();
	CalcXtY(C, A, B);
	C.print("C:");
#else
	mat C = CalcXtY(A, B);
	C.print("C:");
#endif
}


void test_lasso() {

	using milli = std::chrono::milliseconds;
	
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
	opt.pos = true;

	mat result = mat();

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++) {
#ifdef OPTIMIZE
		
		fista_lasso(result, Y, D, opt);
#else
		result = fista_lasso(Y, D, opt);
#endif
	}

	//result.print("result:");

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

