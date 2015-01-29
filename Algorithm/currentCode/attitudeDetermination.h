//  Author: Taylor Dean
//  Email: Taylor.Dean@Colorado.Edu
//  File: attitudeDetermination.h
//  Purpose: This file contains all of the required functions to perform the edge 
//           detection and attitude determination.
//
//  Created by Taylor Dean on 11/17/14.
//  Copyright (c) 2014 Taylor Dean. All rights reserved.
//

#ifndef ATTITUDE_DETERMINATION_H
#define ATTITUDE_DETERMINATION_H

#include "dataStructures.h"
#include <string.h>
#include <chrono>
#include "edgeDetection.h"
#include <fstream>

using namespace cv;
using namespace std;

// Global Variables
const float thresholdFactor = atof(getenv("THRESHOLDFACTOR"));
const int buffer = atoi(getenv("BUFFER"));
const int subPix = atoi(getenv("SUBPIX"));
const float altitude = atof(getenv("ALTITUDE"));
const float pixelPitch = atof(getenv("PIXELPITCH"));
const int searchRange = atoi(getenv("SEARCHRANGE"));
const float fLength = atof(getenv("FLENGTH"));
const int visualizer = atoi(getenv("VISUALIZER"));
const string imPath = getenv("IMAGEPATH");
const int latency = atoi(getenv("LATENCY"));
const int lookForward = atoi(getenv("LOOKFORWARD"));
const int calibrate = atoi(getenv("CALIBRATE"));
// End Global Variables

// Forward Declaration of Functions
void initializeAlgorithm();
void determineAttitude();
void calibrateAlgorithm();
// End Forward Declaration

void initializeAlgorithm()
{
		if (calibrate == 1) {
				calibrateAlgorithm();
		}
		else {
				determineAttitude();
		}
}

void determineAttitude()
{    
    if (visualizer == 1){
        namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    }
    // Start timer
    chrono::high_resolution_clock::time_point tStart = chrono::high_resolution_clock::now();
    
    coefficients coeff = getCoefficients(altitude);
    
    Mat image;
    image = imread(imPath, CV_LOAD_IMAGE_GRAYSCALE);   // Read the file
    
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl;
    }
    
    pair<string, float> output = startSide(image, buffer);
    float threshold = output.second*thresholdFactor;
    
    vector<vector<float> > edges;
    
    if (output.first == "top") {
        edges = topEdges(image, threshold, searchRange, lookForward);
    }
    else if (output.first == "bottom") {
        edges = bottomEdges(image, threshold, searchRange, lookForward);
    }
    else if (output.first == "left") {
        edges = leftEdges(image, threshold, searchRange, lookForward);
    }
    else if (output.first == "right") {
        edges = rightEdges(image, threshold, searchRange, lookForward);
    }
    else {
        cout << "Did not recognize start side value" << endl;
    }
    
    lstcircle lstSqCircle;
    
    // Perform subpixel estimation if desired
    if (subPix == 1) {
        vector<vector<float> > modEdges = performSubpixelEstimation(image,  edges, threshold, output.first);
        lstSqCircle = circularLeastSquares(modEdges, image.cols, image.rows);
    }
    else {
        lstSqCircle = circularLeastSquares(edges, image.cols, image.rows);
    }
    
    pair<float, float> center = make_pair(lstSqCircle.xc, lstSqCircle.yc);
    
    float rollAngle = calculateRoll(center);
    float pitchAngle = calculatePitch2(center, lstSqCircle.r, fLength, pixelPitch);
    float rollCor = reduceRollError(rollAngle, coeff);
    float pitchCor = reducePitchError(pitchAngle, coeff);
    
    if (latency == 1){
        // ALL CALCULATIONS SHOULD OCCURE ABOVE THIS COMMENT
        // End timer
        chrono::high_resolution_clock::time_point tEnd = chrono::high_resolution_clock::now();
        
        // Calculate elapsed time
        float duration = std::chrono::duration_cast<std::chrono::microseconds>(tEnd - tStart).count();
        
        // Output effective rate in Hz
        cout << "\n\nExecution Speed = " << 1000000/duration << " Hz" << endl;
    }
    if (visualizer == 1){
        // Show edges
        Mat newImage = showEdges(image, edges);
        imshow( "Display window", newImage);                   // Show our image inside it.
        waitKey(0);
    }
    
    // Output pitch and roll calculations for debugging
    cout << "pitch = " << pitchAngle << "º" << endl;
    cout << "roll = " << rollAngle << "º" << endl;
    cout << "corrected pitch = " << pitchCor << "º" << endl;
    cout << "corrected roll = " << rollCor << "º\n\n" << endl;
}

void calibrateAlgorithm()
{
		if (visualizer == 1){
        namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    }
    string alt1 [] = {"750", "725", "700", "675", "650", "625", "600", "575",
     "550", "525", "500", "475", "450", "425", "400", "375", "350", "325", "300", "275",
      "250"};
    float alts [] ={750, 725, 700, 675, 650, 625, 600, 575,
     								550, 525, 500, 475, 450, 425, 400, 375, 350, 325, 300, 275, 250};

    string fname = "../matlabTools/attitudeOutput.m";
    string logName = "./algorithm_log.txt";
    ofstream fout;
    fout.open(fname);
    ofstream logout;
    logout.open(logName);
    chrono::high_resolution_clock::time_point tStart;
    chrono::high_resolution_clock::time_point tEnd;
    float duration;
    coefficients coeff;
    
    logout << "Image Name" << "\t" << "Altitude [km]" << "\t" << "Roll Angle [º]" << "\t" << "Corrected Roll [º]" << "\t" << "Pitch Angle [º]" << "\t" << "Corrected Pitch [º]" << "\t" << "Latency [s]" << "\t" << "Rate [Hz]" << endl;
    
    float rollAngle [] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
												 -1, -1, -1, -1, -1, -1, -1};
		float pitchAngle [] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
												 -1, -1, -1, -1, -1, -1, -1};
		float rollCor [] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
												 -1, -1, -1, -1, -1, -1, -1};
		float pitchCor [] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
												 -1, -1, -1, -1, -1, -1, -1};
    
		for (int n=0; n<21; n++)
		{
				string alt = alt1[n];
				coeff = getCoefficients(alts[n]);
				string testImages [] = {"../testImages/"+alt+"p0.png",
						"../testImages/"+alt+"p5.png",
						"../testImages/"+alt+"p10.png",
						"../testImages/"+alt+"p15.png",
						"../testImages/"+alt+"p20.png",
						"../testImages/"+alt+"r0.png",
						"../testImages/"+alt+"r5.png",
						"../testImages/"+alt+"r10.png",
						"../testImages/"+alt+"r15.png",
						"../testImages/"+alt+"r20.png"};
				
				for (int k=0;k<10;k++)
				{

						// Start timer
    				tStart = chrono::high_resolution_clock::now();
    
						Mat image;
						image = imread(testImages[k], CV_LOAD_IMAGE_GRAYSCALE);   // Read the file
		
						if(! image.data )                              // Check for invalid input
						{
								cout <<  "Could not open or find the image" << std::endl;
						}
		
						pair<string, float> output = startSide(image, buffer);
						float threshold = output.second*thresholdFactor;
		
						vector<vector<float> > edges;
		
						if (output.first == "top") {
								edges = topEdges(image, threshold, searchRange, lookForward);
						}
						else if (output.first == "bottom") {
								edges = bottomEdges(image, threshold, searchRange, lookForward);
						}
						else if (output.first == "left") {
								edges = leftEdges(image, threshold, searchRange, lookForward);
						}
						else if (output.first == "right") {
								edges = rightEdges(image, threshold, searchRange, lookForward);
						}
						else {
								cout << "Did not recognize start side value" << endl;
						}
		
						lstcircle lstSqCircle;
		
						// Perform subpixel estimation if desired
						if (subPix == 1) {
								vector<vector<float> > modEdges = performSubpixelEstimation(image,  edges, threshold, output.first);
								lstSqCircle = circularLeastSquares(modEdges, image.cols, image.rows);
						}
						else {
								lstSqCircle = circularLeastSquares(edges, image.cols, image.rows);
						}
		
						pair<float, float> center = make_pair(lstSqCircle.xc, lstSqCircle.yc);
						
						rollAngle[k] = calculateRoll(center);
						pitchAngle[k] = calculatePitch2(center, lstSqCircle.r, fLength, pixelPitch);
						rollCor[k] = reduceRollError(rollAngle[k], coeff);
						pitchCor[k] = reducePitchError(pitchAngle[k], coeff);
												
						// End timer
        		tEnd = chrono::high_resolution_clock::now();
						
						if (visualizer == 1){
								// Show edges
								Mat newImage = showEdges(image, edges);
								imshow( "Display window", newImage);                   // Show our image inside it.
								waitKey(0);
						}
						
						duration = std::chrono::duration_cast<std::chrono::microseconds>(tEnd - tStart).count();
						
						logout << testImages[k] << "\t" << alt << "\t" << rollAngle[k] << "\t" << rollCor[k] << "\t" << pitchAngle[k] << "\t" << pitchCor[k] << "\t" << duration/1000000 << "\t" << 1000000/duration << endl;
						
						// Output pitch and roll calculations for debugging
						cout << testImages[k] << endl;
						cout << "pitch = " << pitchAngle[k] << "º" << endl;
						cout << "roll = " << rollAngle[k] << "º" << endl;
						cout << "corrected pitch = " << pitchCor[k] << "º" << endl;
						cout << "corrected roll = " << rollCor[k] << "º\n\n" << endl;
						
				}
				
				fout << "r(" << n+1 <<",:) = [";
				for (int k=5;k<10;k++){
						fout <<  " " << rollAngle[k];
				}
				fout << "];";
				
				fout << endl << "p(" << n+1 << ",:) = [";
				for (int k=0;k<5;k++){
						fout << " " << pitchAngle[k];
				}
				fout << "];" << endl;
		}	
    fout.close();
    logout.close();
}

#endif