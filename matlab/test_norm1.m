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