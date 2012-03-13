% nowe testy dla projektu  z gtest

%% test LoadData
pa = '..\C_Matrix_Container_Factory_TEST\';
a = 1;
b = [3 4];
c = 5;
savebinarymatrix(a,[pa,'Plik_0.dat']);
savebinarymatrix(b,[pa,'Plik_1.dat']);
savebinarymatrix(c,[pa,'Plik_2.dat']);
%% Test LinSpace
pa = '..\C_Line_TEST\';
% case1
a = 0; b = 100;N = 100; 
case1 = linspace(a,b,N);
savebinarymatrix(a,[pa,'case1_0.dat']);
savebinarymatrix(b,[pa,'case1_1.dat']);
savebinarymatrix(N,[pa,'case1_2.dat']);
savebinarymatrix(case1,[pa,'case1_3.dat']);
% case2
a = 20; b = 10;N = 100; 
case2 = linspace(a,b,N);
savebinarymatrix(a,[pa,'case2_0.dat']);
savebinarymatrix(b,[pa,'case2_1.dat']);
savebinarymatrix(N,[pa,'case2_2.dat']);
savebinarymatrix(case2,[pa,'case2_3.dat']);
% case3
a = -20; b = 10;N = 99; 
case3 = linspace(a,b,N);
savebinarymatrix(a,[pa,'case3_0.dat']);
savebinarymatrix(b,[pa,'case3_1.dat']);
savebinarymatrix(N,[pa,'case3_2.dat']);
savebinarymatrix(case3,[pa,'case3_3.dat']);
%% testy evalLine
pa = '..\C_Line_TEST\';
x_data = [-4 15 100];
x = linspace(x_data(1),x_data(2),x_data(3));
a1 = 1;b1 = 0;pion1= 0;
savebinarymatrix(a1,[pa,'line1_0.dat']);
savebinarymatrix(b1,[pa,'line1_1.dat']);
savebinarymatrix(pion1,[pa,'line1_2.dat']);
savebinarymatrix(x_data,[pa,'line1_3.dat']);
a2 = -2;b2 = 10;pion2 = 0;
savebinarymatrix(a2,[pa,'line2_0.dat']);
savebinarymatrix(b2,[pa,'line2_1.dat']);
savebinarymatrix(pion2,[pa,'line2_2.dat']);
savebinarymatrix(x_data,[pa,'line2_3.dat']);
a3 = 0;b3 = 5;pion3 = 1;
savebinarymatrix(a3,[pa,'line3_0.dat']);
savebinarymatrix(b3,[pa,'line3_1.dat']);
savebinarymatrix(pion3,[pa,'line3_2.dat']);
savebinarymatrix(x_data,[pa,'line3_3.dat']);

line1 = Line(a1,b1);
line2 = Line(a2,b2);
line3 = Line(NaN,b3);
y = line1.evalLine(x);
savebinarymatrix(y(:,2)',[pa,'line1_4.dat']);
y = line2.evalLine(x);
savebinarymatrix(y(:,2)',[pa,'line2_4.dat']);
y = line3.evalLine(x);
% dla pionowych tu inaczej dzia��
y = repmat(y,x_data(3),1);
savebinarymatrix(y(:,1)',[pa,'line3_4.dat']);
%% getPointsOnLine
pa = '..\C_Line_TEST\';
P0 = [1 1];
P1 = [10 10];
N = 20;
x = linspace(P0(1),P1(1),N);
y = x;
savebinarymatrix(P0,[pa,'getPointsOnLine_0.dat']);
savebinarymatrix(P1,[pa,'getPointsOnLine_1.dat']);
savebinarymatrix(N,[pa,'getPointsOnLine_2.dat']);
savebinarymatrix(y,[pa,'getPointsOnLine_3.dat']);
savebinarymatrix(x,[pa,'getPointsOnLine_4.dat']);

P0 = [0 1];
P1 = [10 10];
N = 20;
x = linspace(P0(1),P1(1),N);
y = x;
savebinarymatrix(P0,[pa,'getPointsOnLine1_0.dat']);
savebinarymatrix(P1,[pa,'getPointsOnLine1_1.dat']);
savebinarymatrix(N,[pa,'getPointsOnLine1_2.dat']);
savebinarymatrix(y,[pa,'getPointsOnLine1_3.dat']);
savebinarymatrix(x,[pa,'getPointsOnLine1_4.dat']);
%% TEST_F(C_LineApprox_Test1, getPointonLine)
pa = '..\C_LineApprox_TEST\';
a=-2;b=10;  % sprzezone z C_LineApprox_Test1 !!!
P0 = Point(0,10);
P1 = Point(1,8);
li = Line(a,b);
y = li.getSection(P0,P1,100);
savebinarymatrix(y(:,2)',[pa,'getPointonLine.dat']);
%% test interpolacji getInterpolated_data - uruchamia� po kompilacji test�w
pa = '..\C_LineApprox_TEST\';
I = checkerboard;
lin = I(35+1,5+1:75+1);% - bo w c jest od zera
% plot(lin)
savebinarymatrix(I,[pa,'getInterpolated_data.dat']);
% wczytywanie danych do testu poprzedniego
kat = cd;
cd(pa)
!"../x64/debug/C_LineInterp_TEST.exe"
cd(kat)
pa = '..\C_LineApprox_TEST\';
ImportDumpFile([pa,'getInterpolated_data.out']);
ImportDumpFile([pa,'getInterpolated_data_case1.out']);
plot(getInterpolated_data_outx);grid on;title('getInterpolated_data_outx')
figure
plot(getInterpolated_data_outy);grid on;title('getInterpolated_data_outy')
figure
plot(getInterpolated_data_result);grid on
hold on
plot(getInterpolated_data_result_case1)
plot(lin,'or')
figure
plot(getInterpolated_data_outx_case1);grid on;title('getInterpolated_data_outx_case1')
figure
plot(getInterpolated_data_outy_case1);grid on;title('getInterpolated_data_outy_case1')


lin1 = I(10+1:50+1,5+1);% - bo w c jest od zera
ImportDumpFile([pa,'getInterpolated_data_case3.out']);
figure
plot(getInterpolated_data_result_case3);grid on;title('getInterpolated_data_result_case3')
hold on
plot(lin1,'or')
figure
plot(getInterpolated_data_outx_case3);grid on;title('getInterpolated_data_outx_case3')
figure
plot(getInterpolated_data_outy_case3);grid on;title('getInterpolated_data_outy_case3')
%% TEST(C_LineweldApprox_Test1, GaussLin_Case_1)
pa = '..\C_LineweldApprox_TEST\';
a = 10;
b = 1000;
c = 100;
d = 0;
e = 0;
x = 1:2000;
data_in = [a b c d e];
expected = gausslinear(x,data_in(1),data_in(2),data_in(3),data_in(4),data_in(5));
savebinarymatrix(data_in,[pa,'GaussLin_test_1.dat']);
savebinarymatrix(x,[pa,'GaussLin_test_1_x.dat']);
savebinarymatrix(expected,[pa,'GaussLin_test_1_expected.dat']);
%% TEST(C_LineweldApprox_Test1, jGaussLin_Case_1)
pa = '..\C_LineweldApprox_TEST\';
syms a b c d e1 e
f = 'a*e^(-((x-b)/c)^2)+d*x+e1';
v = [a,b,c,d,e1];
R = jacobian(f, v);
a = 1;b = 1;c = 1; d = 1; e1 = 1; x = 1;e = exp(1);
data_in = [a b c d e1];
expected = eval(R);
savebinarymatrix(data_in,[pa 'jGaussLin_test_1.dat']);% wsp
savebinarymatrix(x,[pa 'jGaussLin_test_1_x.dat']);% warto�c x
savebinarymatrix(expected,[pa 'jGaussLin_test_1_expected.dat']);
%% TEST(C_LineweldApprox_Test1, jGaussLin_Case_2)
pa = '..\C_LineweldApprox_TEST\';
syms a b c d e1 e
f = 'a*e^(-((x-b)/c)^2)+d*x+e1';
v = [a,b,c,d,e1];
R = jacobian(f, v);
a = 10;b = 1;c = 0.25; d = -1.7; e1 = 7; x = 100;e = exp(1);
data_in = [a b c d e1];
expected = eval(R);
savebinarymatrix(data_in,[pa 'jGaussLin_test_2.dat']);% wsp
savebinarymatrix(x,[pa 'jGaussLin_test_2_x.dat']);% warto�c x
savebinarymatrix(expected,[pa 'jGaussLin_test_2_expected.dat']);
%% TEST(C_LineweldApprox_Test1, jGaussLin_Case_3)
clear all
pa = '..\C_LineweldApprox_TEST\';
syms a b c d e1 e
f = 'a*e^(-((x-b)/c)^2)+d*x+e1';
v = [a,b,c,d,e1];
x_data = 1:1825;
R = jacobian(f, v);
a = 65535/2;b =  length(x_data)/2 ;c = 150; d = 1; e1 = 7; e = exp(1);
data_in = [a b c d e1];
for aa=1:length(x_data)
    x = x_data(aa);
    expected(aa,:) = eval(R);
end
expected = reshape(expected,[],5);
savebinarymatrix(data_in,[pa 'jGaussLin_test_3.dat']);% wsp
savebinarymatrix(x_data,[pa 'jGaussLin_test_3_x.dat']);% warto�c x
savebinarymatrix(expected,[pa 'jGaussLin_test_3_expected.dat']);
%% TEST(C_LineweldApprox_Test1,getLineApproxGaussLin)
clear all
pa = '..\C_LineweldApprox_TEST\';

im1 = double(imread('300_09.tif'));
im1r = RemoveFrame(im1); 
data_in = im1r(:,4814)';
x = 1:length(data_in);
p = [1060 length(data_in)/2 160 0 0];
lb = [0 length(data_in)/2-length(data_in)/4 50 -1 -20000];
ub = [65535 length(data_in)/2+length(data_in)/4 600 1 20000];

savebinarymatrix(data_in,[pa,'getLineApproxGaussLin_test_1.dat']);
savebinarymatrix(x,[pa,'getLineApproxGaussLin_test_1_x.dat']);
savebinarymatrix(p,[pa,'getLineApproxGaussLin_test_1_p.dat']);
savebinarymatrix(lb,[pa,'getLineApproxGaussLin_test_1_lb.dat']);
savebinarymatrix(ub,[pa,'getLineApproxGaussLin_test_1_ub.dat']);
kat = cd;
cd(pa)
!"../x64/debug/C_LineweldApprox_TEST.exe"
cd(kat)
ImportDumpFile([pa,'getLineApproxGaussLin_test_1_expected.out']);
e = gausslinear(x,expected(1),expected(2),expected(3),expected(4),expected(5));
plot(x,data_in)
hold on
plot(x,e,'r','linewidth',2);
%% class C_LinearWeld_Test1
clear all
pa = '..\C_WeldlineDetect_TEST\';

rtg = double(imread('300_09.tif'));
rtg1 = RemoveFrame(rtg); 
savebinarymatrix(rtg1,[pa,'testimag1.dat']);