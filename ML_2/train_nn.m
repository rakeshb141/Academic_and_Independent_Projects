% This file loads the Project_2_features.mat file which has the variables 
% 1.training_set[N*(D+1)] 2.testing_set [T*D] 3.training_testing[(N+T)*D](USED FOR NN PACKAGE)
% The output of this file is Weight Matrix and then it calls test_nn which
% gives Output which are the classified test features.


load('Proj_2_Data');
[R, C] = size(Data);
Input = Data(:,2:end);
train_rate = 0.03;
Input = [ones(R,1) Input];
Input_with_target = 513;
No_of_classes = 10; 
No_of_hidden_nodes = 80;
Weight_1 = rand(No_of_hidden_nodes,Input_with_target)*2;
Weight_2 = rand(No_of_classes,No_of_hidden_nodes)*2;
Weight_1 = ones(No_of_hidden_nodes,Input_with_target) - Weight_1;
Weight_2 = ones(No_of_classes,No_of_hidden_nodes) - Weight_2;
%y = softmax(w*x')';
for j = 1 : 1 : R
    i = round(rand(1)*R);
    if i==0
        i = i+1;
    end
    A_J = Weight_1*Input(i,:)';
    Z_J = logsig(A_J);
    A_K = Weight_2*Z_J;
    Y_K = softmax(A_K);
    delta_K = Y_K' - training_target_matrix(i,:);
    gradient_2 = delta_K' * Z_J';
    delta_J = (Z_J .* (ones(No_of_hidden_nodes,1) - Z_J)) .* (Weight_2'*delta_K');
    gradient_1 = delta_J*Input(i,:);
    Weight_1 = Weight_1 - train_rate*gradient_1;
    Weight_2 = Weight_2 - train_rate*gradient_2;
end

test_nn;