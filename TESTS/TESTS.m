%% kasowanie wszystkich plików out
% nalezy uruchmic plik test.m, póŸnej testy w vcc i jeszcze raz m, bo
% niektóre testy s¹ sprawdzane w matlabie
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
%% przygotowanie TestCase_32 jGaussLin
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
%% przygotowanie TestCase_33 jGaussLin
pa = '..\x64\debug\';
TestCase_33.name = 'jGaussLin_test_3';
TestCase_33.indataname = 'jGaussLin_test_3.out'; % ten z progarmy
TestCase_33.outdataname{1} = 'jGaussLin_test_3.dat'; % ten z matlaba
TestCase_33.outdataname{2} = 'jGaussLin_test_3_x.dat';
syms a b c d e1 e
f = 'a*e^(-((x-b)/c)^2)+d*x+e1';
v = [a,b,c,d,e1];
R = jacobian(f, v);
a = 65535/2;b =  length(data_in)/2 ;c = 150; d = 1; e1 = 7; x = 1:1825;e = exp(1);
TestCase_33.data_in{1} = [a b c d e1];
TestCase_33.data_in{2} = x; 
for aa=1:length(TestCase_33.data_in{2})
    x = TestCase_33.data_in{2}(aa);
    TestCase_33.expected(aa,:) = eval(R);
end
TestCase_33.expected = reshape(TestCase_33.expected,[],5);
savebinarymatrix(TestCase_33.data_in{1},[pa TestCase_33.outdataname{1}]);
savebinarymatrix(TestCase_33.data_in{2},[pa TestCase_33.outdataname{2}]);
%% przygotowanie TestCase_41 getLineApproxGaussLin
pa = '..\x64\debug\';
TestCase_41.name = 'getLineApproxGaussLin_test_1';
TestCase_41.indataname = 'getLineApproxGaussLin_test_1.out';
TestCase_41.outdataname{1} = 'getLineApproxGaussLin_test_1.dat';
TestCase_41.outdataname{2} = 'getLineApproxGaussLin_test_1_x.dat';
TestCase_41.outdataname{3} = 'getLineApproxGaussLin_test_1_p.dat';
TestCase_41.outdataname{4} = 'getLineApproxGaussLin_test_1_lb.dat';
TestCase_41.outdataname{5} = 'getLineApproxGaussLin_test_1_ub.dat';
im1 = double(imread('300_09.tif'));
im1r = RemoveFrame(im1); 
data_in = im1r(:,4814)';
x = 1:length(data_in);
TestCase_41.data_in{1} = data_in;
TestCase_41.data_in{2} = x;
TestCase_41.data_in{3} = [1060 length(data_in)/2 160 0 0];
TestCase_41.data_in{4} = [0 length(data_in)/2-length(data_in)/4 50 -1 -20000];
TestCase_41.data_in{5} = [65535 length(data_in)/2+length(data_in)/4 600 1 20000];

TestCase_41.expected{1} = data_in;

for aa=1:length(TestCase_41.data_in)
    savebinarymatrix(TestCase_41.data_in{aa},[pa,TestCase_41.outdataname{aa}]);
end
%% przygotowanie TestCase_51 getLineApproxGaussLinWeighted
pa = '..\x64\debug\';
TestCase_51.name = 'getLineApproxGaussLinWeighted_test_1';
TestCase_51.indataname = 'getLineApproxGaussLinWeighted_test_1.out';
TestCase_51.outdataname{1} = 'getLineApproxGaussLinWeighted_test_1.dat';
TestCase_51.outdataname{2} = 'getLineApproxGaussLinWeighted_test_1_x.dat';
TestCase_51.outdataname{3} = 'getLineApproxGaussLinWeighted_test_1_p.dat';
TestCase_51.outdataname{4} = 'getLineApproxGaussLinWeighted_test_1_lb.dat';
TestCase_51.outdataname{5} = 'getLineApproxGaussLinWeighted_test_1_ub.dat';
im1 = double(imread('300_09.tif'));
im1r = RemoveFrame(im1); 
data_in = im1r(:,4814)';
x = 1:length(data_in);
TestCase_51.data_in{1} = data_in;
TestCase_51.data_in{2} = x;
TestCase_51.data_in{3} = [1060 length(data_in)/2 160 0 0];
TestCase_51.data_in{4} = [0 length(data_in)/2-length(data_in)/4 50 -1 -20000];
TestCase_51.data_in{5} = [65535 length(data_in)/2+length(data_in)/4 600 1 20000];

TestCase_51.expected{1} = data_in;

for aa=1:length(TestCase_51.data_in)
    savebinarymatrix(TestCase_51.data_in{aa},[pa,TestCase_51.outdataname{aa}]);
end
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
ImportDumpFile([pa TestCase_33.indataname]);
if rmse(TestCase_33.expected(:),data(:))<1e-12
    fprintf('%s ---> PASSED\n',TestCase_33.name);
else
    fprintf('%s ---> FAILED\n',TestCase_33.name);
end
%% run TestCase_41 getLineApproxGaussLin
ImportDumpFile([pa TestCase_41.indataname]);
if isequal(TestCase_41.expected{1}(:),profil(:))
    fprintf('%s ---> PASSED\n',TestCase_41.name);
else
    fprintf('%s ---> FAILED\n',TestCase_41.name);
end
fprintf('Iterations %d\n',info(6));
fprintf('Reason %d\n',info(7));
plot(TestCase_41.data_in{2},TestCase_41.expected{1})
hold on
plot(TestCase_41.data_in{2},gausslinear(TestCase_41.data_in{2},...
p(1),p(2),p(3),p(4),p(5)),'r','linewidth',2)
% plot(TestCase_41.data_in{2},gausslinear(TestCase_41.data_in{2},TestCase_41.data_in{3}(1),TestCase_41.data_in{3}(2),TestCase_41.data_in{3}(3),TestCase_41.data_in{3}(4),TestCase_41.data_in{3}(5)),'g')
%% run TestCase_51 getLineApproxGaussLinWeighted
ImportDumpFile([pa TestCase_51.indataname]);
if isequal(TestCase_51.expected{1}(:),profil(:))
    fprintf('%s ---> PASSED\n',TestCase_51.name);
else
    fprintf('%s ---> FAILED\n',TestCase_51.name);
end
fprintf('Iterations %d\n',info(6));
fprintf('Reason %d\n',info(7));
plot(TestCase_51.data_in{2},TestCase_51.expected{1})
hold on
plot(TestCase_51.data_in{2},gausslinear(TestCase_51.data_in{2},...
p(1),p(2),p(3),p(4),p(5)),'r','linewidth',2)