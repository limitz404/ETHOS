//
//  edgeDetection.h
//  EdgeDetection
//
//  Created by Taylor Dean on 11/5/14.
//  Copyright (c) 2014 Taylor Dean. All rights reserved.
//

#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include "dataStructures.h"

using namespace cv;
using namespace std;

#define PI 3.141592653589793238462643383279502884197

// FORWARD DECLARATION OF FUNCTIONS
vector<vector<float> > topEdges(Mat, float, int, int);
vector<vector<float> > bottomEdges(Mat, float, int, int);
vector<vector<float> > leftEdges(Mat, float, int, int);
vector<vector<float> > rightEdges(Mat, float, int, int);
Mat showEdges(Mat, vector<vector<float> >);
float minVal(vector<float>);
float maxVal(vector<float>);
pair<string, float> startSide(Mat, int);
float calculateRoll(pair<float, float>);
lstcircle circularLeastSquares(vector<vector<float> >, float, float);
float subpixelEst(float, float, float);
vector<vector<float> > performSubpixelEstimation(Mat, vector<vector<float> >, float, string);
float calculatePitch2(pair<float, float> , float , float , float );
float reducePitchError(float, coefficients);
float reduceRollError(float, coefficients);
coefficients getCoefficients(float);

#endif