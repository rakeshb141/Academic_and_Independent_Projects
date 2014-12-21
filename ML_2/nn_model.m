% This file loads the Project_2_features.mat file which has the variables 
% 1.training_set[N*(D+1)] 2.testing_set [T*D] 3.training_testing[(N+T)*D](USED FOR NN PACKAGE)
% The output of this file is Weight Matrix and then it calls test_nn which
% gives Output which are the classified test features.


load('Proj_2_Data');
[R, C] = size(Data);
X_input = Data(:,2:end);
Dimensions = X_input';
Targets = training_target_matrix';

% Create a Pattern Recognition Network
hiddenLayerSize = 20;
net = patternnet(hiddenLayerSize);

% Choose Input and Output Pre/Post-Processing Functions
% For a list of all processing functions type: help nnprocess
net.inputs{1}.processFcns = {'removeconstantrows','mapminmax'};
net.outputs{2}.processFcns = {'removeconstantrows','mapminmax'};


% Setup Division of Data for Training, Validation, Testing
% For a list of all data division functions type: help nndivide
net.divideFcn = 'dividerand';  % Divide data randomly
net.divideMode = 'sample';  % Divide up every sample
net.divideParam.trainRatio = 55/100;
net.divideParam.valRatio = 35/100;
net.divideParam.testRatio = 10/100;

% For help on training function 'trainscg' type: help trainscg
% For a list of all training functions type: help nntrain
net.trainFcn = 'trainscg';  % Scaled conjugate gradient

% Choose a Performance Function
% For a list of all performance functions type: help nnperformance
net.performFcn = 'mse';  % Mean squared error

% Choose Plot Functions
% For a list of all plot functions type: help nnplot
net.plotFcns = {'plotperform','plottrainstate','ploterrhist', ...
  'plotregression', 'plotfit'};


% Train the Network
[net,tr] = train(net,Dimensions,Targets);

% Test the Network
outputs = net(Dimensions);
errors = gsubtract(Targets,outputs);
performance = perform(net,Targets,outputs)

% Recalculate Training, Validation and Test Performance
trainTargets = Targets .* tr.trainMask{1};
valTargets = Targets  .* tr.valMask{1};
testTargets = Targets  .* tr.testMask{1};
trainPerformance = perform(net,trainTargets,outputs)
valPerformance = perform(net,valTargets,outputs)
testPerformance = perform(net,testTargets,outputs)

% View the Network
view(net)
%end
% Plots
% Uncomment these lines to enable various plots.
figure, plotperform(tr)
figure, plottrainstate(tr)
figure, plotconfusion(targets,outputs)
figure, plotroc(targets,outputs)
figure, ploterrhist(errors)
