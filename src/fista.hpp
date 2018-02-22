#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

struct lasso_options {
	double lambda;
	double tolerance;
	int max_iterations;
};

// function res = norm1(X)
// res = full(sum(abs(X(:))));
// end
double norm1(mat &X);

// X = max(0, U - lambda);
mat proj_l1(mat &U, double &lambda);

// DtD = D'*D;
// DtY = D'*Y;
// function res = grad(X)
// res = DtD*X - DtY;
// end
mat Gradient(mat &X, mat &DtD, mat &DtY);

// Compute XtY2 = X'*Y;
mat CalcXtY(mat &X, mat &Y);

// Fista general algorithm
mat fista_general(mat &Xinit, mat &DtD, mat &DtY, double &L, lasso_options &opts);

// Fista lasso algorithm
mat fista_lasso(mat &Y, mat &D, lasso_options &opts);
