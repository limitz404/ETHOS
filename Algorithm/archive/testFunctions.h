//  Author: Taylor Dean
//  Email: Taylor.Dean@Colorado.Edu
//  File: testFunctions.h
//  Purpose: This file contains all of the required functions to perform the edge 
//           detection and attitude determination.
//
//  Created by Taylor Dean on 11/17/14.
//  Copyright (c) 2014 Taylor Dean. All rights reserved.
//

#ifndef TESTFUCTIONS_H
#define TESTFUCTIONS_H

#include "edgeDetection.h"
#include "dataStructures.h"

using namespace cv;
using namespace std;

void multipleImages(args argp)
{
    string testImages [] = {"/Users/taylordean/Documents/Xcode/Images/350/Adjust/p0.png",
                            "/Users/taylordean/Documents/Xcode/Images/350/Adjust/p5.png",
                            "/Users/taylordean/Documents/Xcode/Images/350/Adjust/p10.png",
                            "/Users/taylordean/Documents/Xcode/Images/350/Adjust/p15.png",
                            "/Users/taylordean/Documents/Xcode/Images/350/Adjust/p20.png",
                            "/Users/taylordean/Documents/Xcode/Images/350/Adjust/r0.png",
                            "/Users/taylordean/Documents/Xcode/Images/350/Adjust/r5.png",
                            "/Users/taylordean/Documents/Xcode/Images/350/Adjust/r10.png",
                            "/Users/taylordean/Documents/Xcode/Images/350/Adjust/r15.png",
                            "/Users/taylordean/Documents/Xcode/Images/350/Adjust/r20.png"};
    
    if (argp.visualizer == 1){
        namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    }
    for (int k=0;k<10;k++)
    {
        // Start timer
        chrono::high_resolution_clock::time_point tStart = chrono::high_resolution_clock::now();
        
        // Change constants here
        float thresholdFactor = 0.25;
        int buffer = 5;
        int subPix = 0;
        float height = argp.alt;
        float pixelPitch = 0.0000289614;
        int searchRange = 10;
        float fLength = 0.0038;
        coefficients coeff = getCoefficients(height);
        
        Mat image;
        image = imread(testImages[k], CV_LOAD_IMAGE_GRAYSCALE);   // Read the file
        
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
        
        if (argp.latency == 1){
            // ALL CALCULATIONS SHOULD OCCURE ABOVE THIS COMMENT
            // End timer
            chrono::high_resolution_clock::time_point tEnd = chrono::high_resolution_clock::now();
            
            // Calculate elapsed time
            float duration = std::chrono::duration_cast<std::chrono::microseconds>(tEnd - tStart).count();
            
            cout << 1000000/duration << " Hz" << endl;
        }
        if (argp.visualizer == 1){
            // Show edges
            Mat newImage = showEdges(image, edges);
            imshow( "Display window", newImage);                   // Show our image inside it.
            waitKey(0);
        }
        
        // Output effective rate in Hz
        cout << testImages[k] << endl;
        cout << "pitch = " << pitchAngle << "º" << endl;
        cout << "roll = " << rollAngle << "º" << endl;
        cout << "corrected pitch = " << pitchCor << "º" << endl;
        cout << "corrected roll = " << rollCor << "º\n\n" << endl;
        
    }
}

void singleImage(args argp)
{
    string testImages = argp.imPath;
    
    if (argp.visualizer == 1){
        namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    }
    // Start timer
    chrono::high_resolution_clock::time_point tStart = chrono::high_resolution_clock::now();
    
    // Change constants here
    float thresholdFactor = 0.25;
    int buffer = 5;
    int subPix = 0;
    float height = argp.alt;
    float pixelPitch = 0.0000289614;
    int searchRange = 10;
    float fLength = 0.0038;
    coefficients coeff = getCoefficients(height);
    
    Mat image;
    image = imread(testImages, CV_LOAD_IMAGE_GRAYSCALE);   // Read the file
    
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
    
    if (argp.latency == 1){
        // ALL CALCULATIONS SHOULD OCCURE ABOVE THIS COMMENT
        // End timer
        chrono::high_resolution_clock::time_point tEnd = chrono::high_resolution_clock::now();
        
        // Calculate elapsed time
        float duration = std::chrono::duration_cast<std::chrono::microseconds>(tEnd - tStart).count();
        
        cout << 1000000/duration << " Hz" << endl;
    }
    if (argp.visualizer == 1){
        // Show edges
        Mat newImage = showEdges(image, edges);
        imshow( "Display window", newImage);                   // Show our image inside it.
        waitKey(0);
    }
    
    // Output effective rate in Hz
    cout << testImages << endl;
    cout << "pitch = " << pitchAngle << "º" << endl;
    cout << "roll = " << rollAngle << "º" << endl;
    cout << "corrected pitch = " << pitchCor << "º" << endl;
    cout << "corrected roll = " << rollCor << "º\n\n" << endl;

}


#endif
