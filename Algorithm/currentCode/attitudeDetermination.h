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

#include "edgeDetection.h"
#include "dataStructures.h"
#include <string.h>
#include <chrono>

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

// End Global Variables

void determineAttitude()
{    
    cout << "DetermineAttitude" << endl;

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
    string side = output.first;
    float threshold = output.second*thresholdFactor;
    
    vector<vector<float> > edges;
    vector<vector<float> > edgesMod;
    
    if (side == "top") {
        edges = topEdges(image, threshold, searchRange);
    }
    else if (side == "bottom") {
        edges = bottomEdges(image, threshold, searchRange);
    }
    else if (side == "left") {
        edges = leftEdges(image, threshold, searchRange);
    }
    else if (side == "right") {
        edges = rightEdges(image, threshold, searchRange);
    }
    else {
        cout << "Did not recognize start side value" << endl;
    }
    
    // START STRUCT
    
    lstcircle lstSqCircle;
    
    if (subPix == 0) {
        vector<vector<float> > modEdges = performSubpixelEstimation(image,  edges, threshold, side);
        lstSqCircle = circularLeastSquares(modEdges, image.cols, image.rows);
    }
    else {
        lstSqCircle = circularLeastSquares(edges, image.cols, image.rows);
    }
    
    float xc = lstSqCircle.xc;
    float yc = lstSqCircle.yc;
    float r = lstSqCircle.r;
    
    pair<float, float> center = make_pair(xc, yc);
    
    float rollAngle = calculateRoll(center);
    float pitchAngle = calculatePitch2(center,r,fLength,pixelPitch);
    float rollCor = reduceRollError(rollAngle, coeff);
    float pitchCor = reducePitchError(pitchAngle, coeff);
    
    if (latency == 1){
        // ALL CALCULATIONS SHOULD OCCURE ABOVE THIS COMMENT
        // End timer
        chrono::high_resolution_clock::time_point tEnd = chrono::high_resolution_clock::now();
        
        // Calculate elapsed time
        float duration = std::chrono::duration_cast<std::chrono::microseconds>(tEnd - tStart).count();
        
        cout << 1000000/duration << " Hz" << endl;
    }
    if (visualizer == 1){
        // Show edges
        Mat newImage = showEdges(image, edges);
        imshow( "Display window", newImage);                   // Show our image inside it.
        waitKey(0);
    }
    
    // Output effective rate in Hz
    cout << imPath << endl;
    cout << "pitch = " << pitchAngle << "º" << endl;
    cout << "roll = " << rollAngle << "º" << endl;
    cout << "corrected pitch = " << pitchCor << "º" << endl;
    cout << "corrected roll = " << rollCor << "º\n\n" << endl;

}


#endif
