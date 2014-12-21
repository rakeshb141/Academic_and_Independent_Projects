clc;
clear;

load('project1_data.mat');
.............................................................................
%CFS    
train_cfs(initial_data,to_check_with);

predicted = test_cfs( initial_data,to_check_with );

e_d = 0;
size_initial_data = size(initial_data);
percent_80_data = round(0.8 * size_initial_data(1,1));
percent_10_data = ceil(0.1 * size_initial_data(1,1));
percent_90_data = round(0.9 * size_initial_data(1,1));
for row = 1:percent_10_data
    e_d = e_d + (to_check_with(62660+row) - predicted(row))^2;
end
e_d = e_d/2;

e_rms_cfs = sqrt(2 * e_d / percent_10_data);
...............................................................................
%GD

train_gd(initial_data,to_check_with);
e_d_=0
predicted = test_gd( initial_data,to_check_with );

for row = 1:percent_10_data
    e_d_ = e_d_ + (to_check_with(62660+row) - predicted(row))^2;
end
e_d_ = e_d_/2;

e_rms_gd = sqrt(2 * e_d_ / percent_10_data);

...........................................................................
%OUTPUT
fprintf('My ubit name is %s\n', 'rbaingol' );
fprintf('My student number is %d \n', 50097576);
fprintf('the model complexity M_cfs is %d\n', 2);
fprintf('the model complexity M_gd is %d\n', 16);
fprintf('the regularization parameters lambda_cfs is %4.2f\n', 2);
fprintf('the regularization parameters lambda_gd is %4.2f\n', 0.06);
fprintf('the root mean square error for the closed form solution is %4.2f\n', e_rms_cfs);
fprintf('the root mean square error for the gradient descent method is %4.2f\n', e_rms_gd);
