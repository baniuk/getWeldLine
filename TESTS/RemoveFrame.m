function out = RemoveFrame(in)
% Funkcja uzuwa ramkê wokó³ skanowanych obrazów.
% przycinanie polega na znalezieniu najbardziej wysuniêtych pikseli obrazka
% po rogach (najbli¿szych rogów i uznanie ich jako pixeli wierzcho³ka ROI

im1sc = Scale01(in);
si = size(im1sc);
z = 200;    % iloœæ pixeli rogu do analizy
pr = 5; % ilosc marginesu

l_g = im1sc(1:z,1:z);  % wycinek lewego górnego rogu
[r,k] = find(l_g<0.9);  % wszytkie ciemne
% szukam który z ciemnych pixeli ma najbli¿ej do rogu
dist = sqrt((1-r).^2+(1-k).^2);
m_d = find(dist==min(dist));
cut_lg = [r(m_d(1)) k(m_d(1))];   % punkt ciêcia dla lewego górnego rogu

l_d = im1sc(end-z:end,1:z);  % wycinek lewego dolnego rogu
[r,k] = find(l_d<0.9);  % wszytkie ciemne
r = r + si(1) - z;
% szukam który z ciemnych pixeli ma najbli¿ej do rogu
dist = sqrt((si(1)-r).^2+(1-k).^2);
m_d = find(dist==min(dist));
cut_ld = [r(m_d(1)) k(m_d(1))];   % punkt ciêcia dla lewego dolnego rogu

p_g = im1sc(1:z,end-z:end);  % wycinek prawego górnego rogu
[r,k] = find(p_g<0.9);  % wszytkie ciemne
k = k + si(2) - z;
% szukam który z ciemnych pixeli ma najbli¿ej do rogu
dist = sqrt((1-r).^2+(si(2)-k).^2);
m_d = find(dist==min(dist));
cut_pg = [r(m_d(1)) k(m_d(1))];   % punkt ciêcia dla prawego górnego rogu

p_d = im1sc(end-z:end,end-z:end);  % wycinek prawego górnego rogu
[r,k] = find(p_d<0.9);  % wszytkie ciemne
k = k + si(2) - z;
r = r + si(1) - z;
% szukam który z ciemnych pixeli ma najbli¿ej do rogu
dist = sqrt((si(1)-r).^2+(si(2)-k).^2);
m_d = find(dist==min(dist));
cut_pd = [r(m_d(1)) k(m_d(1))];   % punkt ciêcia dla prawego górnego rogu

hh = figure; imshow(im1sc);
hold on
plot(cut_lg(2),cut_lg(1),'o')
plot(cut_ld(2),cut_ld(1),'o')
plot(cut_pg(2),cut_pg(1),'o')
plot(cut_pd(2),cut_pd(1),'o')

L = [cut_lg;cut_ld;cut_pd;cut_pg;cut_lg];
L = fliplr(L);
line(L(:,1),L(:,2),'color','r')
% hold off

% wiêkszy z rzêdów (od góry w dó³ siê przesuwam)
s = sort([cut_lg(1) cut_pg(1)]);
r_start = s(2);

% mniejszy z rzêdów (od do³u w górê siê przesuwam)
s = sort([cut_ld(1) cut_pd(1)]);
r_stop = s(1);

% wiêkszy z kolumn (od lewej do prawej siê przesuwam)
s = sort([cut_lg(2) cut_ld(2)]);
k_start = s(2);

% mniejszy z kolumn (od prawej do lewej siê przesuwam)
s = sort([cut_pg(2) cut_pd(2)]);
k_stop = s(1);

% nowe rogi
r_lg = [r_start+pr k_start+pr];
r_ld = [r_stop-pr k_start+pr];
r_pg = [r_start+pr k_stop-pr];
r_pd = [r_stop-pr k_stop-pr];
L = [r_lg;r_ld;r_pd;r_pg;r_lg];
L = fliplr(L);
line(L(:,1),L(:,2),'color','g')
close(hh);
out = in(r_start+pr:r_stop-pr,k_start+pr:k_stop-pr);