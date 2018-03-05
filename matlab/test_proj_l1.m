%% Test proj_l1
disp("Testing proj_l1(X)")
tmex = 0;
tmat = 0;
for c = 1:1000
    
    % Generating random input    
    d = randi([100 2000],1,1);
    N = randi([10 200],1,1);
    X=randn(d,N);

    lambda = rand(1,1);
    pos = randi([0,1]);
    
    % test C++ implementation via mex file  
    tic
    res = mex_projl1(X, lambda, pos);
    tmex = tmex + toc;
   
    % call Matlab implementation
    opts.lambda = lambda;
    opts.pos = pos;
    tic;
    res2 = proj_l1(X,opts);
    tmat = tmat + toc;
    
    % bit-exactness correctness
    diff = sum((res2(:)- res(:)).^2);
    if diff > 1.0e-20
        warning('Error while testing norm1(X)');
    end
end
fprintf('mex-file time: %fs\n',tmex);
fprintf('matlab-file time: %fs\n',tmat);
fprintf('ratio = : %f\n\n',(tmex/tmat));
