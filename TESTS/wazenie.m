function out = wazenie(s,etab)

w = mean(etab); % uœrednione aproxymacje z bufora
d = abs(s-w).^1.2;
mm = minmax(d);
for l=1:length(d)
    d(l) = d(l)/(mm(2)-mm(1))-mm(1)/(mm(2)-mm(1));
end

dd = 1-d;
out = w.*dd + s.*d;