function ret=isequalfloat(a,b,dok)
% por�wnuje z zadan� dok��dno�ci�
% dok��dno�� <1 musi by� i oznacza ilo�� pozycji na kt�rych jest porowywane

d = abs(a-b);

if d>dok
    ret = 0;
else
    ret = 1;
end