function out = Scale01(data,varargin)
% skaluje dane do zakresu 0-1
% varargin - [a b] = skaluje do zakresu a - b

if diff(minmax(data(:)'))==0
    disp('Dane jednorodne')
    out = data;
    return;
end
if nargin==1
    mi = unique(min(min(data)));
    skala = data - mi;
    ma = unique(max(max(skala)));
    outtmp = skala/ma;
else
    zakres = varargin{1};
    delta = abs(zakres(2) - zakres(1));
    mi = unique(min(min(data)));
    ma = unique(max(max(data)));
    outtmp = delta.*data/(ma - mi)-mi*delta/(ma - mi);
end
out = outtmp;