%% Test Gradiant
disp("Testing Gradiant")

tmex = 0;
tmat = 0;

for c = 1:1000
    
    %
    % generate random input
    %
    d = randi([100 2000],1,1); % data dimension
    N = randi([10 200],1,1); % number of samples 
    k = randi([10 200],1,1); % dictionary size 
    
    Y = normc(rand(d, N));
    D = normc(rand(d, k));
    X = randn(k,N);

    DtD = D'*D;
    DtY = D'*Y;
    
    % test C++ implementation via mex file
    tic
    mexGradX = mexGradiant(X,DtD,DtY);
    tmex = tmex + toc;
    
    % test Matlab implementation
    tic
    GradX = grad(X,DtD,DtY);
    tmat=tmat+toc;

    % bit-exactness correctness
    diff = sum((GradX(:)-mexGradX(:)).^2);
    if diff > 1.0e-20
        warning('Error while testing Gradiant');
    end
end
fprintf('mex-file time: %fs\n',tmex);
fprintf('matlab-file time: %fs\n',tmat);
fprintf('ratio = : %f\n\n',(tmex/tmat));

function res = grad(X,DtD,DtY) 
    res = DtD*X - DtY;
end