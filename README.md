# FISTA
This repository contains C++ implementation code of the Fast Iterative Shrinkage/Thresholding Algorithm.

## Considerations
- The software builds for Windows, with Microsoft Visual Studio Community 2017 as a build platform. 
- library developped in C++
- it is a constraint time development exercise
- as the FISTA algorithm operates on Matrixes, I used [Armadillo](http://arma.sourceforge.net/) as a library of choice for all matrix operations
- [Armadillo](http://arma.sourceforge.net/) needs the [LAPACK and the BLAS libraries](http://www.netlib.org/lapack/lug/node11.html). I only found **a release** versions of those libraries for Windows. (this is important for later, a debug library would have been of good use).

## Directory content
- externals: artifacts needed for my code (Armadillo and Lapack and Blas)
- solutions: microsoft visual studio solution and 2 project files:
    1. fista_lib.vcxproj to build the algorithm  (*.lib)
    1. mve.vcxproj to build the fistal algo test app (*.exe)
- src: the Fista algorithm code

## Code considerations
As this is a direct implementation of some Matlab code, I kept a lot of things from Matlab
- all variables names are kept
- overall algo structure and function names are kept
- most of the comments in C++ contains the Matlab code. As such, the c++ code below a comment is the c++ implementation of the commented Matlab code
- the Matlab code is not oo oriented, neither is mine. This point should probably be addressed in a refactoring effort
- the Matlab code uses function handle to pass functions to the algorithm (gradient, project and cost). In my implementation, those functions are hardcoded (they are in the same file). Function pointers should be used, or may be a more elaborate construct could be defined in a refactoring effort.

## Testing
For the moment, what is built is a C++ implemantation mimicking the Matlab code. The C++ code was built progressively doing one function at a time using the test_all() function in the test application.
- test_CalcXtY();
- test_Gradient();
- test_proj_l1();
- test_norm1();
- test_lasso();

The code build and runs, but probably is not bit-exact with Matlab. As I say, if something is not tested properly, it probably does not work. Testing my algorithm against the Matlab reference still needs to be done.  

## What to do next
### Testing against the Matlab reference
This is a tricky subject, but there is actually everything in place in the original FISTA repository: https://github.com/tiepvupsu/FISTA under the spams/test_release directory.  
The development strategy for the spams toolbox is a divide and conquer approach. For each FISTA algorithm elementary function to implement in C++, a matlab test implements the function in Matlab, and compare the result against the C++ implementation called via a mex file.  
As more and more "elementary" functions are tested, intermediate and more complex functions can be assembled, and tested against Matlab code.

Debugging becomes easy as it is possible to step inside the mex code and see what is going on with the Visual Studio debugger. (Attach to process, Matlab). Code needs to be built in debug, I was missing the LAPACK and the BLAS debug libraries.



