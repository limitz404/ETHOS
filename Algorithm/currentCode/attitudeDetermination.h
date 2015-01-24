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
// End Global Variables

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
    vector<vector<float> > edgesMod;
    
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
    float pitchAngle = calculatePitch2(center,lstSqCircle.r,fLength,pixelPitch);
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
#endif