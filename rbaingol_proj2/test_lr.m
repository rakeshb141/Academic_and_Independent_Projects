%this file is run from train_lr file
Output = [];
[R, C] = size(testing_set);
data =[ones(R,1) testing_set];
for j = 1: 1 : R
   X = data(j,:);
   Y = softmax(Weight*X');
   [NUM ID] = max(Y(:));
   [x y] = ind2sub(size(Y),ID);
   Output = vertcat(Output, x-1);
end