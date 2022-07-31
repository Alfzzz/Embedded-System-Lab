clc
clear all
[y,Fs] = audioread('temperaturaAlta.wav','native');
fid = fopen('temperaturaAlta.h','w');
res = dec2hex(y);
fprintf(fid,"const unsigned char temperaturaAlta[] = {");
for i = 1:1: length(res)
fprintf(fid,'0x%s,',res(i,:,:,:));
end
fprintf(fid,"};");
fclose(fid);