function out = addipimport(name)
% Imports binary file from addip software. Returns an image

fid = fopen(name, 'r');

x = fscanf(fid, '%ld\n',1);
y = fscanf(fid, '%ld\n',1);
c = fscanf(fid, '%f\n',[y x]);
fclose(fid);

out = c';