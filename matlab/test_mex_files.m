%% Bit exactness testing against Matlab

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
    res2 = full(sum(abs(X(:))));
    tmat=tmat+toc;
    
    diff = abs(res2 - res);
    if diff > 1.0e-10
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
    tmex = tmex + toc;;

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