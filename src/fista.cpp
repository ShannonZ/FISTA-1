#include <iostream>
#include <armadillo>

#include "fista.hpp"

#ifdef DEBUG
#include <typeinfo>
#endif

using namespace std;
using namespace arma;

// function res = norm1(X)
// res = full(sum(abs(X(:))));
// end
#ifdef OPTIMIZE
double norm1(mat &X) {
	return (double)sum(abs(vectorise(X)));
}
#else
double norm1(mat &X) {
	vec v = vectorise(X);
	vec v2 = abs(v);
	return (double)sum(v2);
}
#endif

#ifdef OPTIMIZE
// X = max(0, U - lambda);
void proj_l1(mat &U, mat &Out, double lambda, bool pos)
{
	// Using reference as an out parameter
	if (pos) {
		Out = arma::max(mat(U.n_rows, U.n_cols, fill::zeros), U - lambda);
	}
	else {
		Out = arma::max(mat(U.n_rows, U.n_cols, fill::zeros), U - lambda) + arma::min(mat(U.n_rows, U.n_cols, fill::zeros), U + lambda);
	}
}
#else
mat proj_l1(mat &U, double lambda, bool pos)
{
	if (lambda<0.0 || lambda>1.0) {
		throw std::out_of_range("Invalid lambda value");
	}
	
	if (pos) {
		mat t = U - lambda;
		mat zeros = mat(t.n_rows, t.n_cols, fill::zeros);
		return arma::max(zeros, t);
	}
	else {
		mat t = U - lambda;
		mat t2 = U + lambda;
		mat zeros = mat(t.n_rows, t.n_cols, fill::zeros);
		mat zeros2 = mat(t.n_rows, t.n_cols, fill::zeros);
		return arma::max(zeros, t) + arma::min(zeros, t2);
	}
}
#endif

// DtD = D'*D;
// DtY = D'*Y;
// function res = grad(X)
// res = DtD*X - DtY;
// end
#ifdef OPTIMIZE
void Gradient(mat &Gradiant, mat &X, mat &DtD, mat &DtY)
{
	// Using reference as an out parameter
	Gradiant = (DtD*X) - DtY;
	return;
}
#else
mat Gradient(mat &X, mat &DtD, mat &DtY)
{
	return (DtD*X) - DtY;
}
#endif

// Compute XtY2 = X'*Y;
#ifdef OPTIMIZE
void CalcXtY(mat &XtY, mat &X, mat &Y)
{
	// Using reference as an out parameter
	XtY = X.t() * Y;
	return;
}
#else
mat CalcXtY(mat &X, mat &Y)
{
	return X.t() * Y;
}
#endif

// fista_lasso.m
// function X = fista_lasso(Y, D, Xinit, opts)
#ifdef OPTIMIZE
void fista_lasso(mat &Out, mat &Y, mat &D, lasso_options &opts)
{
	// Using reference as an out parameter

	//if numel(Xinit) == 0
	//	Xinit = zeros(size(D, 2), size(Y, 2));
	//end
	mat Xinit(D.n_cols, Y.n_cols, fill::zeros);

	mat DtD = mat();
	mat DtY = mat();

	CalcXtY(DtD, D, D);
	CalcXtY(DtY, D, Y);

	cx_double max_eigen;
	//%% Lipschitz constant
	//	L = max(eig(DtD));
	cx_vec eigval;
	cx_mat eigvec;

	eig_gen(eigval, eigvec, DtD);
	max_eigen = eigval.max();

	double L = max_eigen.real(); // TODO: check in Matlab if eigen values are complex
	fista_general(Out, Xinit, DtD, DtY, L, opts);
}
#else
mat fista_lasso(mat &Y, mat &D, lasso_options &opts) {

	mat result = mat();

	//if numel(Xinit) == 0
	//	Xinit = zeros(size(D, 2), size(Y, 2));
	//end
    mat Xinit(D.n_cols, Y.n_cols, fill::zeros);

	mat DtD = CalcXtY(D, D);
	mat DtY = CalcXtY(D, Y);

	cx_double max_eigen;
	//%% Lipschitz constant
	//	L = max(eig(DtD));
	cx_vec eigval;
	cx_mat eigvec;

	eig_gen(eigval, eigvec, DtD);
	max_eigen = eigval.max();

	double L = max_eigen.real(); // TODO: check in Matlab if eigen values are complex
	return fista_general(Xinit, DtD, DtY, L, opts);
}
#endif

// fista_general.m
// function[X, iter, min_cost] = fista_general(grad, proj, Xinit, L, opts, calc_F)
#ifdef OPTIMIZE
void fista_general(mat &Out, mat &Xinit, mat &DtD, mat &DtY, double L, lasso_options &opts) {

	double Linv = 1 / L;
	double lambdaLiv = opts.lambda*Linv;

	mat x_old = Xinit;
	mat y_old = Xinit;
	double t_old = 1;
	double t_new;
	int iter = 0;
	double cost_old = 1e10;

	mat x_new;
	mat y_new;
	mat grad;
	mat tmp;
	mat tmp_grad;

	double e;
	double new_tolerance = opts.tolerance*Xinit.n_elem;

	while (iter < opts.max_iterations) {
		// iter = iter + 1;
		iter++;

		// x_new = feval(proj, y_old - Linv*feval(grad, y_old), opts_proj);
		// FIRST ,compute y_old - Linv*feval(grad, y_old)
		Gradient(tmp_grad, y_old, DtD, DtY);
		grad = y_old - Linv*tmp_grad;
		
		// Then do the projection 
		proj_l1(grad, x_new, lambdaLiv, opts.pos);
		// t_new = 0.5*(1 + sqrt(1 + 4 * t_old ^ 2));
		// t_new = 0.5*(1 + sqrt(1 + 4 * pow(t_old, 2)));
		t_new = 0.5 + sqrt(0.25 +  pow(t_old, 2));
		// y_new = x_new + (t_old - 1) / t_new * (x_new - x_old);
		y_new = x_new + (t_old - 1) / t_new * (x_new - x_old);

		// %% check stop criteria
		//  e = norm1(x_new - x_old) / numel(x_new);
		// if e < opts.tol
		// 		break;
		// end
		tmp = x_new - x_old;
		e = norm1(tmp);// / x_new.n_elem;

		if (e < new_tolerance) {
			break;
		}

		// %% update
		// x_old = x_new;
		// t_old = t_new;
		// y_old = y_new;
		x_old = x_new;
		t_old = t_new;
		y_old = y_new;
	}

	Out = x_new;
	return;
}

#else

mat fista_general(mat &Xinit, mat &DtD, mat &DtY, double L, lasso_options &opts) {

	double Linv = 1 / L;
	double lambdaLiv = opts.lambda*Linv;

	mat x_old = Xinit;
	mat y_old = Xinit;
	double t_old = 1;
	double t_new;
	int iter = 0;
	double cost_old = 1e10;

	mat x_new = mat();
	mat y_new = mat();
	mat grad = mat();
	mat tmp = mat();

	double e;

	while (iter < opts.max_iterations) {
		// iter = iter + 1;
		iter++;

		// x_new = feval(proj, y_old - Linv*feval(grad, y_old), opts_proj);
		// FIRST ,compute y_old - Linv*feval(grad, y_old)
		grad = y_old - Linv*Gradient(y_old, DtD, DtY);
		// Then do the projection 
		x_new = proj_l1(grad, lambdaLiv, opts.pos);
		// t_new = 0.5*(1 + sqrt(1 + 4 * t_old ^ 2));
		t_new = 0.5*(1 + sqrt(1 + 4 * pow(t_old, 2)));

		// y_new = x_new + (t_old - 1) / t_new * (x_new - x_old);
		y_new = x_new + (t_old - 1) / t_new * (x_new - x_old);

		// %% check stop criteria
		//  e = norm1(x_new - x_old) / numel(x_new);
		// if e < opts.tol
		// 		break;
		// end
		tmp = x_new - x_old;
		e = norm1(tmp) / x_new.n_elem;

		if (e < opts.tolerance) {
			break;
		}

		// %% update
		// x_old = x_new;
		// t_old = t_new;
		// y_old = y_new;
		x_old = x_new;
		t_old = t_new;
		y_old = y_new;
	}
	return x_new;
}
#endif
// End of File
