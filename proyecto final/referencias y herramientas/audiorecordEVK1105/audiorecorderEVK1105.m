clc
clear all
%nombre='bienvenido'
%nombre='temperaturaAlta';
%nombre='lucesEncendidos';
nombre='gasDetectado';
[y,Fs] = audioread(strcat(nombre,'.wav'),'native');
fid = fopen(strcat(nombre,'.h'),'w');
res = dec2hex(y/2); 
fprintf(fid,strcat("const unsigned char ",nombre,"[] = {"));
for i = 1:1: length(res)
fprintf(fid,'0x%s,',res(i,:,:,:));
end
fprintf(fid,"};");
fclose(fid);