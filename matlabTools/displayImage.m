close all;
clear all;
clc;

filePath = './image.txt';
bitbangImage = load(filePath);
imagesc(fliplr(rot90(bitbangImage,0)));
colormap(gray);