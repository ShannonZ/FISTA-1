// Copyright 2014 Conrad Sanderson (http://conradsanderson.id.au)
// Copyright 2014 National ICT Australia (NICTA)
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ------------------------------------------------------------------------


#include "armaMex.hpp"
#include "fista.hpp"


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	// Check the number of input arguments.
	if (nrhs != 3)
		mexErrMsgTxt("Incorrect number of input arguments.");

	// Check type of input.
	if ((mxGetClassID(prhs[0]) != mxDOUBLE_CLASS))
		mexErrMsgTxt("Input must me of type double.");

	// Check if input is real.
	if ((mxIsComplex(prhs[0])) || (mxIsComplex(prhs[1])))
		mexErrMsgTxt("Input must be real.");

	// Create matrices X and lambda value from the first and second mex argument.
	mat X = armaGetPr(prhs[0]);
	double lambda = armaGetScalar<double>(prhs[1]);
	int pos = armaGetScalar<int>(prhs[2]);

#ifdef OPTIMIZE
	mat *C = new mat();
	if (pos) {
		proj_l1(X, *C, lambda, true);
	}
	else {
		proj_l1(X, *C, lambda, false);
	}

	// Create the output argument plhs[0] to return result
	plhs[0] = armaCreateMxMatrix(C->n_rows, C->n_cols);

	// Return C as plhs[0] in Matlab/Octave
	armaSetPr(plhs[0], *C);
#else
	// Call function under test
	mat C = mat();
	if (pos) {
		C = proj_l1(X, lambda, true);
	}
	else {
		C = proj_l1(X, lambda, false);
	}
	
	// Create the output argument plhs[0] to return result
	plhs[0] = armaCreateMxMatrix(C.n_rows, C.n_cols);

	// Return C as plhs[0] in Matlab/Octave
	armaSetPr(plhs[0], C);
#endif
	return;
}
