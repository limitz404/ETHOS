//
//  edgeDetection.h
//  EdgeDetection
//
//  Created by Taylor Dean on 11/5/14.
//  Copyright (c) 2014 Taylor Dean. All rights reserved.
//

#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include "dataStructures.h"

#define NUMCOLS 162
#define PI 3.141592653589793238462643383279502884197

// FORWARD DECLARATION OF FUNCTIONS
std::vector<std::vector<int> > topEdges(int [][NUMCOLS], float, int, int);
std::vector<std::vector<int> > bottomEdges(int [][NUMCOLS], float, int, int);
std::vector<std::vector<int> > leftEdges(int [][NUMCOLS], float, int, int);
std::vector<std::vector<int> > rightEdges(int [][NUMCOLS], float, int, int);
float minVal(std::vector<float>);
float maxVal(std::vector<float>);
std::pair<std::string, float> startSide(int [][NUMCOLS], int);
float calculateRoll(std::pair<float, float>);
lstcircle circularLeastSquares(std::vector<std::vector<int> >, float, float);
lstcircle circularLeastSquaresSubPix(std::vector<std::vector<float> >, float, float);
float subpixelEst(float, float, float);
std::vector<std::vector<float> > performSubpixelEstimation(int [][NUMCOLS], std::vector<std::vector<int> >, float, std::string);
float calculatePitch(std::pair<float, float> , float , float , float );
float reducePitchError(float, coefficients);
float reduceRollError(float, coefficients);
coefficients getCoefficients(float);

#endif