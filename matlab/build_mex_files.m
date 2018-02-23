%%
% Building all mex files with the -g option, in order to be able to debug them
mex -g -I..\src -I../externals/armadillo -l..\solutions\x64\Debug\fista_lib.lib -l../externals/blas_lapack_lib_win64/blas_win64_MT.lib  -l../externals/blas_lapack_lib_win64/lapack_win64_MT.lib mexCalcXtY.cpp
mex -g -I..\src -I../externals/armadillo -l..\solutions\x64\Debug\fista_lib.lib -l../externals/blas_lapack_lib_win64/blas_win64_MT.lib  -l../externals/blas_lapack_lib_win64/lapack_win64_MT.lib mexnorm1.cpp
mex -g -I..\src -I../externals/armadillo -l..\solutions\x64\Debug\fista_lib.lib -l../externals/blas_lapack_lib_win64/blas_win64_MT.lib  -l../externals/blas_lapack_lib_win64/lapack_win64_MT.lib mexGradiant.cpp
mex -g -I..\src -I../externals/armadillo -l..\solutions\x64\Debug\fista_lib.lib -l../externals/blas_lapack_lib_win64/blas_win64_MT.lib  -l../externals/blas_lapack_lib_win64/lapack_win64_MT.lib mex_projl1.cpp


