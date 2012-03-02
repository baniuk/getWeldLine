function y=gausslinear(x,a1,b1,c1,d1,e1)

y = a1*exp(-((x-b1)./c1).^2)+d1*x+e1;