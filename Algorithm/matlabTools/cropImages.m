close all;
clear all;
clc;

%% Image locations
folder = '/Users/taylordean/Desktop/Pics';
outFolder = '/Users/taylordean/ETHOS_Git/Algorithm/testImages';

%% Find all images
dirs = dir(folder);

%% Loop through dirs and images and crop them
for k=1:length(dirs)
    if ~strcmp(dirs(k).name,'.') && ~strcmp(dirs(k).name,'..') && ~strcmp(dirs(k).name,'.DS_Store');
        %imDirs = dir(strcat(folder,dirs(k).name))
        imDirs = dirs(k);
        for n=1:length(imDirs)
            %imNames = dir(strcat(folder,dirs(k).name,'/',imDirs(n).name,'/*png*'));
            imNames = imDirs;
            for m=1:length(imNames)
                %imPath = strcat(folder,dirs(k).name,'/',imDirs(n).name,'/',imNames(m).name);
                imPath = strcat(folder,'/',imNames(m).name);
                im = imread(imPath);
                bw = rgb2gray(im);
                cropped = bw(109:end-160,113:end-112);
                %saveLoc = strcat(outFolder, dirs(k).name,'/',imDirs(n).name,'/',imNames(m).name);
                saveLoc = strcat(outFolder,'/',imNames(m).name);
                finalIm = imresize(cropped,[162,128]);
                imwrite(finalIm,saveLoc);
            end
        end
    end
end