clc
close all
clear all
delete(instrfind({'Port'},{'COM6'}))

x=serial('COM6','BAUD', 57600);
fopen(x);
fprintf(x,'%c','w','async');
%% 
n=1;
%img = imread('chew.jpeg');
img = imread('comandante.jpg');
%img = imread('comandante80.jpg');
for row = 1:20
    for col = 1:20
        pixel = impixel(img,col,row);

        
            b(n)=floor(pixel(3)*32/256)
            fprintf(x,'%c',b(n),'async');
            pause(0.06);
            g(n)=floor(pixel(2)*64/256)
            fprintf(x,'%c',g(n),'async');
            pause(0.06);
            r(n)=floor(pixel(1)*32/256)
            fprintf(x,'%c',r(n),'async');
            pause(0.06);
            n=n+1;
    end
end
fprintf(x,'%c',100,'async');