%script to plot data
%Written for octave
%
%mwm 16.05.2011

clear;
clc;

data = dlmread('data.csv');
labels = textread('labels.csv','%s');

timeVector = data(:,1)-data(1,1);

plotThese = [2 3];

plot(timeVector,data(:,plotThese)); 
legend(labels(plotThese,:));
