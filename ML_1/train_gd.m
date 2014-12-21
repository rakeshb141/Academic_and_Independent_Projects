function [] = train_gd(initial_data,to_check_with)
size_initial_data = size(initial_data);
percent_80_data = round(0.8 * size_initial_data(1,1));
train_set = initial_data(1:percent_80_data,1:end);
%size(train_set);
u_matrix = zeros(1,46);
%phi_matrix = zeros(1,46);


limit = 16;
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
%display(phi_matrix);
save mu_gd.mat u_matrix
save s_gd.mat den
.............
transpose_phi = transpose(phi_matrix);
lambda = 2;
prod = transpose_phi*phi_matrix;
add = lambda*eye(1,1)+ prod;
inv_add = inv(add);
%size (inv_add)
%size (transpose_phi)
to_check_with = to_check_with(1:percent_80_data,1:end);
%size (to_check_with)

%wml = inv_add*transpose_phi*to_check_with
wml = zeros(limit,1);
learning_rate = 0.06;
no_of_iterations = 5;
e_rms = 10;
for i=1:no_of_iterations
    for row=1:percent_80_data
   
        prod = phi_matrix(row,:)*wml;
%         prod
%         wml
%         if(isnan(wml) == 1)
%             break;
%         end
        
        c = to_check_with(row);
        sub = to_check_with(row) - prod; 
        t = size(to_check_with(row));
        r =size(phi_matrix(row,:));
        a= size(sub);
        p = phi_matrix(row,:);
        
        second = learning_rate * ( sub * phi_matrix(row,:));
%        if(isinf(sub) == 1 | isinf(second) == 1)
%             sub
%        end
       e = size(transpose(second));
        %wml
        %second
        trans_second = transpose(second);
        wml = wml + trans_second;
        %wml
        %second
        
    end

%break;

wml;
e_d_w=0;

for row_=1:percent_80_data
sub = [(to_check_with(row_:row_,:))-(phi_matrix(row_:row_,:)*wml)]^2;
e_d_w = e_d_w+sub;

%size(second)
%size(to_check_with(1:1,:))
end
e_d_w = e_d_w/2;

..................
q = 2;
e_w_w = 0;

for d=1:limit
    e_w_w = e_w_w + (abs(wml(d:d,:)))^2;
end

e_w_w = e_w_w/2;
.........................
e_w = e_d_w+(lambda*e_w_w);
%2*e_w
%percent_80_data
%phi_matrix
temp = sqrt(2*e_w/percent_80_data);

i
if(temp < e_rms)
    e_rms = temp
else
    learning_rate = learning_rate/2;
end


    


end
save W_gd.mat wml