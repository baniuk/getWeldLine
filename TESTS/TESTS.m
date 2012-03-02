%% kasowanie wszystkich plików out
%% przygotowanie TestCase_1 GaussLin_test_1
pa = '..\x64\debug\';
TestCase_1.name = 'GaussLin_test_1';
TestCase_1.indataname = 'GaussLin_test_1.out';
TestCase_1.outdataname = 'GaussLin_test_1.dat';
TestCase_1.outdataname2 = 'GaussLin_test_1_x.dat';
a = 10;
b = 1000;
c = 100;
d = 0;
e = 0;
x = 1:2000;
data_in = [a b c d e];
TestCase_1.data_in = data_in;
TestCase_1.expected = gausslinear(x,data_in(1),data_in(2),data_in(3),data_in(4),data_in(5));
savebinarymatrix(data_in,[pa,TestCase_1.outdataname]);
savebinarymatrix(x,[pa,TestCase_1.outdataname2]);
%% przygotowanie TestCase_2 getApproxLine
pa = '..\x64\debug\';
TestCase_2.name = 'getApproxLine_test_1';
TestCase_2.indataname = 'getApproxLine_test_1.out'; % ten z progarmy
TestCase_2.outdataname = 'getApproxLine_test_1.dat'; % ten z matlaba
im1 = double(imread('300_09.tif'));
im1r = RemoveFrame(im1); 
data_in = im1r(:,4814)';
TestCase_2.data_in = data_in;
TestCase_2.expected = data_in;
savebinarymatrix(TestCase_2.data_in,[pa TestCase_2.outdataname]);
%% przygotowanie TestCase_3 jGaussLin
pa = '..\x64\debug\';
TestCase_3.name = 'jGaussLin_test_1';
TestCase_3.indataname = 'jGaussLin_test_1.out'; % ten z progarmy
TestCase_3.outdataname = 'jGaussLin_test_1.dat'; % ten z matlaba
TestCase_3.outdataname2 = 'jGaussLin_test_1_x.dat';
syms a b c d e1 e
f = 'a*e^(-((x-b)/c)^2)+d*x+e1';
v = [a,b,c,d,e1];
R = jacobian(f, v);
a = 1;b = 1;c = 1; d = 1; e1 = 1; x = 1;e = exp(1);
TestCase_3.data_in = [a b c d e1];
TestCase_3.expected = eval(R);
savebinarymatrix(TestCase_3.data_in,[pa TestCase_3.outdataname]);
savebinarymatrix(x,[pa TestCase_3.outdataname2]);
%% przygotowanie TestCase_3 jGaussLin
pa = '..\x64\debug\';
TestCase_32.name = 'jGaussLin_test_2';
TestCase_32.indataname = 'jGaussLin_test_2.out'; % ten z progarmy
TestCase_32.outdataname = 'jGaussLin_test_2.dat'; % ten z matlaba
TestCase_32.outdataname2 = 'jGaussLin_test_2_x.dat';
syms a b c d e1 e
f = 'a*e^(-((x-b)/c)^2)+d*x+e1';
v = [a,b,c,d,e1];
R = jacobian(f, v);
a = 10;b = 1;c = 0.25; d = -1.7; e1 = 7; x = 100;e = exp(1);
TestCase_32.data_in = [a b c d e1];
TestCase_32.expected = eval(R);
savebinarymatrix(TestCase_32.data_in,[pa TestCase_32.outdataname]);
savebinarymatrix(x,[pa TestCase_32.outdataname2]);
%% ----------------------- RUN PROGRAM ------------------------------------ 
%% run TestCase_1 GaussLin_test_1
ImportDumpFile([pa,TestCase_1.indataname]);
if rmse(data,TestCase_1.expected)<1e-12
    fprintf('%s ---> PASSED, RMS = %d\n',TestCase_1.name,rmse(data,TestCase_1.expected));
else
    fprintf('%s ---> FAILED\n',TestCase_1.name);
    figure;plot(data);hold on; plot(TestCase_1.expected,'-r');title(TestCase_1.name)
end
%% run TestCase_2 getApproxLine
ImportDumpFile([pa TestCase_2.indataname]);
if isequal(TestCase_2.expected(:),data(:))
    fprintf('%s ---> PASSED\n',TestCase_2.name);
else
    fprintf('%s ---> FAILED\n',TestCase_2.name);
end
%% run TestCase_3 jGaussLin
ImportDumpFile([pa TestCase_3.indataname]);
if isequal(TestCase_3.expected(:),data(:))
    fprintf('%s ---> PASSED\n',TestCase_3.name);
else
    fprintf('%s ---> FAILED\n',TestCase_3.name);
end
ImportDumpFile([pa TestCase_32.indataname]);
if isequal(TestCase_32.expected(:),data(:))
    fprintf('%s ---> PASSED\n',TestCase_32.name);
else
    fprintf('%s ---> FAILED\n',TestCase_32.name);
end