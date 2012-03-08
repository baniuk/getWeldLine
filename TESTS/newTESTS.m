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
%% test interpolacji getInterpolated_data
pa = '..\C_LineApprox_TEST\';
I = checkerboard;
lin = I(35,5:75);
plot(lin)
savebinarymatrix(I,[pa,'getInterpolated_data.dat']);
    %% wczytywanie danych do testu poprzedniego
pa = '..\C_LineApprox_TEST\';
ImportDumpFile([pa,'getInterpolated_data.out']);
