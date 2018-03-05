%% Bit exactness testing against Matlab

clear all;

% Making sure needed dlls are present next to mex files
copyfile ../externals/blas_lapack_lib_win64/blas_win64_MT.dll .
copyfile ../externals/blas_lapack_lib_win64/lapack_win64_MT.dll .

addpath('../matlab_fista/')

%% Test X'*Y
disp("Testing X'*Y")

tmex = 0;
tmat = 0;
for c = 1:10
    X=randn(64,200)';
    Y=randn(200,20000);

    tic
    XtY=mexCalcXtY(X,Y);
    tmex = tmex + toc;

    tic
    XtY2=X'*Y;
    tmat=tmat+toc;

    diff = sum((XtY(:)-XtY2(:)).^2);
    if diff > 1.0e-10
        warning('Error while testing Xt*Y');
    end
end
fprintf('mex-file time: %fs\n',tmex);
fprintf('matlab-file time: %fs\n\n',tmat);


%% Test norm1
disp("Testing norm1(X)")
tmex = 0;
tmat = 0;
for c = 1:10
    X=randn(64,200)';
    tic
    res=mexnorm1(X);
    tmex = tmex + toc;

    tic
    res2 = norm1(X);
    tmat=tmat+toc;
    
    diff = abs(res2 - res);
    if diff > 1.0e-9
        warning('Error while testing norm1(X)');
    end
end
fprintf('mex-file time: %fs\n',tmex);
fprintf('matlab-file time: %fs\n\n',tmat);

%% Test proj_l1
disp("Testing proj_l1(X)")
tmex = 0;
tmat = 0;
for c = 1:10
    
    % Generating random input
    lambda = rand(1,1);
    X=randn(64,200);
    pos = randi([0,1]);
    
    tic
    res = mex_projl1(X,lambda, pos);
    tmex = tmex + toc;
   
    opts.lambda = lambda;
    opts.pos = pos;
    tic;
    res2 = proj_l1(X,opts);
    tmat = tmat + toc;
    
    % diff = abs(res2 - res);
    diff = sum((res2(:)- res(:)).^2);
    if diff > 1.0e-20
        warning('Error while testing norm1(X)');
    end
end
fprintf('mex-file time: %fs\n',tmex);
fprintf('matlab-file time: %fs\n\n',tmat);

%% Test Gradiant
disp("Testing Gradiant")

d      = 10; 	% data dimension
N      = 20; 	% number of samples 
k      = 30; 	% dictionary size 
lambda = 0.01;

tmex = 0;
tmat = 0;

for c = 1:10
    Y = normc(rand(d, N));
    D = normc(rand(d, k));
    X = randn(k,N);

    DtD = D'*D;
    DtY = D'*Y;

    tic
    mexGradX=mexGradiant(X,DtD,DtY);
    tmex = tmex + toc;

    tic
    GradX=grad(X,DtD,DtY);
    tmat=tmat+toc;

    diff = sum((GradX(:)-mexGradX(:)).^2);
    if diff > 1.0e-10
        warning('Error while testing Gradiant');
    end
end
fprintf('mex-file time: %fs\n',tmex);
fprintf('matlab-file time: %fs\n\n',tmat);

%% Matlab functions
%% to be moved in a separate file
function res = grad(X,DtD,DtY) 
    res = DtD*X - DtY;
end

