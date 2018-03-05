clear all;
addpath('../matlab_fista/');
randn('seed',0);
fprintf('Testing C++ Lasso implementation against Matlab\n');

% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % Decomposition of a large number of signals
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Data are generated
% X=randn(100,100000);
% X=X./repmat(sqrt(sum(X.^2)),[size(X,1) 1]);
% D=randn(100,200);
% D=D./repmat(sqrt(sum(D.^2)),[size(D,1) 1]);
% 
% % parameter of the optimization procedure are chosen
% %param.L=20; % not more than 20 non-zeros coefficients (default: min(size(D,1),size(D,2)))
% param.lambda=0.15; % not more than 20 non-zeros coefficients
% param.numThreads=-1; % number of processors/cores to use; the default choice is -1
%                      % and uses all the cores of the machine
% param.mode=2;        % penalized formulation
% 
% tic
% alpha=mexLasso(X,D,param);
% t=toc
% fprintf('%f signals processed per second\n',size(X,2)/t);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Regularization path of a single signal 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

tmex = 0;
tmat = 0;
for c = 1:10000
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
    
    diff = sum((X_mat(:)-X_mex(:)).^2);
    if diff > 1.0e-25
        warning('Error while testing lasso function ');
    end
end

fprintf('mex-file time: %fs\n',tmex);
fprintf('matlab-file time: %fs\n\n',tmat);

