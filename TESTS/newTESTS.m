%% opracowanie wa¿enia
%load wagi

w = mean(etab); % uœrednione aproxymacje z bufora
plot(w);hold on
s = profil5(10,:); % z³y sygna³
plot(s,'-r')
d = abs(s-w).^3;
mm = minmax(d);
for l=1:length(d)
    d(l) = d(l)/(mm(2)-mm(1))-mm(1)/(mm(2)-mm(1));
end

dd = 1-d;
sr = (w.*dd + s.*d);
plot(sr,'-g');
%% wagi_3
w = mean(etab); % uœrednione aproxymacje z bufora
plot(w);hold on
s = profil5(10,:); % z³y sygna³
plot(s,'-r')
d = sqrt(w);

mm = minmax(d);
for l=1:length(d)
    d(l) = d(l)/(mm(2)-mm(1))-mm(1)/(mm(2)-mm(1));
end

dd = 1-d;
sr = (w.*dd + s.*d);
plot(sr,'-g');
%% nowe testy dla projektu  z gtest
% class C_LinearWeld_Test1 - dane wejsciowe musz¹ byc w katlaogu danego
% testu
clear all
pa = '..\C_WeldlineDetect_TEST\';

im1 = double(imread('100_01.tif'));
im2 = double(imread('100_02.tif'));
im3 = double(imread('200_04.tif'));
im4 = double(imread('200_08.tif'));

im5 = double(imread('100_06.tif'));
im6 = double(imread('400_05.tif'));
im7 = double(imread('200_10.tif'));
im8 = double(imread('400_09.tif'));
im9 = double(imread('300_09.tif'));

im1c = RemoveFrame(im1);
im2c = RemoveFrame(im2);
im3c = RemoveFrame(im3);
im4c = RemoveFrame(im4);

im5c = RemoveFrame(im5);
im6c = RemoveFrame(im6);
im7c = RemoveFrame(im7);
im8c = RemoveFrame(im8); im8c(:,1:589) = [];
im9c = RemoveFrame(im9); 

figure;imshow(im1c,[]);
figure;imshow(im2c,[]);
figure;imshow(im3c,[]);
figure;imshow(im4c,[]);
figure;imshow(im5c,[]);
figure;imshow(im6c,[]);
figure;imshow(im7c,[]);
figure;imshow(im8c,[]);
figure;imshow(im9c,[]);

savebinarymatrix(im1c,[pa,'testimag1.dat']);
savebinarymatrix(im2c,[pa,'testimag2.dat']);
savebinarymatrix(im3c,[pa,'testimag3.dat']);
savebinarymatrix(im4c,[pa,'testimag4.dat']);
savebinarymatrix(im5c,[pa,'testimag5.dat']);
savebinarymatrix(im6c,[pa,'testimag6.dat']);
savebinarymatrix(im7c,[pa,'testimag7.dat']);
savebinarymatrix(im8c,[pa,'testimag8.dat']);
savebinarymatrix(im9c,[pa,'testimag9.dat']);




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
% dla pionowych tu inaczej dzia³¹
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
%% test interpolacji getInterpolated_data - uruchamiaæ po kompilacji testów
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
savebinarymatrix(x,[pa 'jGaussLin_test_1_x.dat']);% wartoœc x
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
savebinarymatrix(x,[pa 'jGaussLin_test_2_x.dat']);% wartoœc x
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
savebinarymatrix(x_data,[pa 'jGaussLin_test_3_x.dat']);% wartoœc x
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

%% C_LinearWeld_Test1, C_LinearWeld_FillBuffor
clear all
rtg = double(imread('300_09.tif'));
rtg1 = RemoveFrame(rtg); 
imshow(rtg1,[]); hold on
pa = '..\C_WeldlineDetect_TEST\';
PAR = [10,15,100,1612,3253,4294,6000];   % parametr z testu parametrycznego - tu pozycja startowa
plot(PAR,repmat(size(rtg1,1)/2,1,length(PAR)),'o','markerfacecolor','r')
for pp=1:length(PAR)
    ImportDumpFile([pa,'C_LinearWeld_FillBuffor',num2str(PAR(pp)),'.out']);
%     figure
%     plot(x');title('C_LinearWeld_FillBuffor_x');grid on
%     figure
%     plot(y');title('C_LinearWeld_FillBuffor_y');grid on
%     figure
%     plot(profil');title('C_LinearWeld_FillBuffor_profil');grid on
    figure
    title(['grupowy zal od y bo linie pionowe start: ',num2str(PAR(pp))])
    clear etab
    for a=1:size(p,1)
        subplot(4,7,a)
        e = gausslinear(y(a,:),p(a,1),p(a,2),p(a,3),p(a,4),p(a,5));
        plot(y,profil(a,:),'-b');
        hold on
        plot(y,e,'r','linewidth',2);
        grid on
        title(num2str(x(a,1)))
        etab(a,:) = e;
    end
    disp('wunik testu getNextParams')
    rr = all([isequal(p_med,median(p)),...
        isequal(ub_med,median(ub)),...
        isequal(lb_med,median(lb)),...
        isequalfloat(w,mean(etab),1e-8)])
    if(rr==0)
        break;
    end
end
%% C_LinearWeld_Test1, C_LinearWeld_evalWeldPos
clear all
rtg = double(imread('300_09.tif'));
rtg1 = RemoveFrame(rtg); 
pa = '..\C_WeldlineDetect_TEST\';
PAR = [10,15,100,1612,3253,4294,6000];   % parametr z testu parametrycznego - tu pozycja startowa
for pp=1:length(PAR)
    ImportDumpFile([pa,'C_LinearWeld_evalWeldPos',num2str(PAR(pp)),'.out']);
%     figure
%     plot(x');title('C_LinearWeld_FillBuffor_x');grid on
%     figure
%     plot(y');title('C_LinearWeld_FillBuffor_y');grid on
%     figure
%     plot(profil');title('C_LinearWeld_FillBuffor_profil');grid on
    figure
    title(['grupowy zal od y bo linie pionowe start: ',num2str(PAR(pp))])
    e = gausslinear(0:size(rtg1,1)-1,p(1),p(2),p(3),p(4),p(5));
    plot(0:size(rtg1,1)-1,e,'r','linewidth',2);
    grid on
    figure;
    imshow(rtg1,[]); hold on
    plot(points(1,1)+1,points(1,2)+1,'ro')
    plot(points(2,1)+1,points(2,2)+1,'x')
    plot(points(3,1)+1,points(3,2)+1,'rs')
    title(num2str(PAR(pp)));
end
%% C_LinearWeld_Start, start
clear all
pa = '..\C_WeldlineDetect_TEST\';
nazwa = {'testimag1.dat','testimag9.dat'};
krok = 1;
for a=1:length(nazwa)
    figure
    rtg1 = readbinarymatrix([pa,nazwa{a}]);
    ImportDumpFile([pa,'C_LinearWeld_Start_start_',nazwa{a},'.out']);
    start = 10+1;% piksel od którego starujemy z testu

    lineoke = [NaN(1,start-1) lineok];
    lineoke = repmat(lineoke,size(rtg1,1),1);
    subplot(2,1,1)
    imshow(rtg1,[]); hold on
    D = weldpos(1:2,:)';
    S = weldpos(3:4,:)';
    G = weldpos(5:6,:)';
    plot(D(:,1),D(:,2),'marker','.','markerfacecolor','r','markersize',4,'LineStyle','none')
    plot(S(:,1),S(:,2),'marker','.','markerfacecolor','g','markersize',4,'LineStyle','none')
    plot(G(:,1),G(:,2),'marker','.','markerfacecolor','r','markersize',4,'LineStyle','none')
    subplot(2,1,2)
    imshow(rtg1,[]);hold on
    for aa=1:length(lineok)
        if lineok(aa)==0
            plot(aa,size(rtg1,1)/2,'x','markerfacecolor','r','markersize',10)
        end
    end
end
%% C_LinearWeld_Start, start_step10
clear all
pa = '..\C_WeldlineDetect_TEST\';
nazwa = {'testimag1.dat','testimag2.dat','testimag3.dat','testimag4.dat','testimag5.dat','testimag6.dat','testimag7.dat','testimag8.dat','testimag9.dat'};
for a=1:length(nazwa)
    figure
    rtg1 = readbinarymatrix([pa,nazwa{a}]);
    ImportDumpFile([pa,'C_LinearWeld_Start_start10_',nazwa{a},'.out']);
    start = 10+1;% piksel od którego starujemy z testu

    lineoke = [NaN(1,start-1) lineok];
    lineoke = repmat(lineoke,size(rtg1,1),1);
    subplot(2,1,1)
    imshow(rtg1,[]); hold on
    D = weldpos(1:2,:)';
    S = weldpos(3:4,:)';
    G = weldpos(5:6,:)';
    plot(D(:,1),D(:,2),'marker','o','markerfacecolor','r','markersize',2,'LineStyle','-')
    plot(S(:,1),S(:,2),'marker','.','markerfacecolor','g','markersize',2,'LineStyle','none')
    plot(G(:,1),G(:,2),'marker','o','markerfacecolor','r','markersize',2,'LineStyle','-')
    subplot(2,1,2)
    imshow(rtg1,[]);hold on
    for aa=1:length(lineok)
        if lineok(aa)==0
            plot(aa,size(rtg1,1)/2,'x','markerfacecolor','r','markersize',10)
        end
    end
end
%% C_LinearWeld_Start, start_step100
clear all
pa = '..\C_WeldlineDetect_TEST\';
nazwa = {'testimag1.dat','testimag2.dat','testimag3.dat','testimag4.dat','testimag5.dat','testimag6.dat','testimag7.dat','testimag8.dat','testimag9.dat'};
for a=1:length(nazwa)
    figure
    rtg1 = readbinarymatrix([pa,nazwa{a}]);
    ImportDumpFile([pa,'C_LinearWeld_Start_start100_',nazwa{a},'.out']);
    start = 10+1;% piksel od którego starujemy z testu

    lineoke = [NaN(1,start-1) lineok];
    lineoke = repmat(lineoke,size(rtg1,1),1);
    subplot(2,1,1)
    imshow(rtg1,[]); hold on
    D = weldpos(1:2,:)';
    S = weldpos(3:4,:)';
    G = weldpos(5:6,:)';
    plot(D(:,1),D(:,2),'marker','o','markerfacecolor','r','markersize',2,'LineStyle','-')
    plot(S(:,1),S(:,2),'marker','.','markerfacecolor','g','markersize',2,'LineStyle','-')
    plot(G(:,1),G(:,2),'marker','o','markerfacecolor','r','markersize',2,'LineStyle','-')
    subplot(2,1,2)
    imshow(rtg1,[]);hold on
    for aa=1:length(lineok)
        if lineok(aa)==0
            plot(aa,size(rtg1,1)/2,'x','markerfacecolor','r','markersize',10)
        end
    end
end
%% weryfikacja rêczna na podstawie logu
clear all
pa = '..\C_WeldlineDetect_TEST\';
nazwa = 'testimag9.dat';

l = 2510;% numer linii x z logu

rtg1 = readbinarymatrix([pa,nazwa]);
pr = rtg1(:,l+1);   % profil z obrazu na podstawie linii l
figure;plot(pr); grid on;hold on
A=552.38; B=1070.65; C=88.54; D=-0.29; E=1327.35;   % skopiowane z logu
e = gausslinear(0:size(rtg1,1)-1,A,B,C,D,E);
plot(e,'-r')
%% testy procedur cf

% point 1
clear all
pa = '..\CalcFeatures_TEST\';
nazwa = 'testimag9.dat';
ImportDumpFile([pa,'point1.out']);

poz = poz+1; x=x+1; y = y+1;
rtg1 = readbinarymatrix([pa,nazwa]);
figure; imshow(rtg1,[]);
hold on
plot(poz(1,1),poz(1,2),'bo');
plot(poz(2,1),poz(2,2),'rx');
plot(poz(3,1),poz(3,2),'bs');
plot(x,y,'gs');
title(inout)

% point 2
clear all
pa = '..\CalcFeatures_TEST\';
nazwa = 'testimag9.dat';
ImportDumpFile([pa,'point2.out']);

poz = poz+1; x=x+1; y = y+1;
rtg1 = readbinarymatrix([pa,nazwa]);
figure; imshow(rtg1,[]);
hold on
plot(poz(1,1),poz(1,2),'bo');
plot(poz(2,1),poz(2,2),'rx');
plot(poz(3,1),poz(3,2),'bs');
plot(x,y,'gs');
title(inout)


% point 3
clear all
pa = '..\CalcFeatures_TEST\';
nazwa = 'testimag9.dat';
ImportDumpFile([pa,'point3.out']);

poz = poz+1; x=x+1; y = y+1;
rtg1 = readbinarymatrix([pa,nazwa]);
figure; imshow(rtg1,[]);
hold on
plot(poz(1,1),poz(1,2),'bo');
plot(poz(2,1),poz(2,2),'rx');
plot(poz(3,1),poz(3,2),'bs');
plot(x,y,'gs');
title(inout)
%% testy procedur cf
pa = '..\CalcFeatures_TEST\';
ImportDumpFile([pa,'getLineApproxGaussLinWeighted_QFXGGDTKMG.out']);
%% testy cf - szukanie problemów e stosem
pa = '..\CalcFeatures_TEST\';

im1 = double(imread('100_01s.tif'));
im2 = double(imread('14-35-49_257A_flaw1A_normal.tif'));

savebinarymatrix(im1,[pa,'100_01s.dat']);
savebinarymatrix(im1,[pa,'14-35-49_257A_flaw1A_normal.dat']);

% point 1
clear all
pa = '..\CalcFeatures_TEST\';
nazwa = '100_01s.dat';
ImportDumpFile([pa,'point1_1.out']);

poz = poz+1; x=x+1; y = y+1;
rtg1 = readbinarymatrix([pa,nazwa]);
figure; imshow(rtg1,[]);
hold on
plot(poz(1,1),poz(1,2),'bo');
plot(poz(2,1),poz(2,2),'rx');
plot(poz(3,1),poz(3,2),'bs');
plot(x,y,'gs');
title(inout)