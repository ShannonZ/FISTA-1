#ifndef FISTA_HPP
#define FISTA_HPP

#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

// #define OPTIMIZE

struct lasso_options {
	double lambda;
	double tolerance;
	int max_iterations;
	bool pos;
};

// function res = norm1(X)
// res = full(sum(abs(X(:))));
// end
double norm1(mat &X);

#ifdef OPTIMIZE
// Implements proj_l1.m function 
void proj_l1(mat &U, mat &Out, double lambda, bool pos);
#else
mat proj_l1(mat &U, double lambda, bool pos);
#endif

// DtD = D'*D;
// DtY = D'*Y;
// function res = grad(X)
// res = DtD*X - DtY;
// end
#ifdef OPTIMIZE
void Gradient(mat &Gradiant, mat &X, mat &DtD, mat &DtY);
#else
mat Gradient(mat &X, mat &DtD, mat &DtY);
#endif

// Compute XtY2 = X'*Y;
#ifdef OPTIMIZE
void CalcXtY(mat &XtY, mat &X, mat &Y);
#else
mat CalcXtY(mat &X, mat &Y);
#endif

#ifdef OPTIMIZE
// Fista general algorithm
void fista_general(mat &Out, mat &Xinit, mat &DtD, mat &DtY, double L, lasso_options &opts);
// Fista lasso algorithm
void fista_lasso(mat &Out,  mat &Y, mat &D, lasso_options &opts);
#else
// Fista general algorithm
mat fista_general(mat &Xinit, mat &DtD, mat &DtY, double L, lasso_options &opts);
// Fista lasso algorithm
mat fista_lasso(mat &Y, mat &D, lasso_options &opts);
#endif

#endif
// end of file 
