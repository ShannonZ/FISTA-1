%% Testing X'*Y
disp("Testing X'*Y")

tmex = 0;
tmat = 0;
for c = 1:1000
    %
    % generate random input
    %
    d = randi([100 2000],1,1);
    N = randi([10 200],1,1);
    k = randi([10 200],1,1);
  
    X=randn(d,N);
    Y=randn(d,k);

    % test C++ implementation via mex file
    tic
    XtY = mexCalcXtY(X,Y);
    tmex = tmex + toc;

    % Matlab version
    tic
    XtY2=X'*Y;
    tmat=tmat+toc;
    
    % bit-exactness correctness
    diff = sum((XtY(:)-XtY2(:)).^2);
    if diff > 1.0e-10
        warning('Error while testing Xt*Y');
    end
end
fprintf('mex-file time: %fs\n',tmex);
fprintf('matlab-file time: %fs\n\n',tmat);