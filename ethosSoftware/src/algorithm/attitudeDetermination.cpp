//  Author: Taylor Dean
//  Email: Taylor.Dean@Colorado.Edu
//  File: attitudeDetermination.h
//  Purpose: This file contains all of the required functions to perform the edge 
//           detection and attitude determination.
//
//  Created by Taylor Dean on 11/17/14.
//  Copyright (c) 2014 Taylor Dean. All rights reserved.
//

#include <string.h>
#include <fstream>
#include "edgeDetection.h"
#include "dataStructures.h"
#include "attitudeDetermination.h"
#include "common.h"

#define NUMCOLS 162

using namespace std;

attitude determineAttitude(int image[][NUMCOLS])
{    
    attitude attitudeCalc;
    coefficients coeff = getCoefficients(ALTITUDE);
    
    pair<string, float> output = startSide(image, BUFFER);
    float threshold = output.second*THRESHOLD_FACTOR;
    
    vector<vector<int> > edges;
    
    if (output.first == "top") {
        edges = topEdges(image, threshold, SEARCH_RANGE, LOOK_FORWARD);
    }
    else if (output.first == "bottom") {
        edges = bottomEdges(image, threshold, SEARCH_RANGE, LOOK_FORWARD);
    }
    else if (output.first == "left") {
        edges = leftEdges(image, threshold, SEARCH_RANGE, LOOK_FORWARD);
    }
    else if (output.first == "right") {
        edges = rightEdges(image, threshold, SEARCH_RANGE, LOOK_FORWARD);
    }
    else {
        cout << "Did not recognize start side value" << endl;
    }
    
    lstcircle lstSqCircle;
    
    // Perform subpixel estimation if desired
    if (SUBPIX == 1) {
        vector<vector<float> > modEdges = performSubpixelEstimation(image,  edges, threshold, output.first);
        lstSqCircle = circularLeastSquaresSubPix(modEdges, NUM_COLS, NUM_ROWS);
    }
    else {
        lstSqCircle = circularLeastSquares(edges, NUM_COLS, NUM_ROWS);
    }
    
    pair<float, float> center = make_pair(lstSqCircle.xc, lstSqCircle.yc);
    
    attitudeCalc.roll = calculateRoll(center);
    attitudeCalc.pitch = calculatePitch(center, lstSqCircle.r, F_LENGTH, PIXEL_PITCH);
    attitudeCalc.rollCor = reduceRollError(attitudeCalc.roll, coeff);
    attitudeCalc.pitchCor = reducePitchError(attitudeCalc.pitch, coeff);
    
    return attitudeCalc;
}