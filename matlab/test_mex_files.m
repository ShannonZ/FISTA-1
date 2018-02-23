%% Bit exactness testing against Matlab

%% Test X'*Y
disp("Testing X'*Y")
X=randn(64,200)';
Y=randn(200,20000);

tic
XtY=mexCalcXtY(X,Y);
t=toc;
fprintf('mex-file time: %fs\n',t);

tic
XtY2=X'*Y;
t=toc;
fprintf('matlab-file time: %fs\n',t);

diff = sum((XtY(:)-XtY2(:)).^2);
if diff > 1.0e-10
    warning('Error while testing Xt*Y');
end

%% Test norm1
disp("Testing norm1(X)")
X=randn(64,200)';

tic
res=mexnorm1(X);
t=toc;
fprintf('mex-file time: %fs\n',t);

tic
res2 = full(sum(abs(X(:))));
t=toc;
fprintf('matlab-file time: %fs\n',t);

diff = abs(res2 - res);
if diff > 1.0e-10
    warning('Error while testing norm1(X)');
end

%% Test Gradiant
disp("Testing Gradiant")

d      = 10; 	% data dimension
N      = 20; 	% number of samples 
k      = 30; 	% dictionary size 
lambda = 0.01;
Y      = normc(rand(d, N));
D      = normc(rand(d, k));

X=randn(k,N);

DtD = D'*D;
DtY = D'*Y;

tic
mexGradX=mexGradiant(X,DtD,DtY);
t=toc;
fprintf('matlab-file time: %fs\n',t);

tic
GradX=grad(X,DtD,DtY);
t=toc;
fprintf('mex-file time: %fs\n',t);

diff = sum((GradX(:)-mexGradX(:)).^2);
if diff > 1.0e-10
    warning('Error while testing Gradiant');
end

%% Matlab functions
%% to be moved in a separate file
function res = grad(X,DtD,DtY) 
    res = DtD*X - DtY;
end