
function [] = train_cfs(initial_data,to_check_with)
size_initial_data = size(initial_data);
percent_80_data = round(0.8 * size_initial_data(1,1));
train_set = initial_data(1:percent_80_data,1:end);
size(train_set);
u_matrix = zeros(1,46);
%phi_matrix = zeros(1,46);




limit = 2;
s = 3 ;
phi_matrix = zeros(percent_80_data,limit);
den = s*s*eye(46,46);


inverse_=inv(den);
....................
for j=1:limit
sample = datasample(train_set,20000);%change to 20000 later
u = mean(sample);
    for row=1:percent_80_data
        x = train_set(row:row,1:end);
        sub = x-u;
        square = (-1/2)*sub*inverse_*transpose(sub);
        phi = exp(square);
        phi_matrix(row,j)= phi;
        
    end  
u_matrix = vertcat(u_matrix,u);
end
%phi_matrix
size(u_matrix);
u_matrix
...........................................
   
save mu_cfs.mat u_matrix
save s_cfs.mat den

.............
transpose_phi = transpose(phi_matrix);
lambda = 2;
prod = transpose_phi*phi_matrix;
add = lambda*eye(limit,limit)+ prod;
inv_add = inv(add);
%size (inv_add)
%size (transpose_phi)
to_check_with = to_check_with(1:percent_80_data,1:end);
%size (to_check_with)
quad_reg_ext = inv_add*transpose_phi*to_check_with

save W_cfs.mat quad_reg_ext
.................

transpose_wml = transpose(quad_reg_ext);
size(quad_reg_ext)
size(transpose_wml)
size(phi_matrix(1:1,:))
e_d_w=0;

for row_=1:percent_80_data
sub = [(to_check_with(row_:row_,:))-(phi_matrix(row_:row_,:)*quad_reg_ext)]^2;
e_d_w = e_d_w+sub;

%size(second)
%size(to_check_with(1:1,:))
end
e_d_w = e_d_w/2

..................
q = 2;
e_w_w = 0;

for d=1:limit
    e_w_w = e_w_w + (abs(quad_reg_ext(d:d,:)))^2;
end

e_w_w = e_w_w/2
.........................
e_w = e_d_w+(lambda*e_w_w)
%2*e_w
%percent_80_data
%phi_matrix
e_rms = sqrt(2*e_w/percent_80_data)
