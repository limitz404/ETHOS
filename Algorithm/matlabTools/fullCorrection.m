close all;
clear all;
clc;
format long;

attitudeOutput;
actualAngle = [0, 5, 10, 15, 20];
alts = [750, 725, 700, 675, 650, 625, 600, 575, 550, 525, 500, 475, 450, 425, 400,375,350,325,300,275,250];

for k=1:size(r,1)
    [Pp(k,:), ~] = polyfit(p(k,:), actualAngle,2);
end

for k=1:size(r,1)
    [Pr(k,:), ~] = polyfit(r(k,:), -actualAngle,2);
end

%% Plot corrected pitch errors vs actual angle for all altitudes for the Adjusted case

[pa, ~] = polyfit(alts', Pp(:,1), 1)
[pb, ~] = polyfit(alts', Pp(:,2), 1)
[pc, ~] = polyfit(alts', Pp(:,3), 1)
[ra, ~] = polyfit(alts', Pr(:,1), 1)
[rb, ~] = polyfit(alts', Pr(:,2), 1)
[rc, ~] = polyfit(alts', Pr(:,3), 1)