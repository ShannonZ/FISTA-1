%% Bit exactness testing against Matlab

clear all;

% Making sure needed dlls are present next to mex files
copyfile ../externals/blas_lapack_lib_win64/blas_win64_MT.dll .
copyfile ../externals/blas_lapack_lib_win64/lapack_win64_MT.dll .

addpath('../matlab_fista/')

test_XtY();
test_norm1();
test_proj_l1();
test_gradiant();


