%% Testing Lasso
fprintf('Testing C++ Lasso implementation against Matlab\n');
addpath('../matlab_fista/');

tmex = 0;
tmat = 0;
tmex_opt = 0;

for c = 1:1000
    d = randi([100 2000],1,1);
    N = randi([10 200],1,1);
    k = randi([10 200],1,1);
  
    X=randn(d,N);
    D=randn(d,k);
    D=D./repmat(sqrt(sum(D.^2)),[size(D,1) 1]);

    param.lambda = rand(1,1);
    param.pos    = randi([0,1]);
   
    % [alpha path]=mexLasso(X,D,param);
    tic;
    X_mat = fista_lasso(X, D, [], param);
    tmat = tmat + toc;
    
    tic;
    X_mex = mex_fista_lasso(X, D, param.lambda, param.pos);
    tmex = tmex + toc;    
    
    tic
    X_mex = mex_fista_lasso_opt(X, D, param.lambda, param.pos);
    tmex_opt = tmex_opt + toc;   
    
    diff = sum((X_mat(:)-X_mex(:)).^2);
    if diff > 1.0e-20
        warning('Error while testing lasso function ');
    end
end

fprintf('mex-file time: %fs\n',tmex);
fprintf('mex-file opt time: %fs\n',tmex_opt);
fprintf('matlab-file time: %fs\n\n',tmat);
fprintf('ratio = : %f\n\n',(tmex/tmat));

