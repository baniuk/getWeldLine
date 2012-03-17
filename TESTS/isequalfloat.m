function ret=isequalfloat(a,b,dok)
% porównuje z zadan¹ dok³¹dnoœci¹
% dok³¹dnoœæ <1 musi byæ i oznacza iloœæ pozycji na których jest porowywane

d = abs(a-b);

if d>dok
    ret = 0;
else
    ret = 1;
end