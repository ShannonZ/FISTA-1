%% Testing Lasso
fprintf('Profiling Matlab Lasso\n');
tmat = 0;

d = 10;
N = 20;
k = 30;

X=randn(d,N);
D=randn(d,k);
D=D./repmat(sqrt(sum(D.^2)),[size(D,1) 1]);

param.lambda = 0.01;
param.pos    = 1;
 
tic;
for c = 1:10000
    % [alpha path]=mexLasso(X,D,param);
    X_mat = fista_lasso(X, D, [], param);
end
tmat = tmat + toc;
fprintf('matlab-file time: %fs\n\n',tmat);
