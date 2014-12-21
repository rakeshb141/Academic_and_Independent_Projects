%this file is called from train_nn.m
Output = [];
[Row, Col] = size(testing_set);
Data =[ones(Row,1) testing_set];
X = Data;
   for j = 1: 1 : Row
       X_T = X(j,:);
       Y_T = softmax(Weight_2 * logsig(Weight_1*X_T'));
       [N ID] = max(Y_T(:));
       [x y] = ind2sub(size(Y_T),ID);
       Output = vertcat(Output, x-1);
   end
%end