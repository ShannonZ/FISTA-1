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
	if (nrhs != 1)
		mexErrMsgTxt("Incorrect number of input arguments.");

	// Check type of input.
	if ((mxGetClassID(prhs[0]) != mxDOUBLE_CLASS))
		mexErrMsgTxt("Input must me of type double.");

	// Check if input is real.
	if ((mxIsComplex(prhs[0])) || (mxIsComplex(prhs[1])))
		mexErrMsgTxt("Input must be real.");

	// Create matrices X and Y from the first and second argument.
	mat X = armaGetPr(prhs[0]);
	
	// Call c++ Fista library 
	double result = norm1(X);
	
	// Create the output argument plhs[0] to return result
	plhs[0] = armaCreateMxMatrix(1, 1);

	// Return C as plhs[0] in Matlab/Octave
	double *dst_pointer = (double*)mxGetPr(plhs[0]);
	*dst_pointer = result;
	//const Type *src_pointer = (Type*)armaMatrix.memptr();

	// std::memcpy(dst_pointer, src_pointer, sizeof(Type)*armaMatrix.n_elem);

	// armaSetPr(plhs[0], result);

	return;
}
