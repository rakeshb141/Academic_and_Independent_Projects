% This file loads the Project_2_features.mat file which has the variables 
% 1.training_set[N*(D+1)] 2.testing_set [T*D] 3.training_testing[(N+T)*D](USED FOR NN PACKAGE)
% The output of this file is Weight Matrix and then it calls test_nn which
% gives Output which are the classified test features.

load('Proj_2_Data');
[n, D] = size(Data);
Input = Data(:,2:end);
Input = [ones(n,1) Input];
Input_with_target = 513;
No_of_Classes = 10;
Weight = ones(No_of_Classes,Input_with_target);
ETA = 0.03;	
No_of_Iterations = 400;
Output = softmax(Weight*Input')';
for i = 0:1:No_of_Iterations
Wk = Weight - ETA*((Output-training_target_matrix)'*Input);
Output = softmax(Weight*Input')';
Weight = Wk;
end
Error = sum(training_target_matrix-Output);
Output = softmax(Weight*Input')';

test_lr;