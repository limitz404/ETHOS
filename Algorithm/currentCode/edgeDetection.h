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
vector<vector<float> > topEdges(Mat, float, int);
vector<vector<float> > bottomEdges(Mat, float, int);
vector<vector<float> > leftEdges(Mat, float, int);
vector<vector<float> > rightEdges(Mat, float, int);
Mat showEdges(Mat, vector<vector<float> >);
float minVal(vector<float>);
float maxVal(vector<float>);
pair<string, float> startSide(Mat, int);
float calculateRoll(pair<float, float>);
lstcircle circularLeastSquares(vector<vector<float> >, float, float);
//tuple<float, float, float> circularLeastSquares(vector<vector<float> >, float, float);
float subpixelEst(float, float, float);
vector<vector<float> > performSubpixelEstimation(Mat, vector<vector<float> >, float);
float calculatePitch2(pair<float, float> , float , float , float );
float reducePitchError(float, coefficients);
float reduceRollError(float, coefficients);
coefficients getCoefficients(float);

// EDGE DETECTION HAPPENS HERE
// Keep in mind that the image is stored in a matrix, therefore the coordinates
// must be transformed from the matrix frame to a reference axis that intersects
// with the center of the image;
//
// Image reference frame:
// 0 1 2 3 4 5 6 7 8 9
// 1
// 2
// 3
// 4
// 5
// 6
// 7
// 8
// 9
//
// Transformed reference frame
//             4
//             3
//             2
//             1
// -4 -3 -2 -1 0 1  2  3  4
//            -1
//            -2
//            -3
//            -4

// BODY OF FILE

vector<vector<float> > topEdges(Mat image, float threshold, int searchRange, int lookForward)
{
    //  Inputs:     image       =   m x n matrix containing pixel intensities
    //              threshold   =   the 'edge' intensity value
    //
    //  Outputs:    edges       =   array containing edge locations in the image
    //                              matrix reference frame
    //
    //  Method:     Detect edges starting from the top of the image, moving down
    //              through the columns
    
    // Allocate a 2d vector to contain the edge pixel locations
    vector<vector<float> > edges;
    vector<float> row;
    edges.push_back(row);
    edges.push_back(row);
    float intensity;
    float intensity2;
    int startLoc = 0;
    
    // Scan through columns to find edge pixels
    for (int i=0; i<image.cols; i++){
        for (int j=startLoc; j<image.rows - lookForward; j++){
            intensity = image.at<uchar>(j, i);
            intensity2 = image.at<uchar>(j - lookForward,i);
            if (intensity >= threshold && intensity2 >= threshold){
                edges[0].push_back(i);
                edges[1].push_back(j);
                
                if (j > searchRange){
                    startLoc = j - searchRange;
                }
                else{
                    startLoc = 0;
                }
                
                break;
            }
        }
    }
    return edges;
}

vector<vector<float> > bottomEdges(Mat image, float threshold, int searchRange, int lookForward)
{
    //  Inputs:     image       =   m x n matrix containing pixel intensities
    //              threshold   =   the 'edge' intensity value
    //
    //  Outputs:    edges       =   array containing edge locations in the image
    //                              matrix reference frame
    //
    //  Method:     Detect edges starting from the bottom of the image, moving
    //              up through the columns.
    
    // Allocate a 2d vector to contain the edge pixel locations
    vector<vector<float> > edges;
    vector<float> row;
    edges.push_back(row);
    edges.push_back(row);
    float intensity;
    float intensity2;
    int startLoc = 0;
    
    // Scan through columns to find edge pixels
    for (int i=0; i<image.cols; i++){
        for (int j=startLoc; j<image.rows - lookForward; j++){
            intensity = image.at<uchar>(image.rows - j - 1,i);
            intensity2 = image.at<uchar>(image.rows - j - 1 - lookForward,i);
            if (intensity >= threshold && intensity2 >= threshold){
                edges[0].push_back(i);
                edges[1].push_back(image.rows - j - 1);
                
                if (j > searchRange){
                    startLoc = j - searchRange;
                }
                else{
                    startLoc = 0;
                }
                
                break;
            }
        }
    }
    return edges;
}

// Detect edges starting from the left of the image, moving right through the
// rows
vector<vector<float> > leftEdges(Mat image, float threshold, int searchRange, int lookForward)
{
    //  Inputs:     image       =   m x n matrix containing pixel intensities
    //              threshold   =   the 'edge' intensity value
    //
    //  Outputs:    edges       =   array containing edge locations in the image
    //                              matrix reference frame
    //
    //  Method:     Detect edges starting from the left of the image, moving
    //              right through the rows
    
    // Allocate a 2d vector to contain the edge pixel locations
    vector<vector<float> > edges;
    vector<float> row;
    edges.push_back(row);
    edges.push_back(row);
    float intensity;
    float intensity2;
    int startLoc = 0;
    
    // Scan through rows to find edge pixels
    for (int j=0; j<image.rows; j++){
        for (int i=startLoc; i<image.cols - lookForward; i++){
            intensity = image.at<uchar>(j, i);
            intensity2 = image.at<uchar>(j, i + lookForward);
            if (intensity >= threshold && intensity2 >= threshold){
                edges[0].push_back(i);
                edges[1].push_back(j);
                
                if (i > searchRange){
                    startLoc = i - searchRange;
                }
                else{
                    startLoc = 0;
                }
                
                break;
            }
        }
    }
    return edges;
}

vector<vector<float> > rightEdges(Mat image, float threshold, int searchRange, int lookForward)
{
    //  Inputs:     image       =   m x n matrix containing pixel intensities
    //              threshold   =   the 'edge' intensity value
    //
    //  Outputs:    edges       =   array containing edge locations in the image
    //                              matrix reference frame
    //
    //  Method:     Detect edges starting from the right of the image, moving
    //              left through the rows
    
    // Allocate a 2d vector to contain the edge pixel locations
    vector<vector<float> > edges;
    vector<float> row;
    edges.push_back(row);
    edges.push_back(row);
    float intensity;
    float intensity2;
    int startLoc = 0;
    
    // Scan through rows to find edge pixels
    for (int j=0; j<image.rows; j++){
        for (int i=startLoc; i<image.cols - lookForward; i++){
            intensity = image.at<uchar>(j, image.cols - i - 1);
            intensity2 = image.at<uchar>(j, image.cols - i - 1 - lookForward);
            if (intensity >= threshold && intensity2 >= threshold){
                edges[0].push_back(image.cols - i -1);
                edges[1].push_back(j);
                
                if (i > searchRange){
                    startLoc = i - searchRange;
                }
                else{
                    startLoc = 0;
                }
                
                break;
            }
        }
    }
    
    return edges;
}
// END EDGE DETECTION

// Modify the image to highlight the edges
Mat showEdges(Mat image, vector<vector<float> > edges)
{
    //  Inputs:     image       =   m x n matrix containing pixel intensities
    //              edges       =   array of edge locations in the image
    //                              reference frame
    //
    //  Outputs:    image       =   an copy of the original image with edges
    //                              highlighted
    //
    //  Purpose:    to enable user to display edges on image to verify
    //              functionality
    //
    //  Method:     Modify intensity values at edge locations to make them white
    
    for (int i=0; i<edges[0].size(); i++){
        image.at<uchar>(edges[1][i],edges[0][i]) = 255;
    }
    return image;
}


// Find the minimum value in a vector
float minVal(vector<float> array)
{
    //  Inputs:     array       =   vector array of values
    //
    //  Outputs:    minimum     =   minimum value in array
    //
    //  Purpose:    to find the minimum value in a vector
    
    float length = array.size();
    float minimum = array[0];
    
    for (int i=0;i<length;i++){
        if (array[i] < minimum){
            minimum = array[i];
        }
    }
    return minimum;
}

// Find the maximum value in a vector
float maxVal(vector<float> array)
{
    //  Inputs:     array       =   vector array of values
    //
    //  Outputs:    maximum     =   maximum value in array
    //
    //  Purpose:    to find the maximum value in a vector
    
    float length = array.size();
    float maximum = array[0];
    
    for (int i=0;i<length;i++){
        if (array[i] > maximum){
            maximum = array[i];
        }
    }
    return maximum;
}

// Determine which side of the image to begin searching for edges from
pair<string, float> startSide(Mat image, int bufferSize)
{
    //  Inputs:     image       =   m x n matrix containing pixel intensities
    //              bufferSize  =   number of rows or columns to be used in
    //                              finding the mean value along edges
    //
    //  Outputs:    side        =   string specifying which edge is space
    //              maxSideArray=   value of maximum mean of sides of image
    //
    //  Method:     Find the average value of each side of image to a depth of
    //              bufferSize. The side with the minimum mean is the side of
    //              space. Return the name of the side corresponding to the
    //              minimum mean intensities and the maximum mean intensity.
    //
    //  Purpose:    to determine which side of the image to begin searching for
    //              edges from;
    //
    //  To do:      change side from string to int
    
    float topMean = 0;
    float bottomMean = 0;
    float leftMean = 0;
    float rightMean = 0;
    string side = "none";
    
    for (int i=0;i<image.cols;i++){
        for (int j=0;j<bufferSize;j++){
            topMean += image.at<uchar>(j,i);
            bottomMean += image.at<uchar>(image.rows-j-1,i);
        }
    }
    for (int i=0;i<image.rows;i++){
        for (int j=0;j<bufferSize;j++){
            leftMean += image.at<uchar>(i,j);
            rightMean += image.at<uchar>(i,image.cols-j);
        }
    }
    
    topMean = topMean/image.cols/bufferSize;
    bottomMean = bottomMean/image.cols/bufferSize;
    leftMean = leftMean/image.rows/bufferSize;
    rightMean = rightMean/image.rows/bufferSize;
    
    //     vector<float> sideArray = {topMean, bottomMean, leftMean, rightMean};
    vector<float> sideArray;
    sideArray.push_back(topMean);
    sideArray.push_back(bottomMean);
    sideArray.push_back(leftMean);
    sideArray.push_back(rightMean);
    float minimum = minVal(sideArray);
    
    if (topMean == minimum) {
        side = "top";
    }
    else if (bottomMean == minimum) {
        side = "bottom";
    }
    else if (leftMean == minimum) {
        side = "left";
    }
    else if (rightMean == minimum) {
        side = "right";
    }
    else {
        cout << "Cannot determine starting edge" << endl;
    }
    
    //    float threshold = (maxVal(sideArray) - minVal(sideArray))/256.0;
    float threshold = maxVal(sideArray);
    
    return make_pair(side, threshold );
}

float calculateRoll(pair<float, float> center)
{
    //  Inputs:     center      =   a float pair specifying the center of the
    //                              circle found by least squares method. Must
    //                              be in the transformed reference frame.
    //                              (xc, yc)
    //
    //  Outputs:    rollAngle   =   roll angle in degrees
    //
    //  Purpose:    to determine the roll angle of the image
    //
    //  To Do:      currently outputting in degrees. Should eventually change
    //              this to radians
    
    float rollAngle = -atan2(center.first, -center.second)*180/PI;
    return rollAngle;
}

lstcircle circularLeastSquares(vector<vector<float> > edges, float width, float height)
//tuple<float, float, float> circularLeastSquares(vector<vector<float> > edges, float width, float height)
{
    //  Inputs:     edges       =   vector array of edges in image reference
    //                              frame
    //              width       =   number of columns in image matrix
    //              height      =   number of rows in image matrix
    //
    //  Outputs:    xc          =   x-coordinate of center of least squares
    //                              circle in transformed reference frame
    //              yc          =   y-coordinate of center of least squares
    //                              circle in transformed reference frame
    //              r           =   radius of least squares circle
    //
    //  Method:     Least squares
    //
    //  Purpose:    to determine the coordinates of the best fit circle for the
    //              edges in the transformed reference frame
    
    Mat a, b, c;
    a = Mat::zeros(3,3,CV_64F);
    b = Mat::zeros(3,1,CV_64F);
    float xi = 0;
    float yi = 0;
    float xbar = 0;
    float ybar = 0;
    float xc = 0;
    float yc = 0;
    float ui = 0;
    float vi = 0;
    float uc = 0;
    float vc = 0;
    float Suu = 0;
    float Suv = 0;
    float Suuu = 0;
    float Suvv = 0;
    float Svv = 0;
    float Svvv = 0;
    float Svuu = 0;
    float r;
    
    for (int i=0;i<edges[0].size();i++) {
        // Need to add 0.5 if edges[0].size() is even
        if (edges[0].size() % 2) {
            xi = xi + edges[0][i] - width*0.5;
            yi = yi + height*0.5 - edges[1][i];
        }
        else {
            xi = xi + edges[0][i] - width*0.5 + 0.5;
            yi = yi + height*0.5  - 0.5 - edges[1][i];
        }
    }
    xbar = xi/edges[0].size();
    ybar = yi/edges[1].size();
    
    for (int i=0;i<edges[0].size();i++) {
        // Need to add 0.5 if edges[0].size() is even
        if (edges[0].size() % 2) {
            xi = edges[0][i] - width*0.5;
            yi = height*0.5 - edges[1][i];
        }
        else {
            xi = edges[0][i] - width*0.5 + 0.5;
            yi = height*0.5  - 0.5 - edges[1][i];
        }
        ui = xi - xbar;
        vi = yi - ybar;
        Suu = Suu + ui*ui;
        Suv = Suv + ui*vi;
        Svv = Svv + vi*vi;
        Suuu = Suuu + ui*ui*ui;
        Suvv = Suvv + ui*vi*vi;
        Svvv = Svvv + vi*vi*vi;
        Svuu = Svuu + vi*ui*ui;
        
    }
    
    vc = (0.5*(Suuu + Suvv) - ((Svvv + Svuu)*Suu)/2/Suv)/(Suv - Svv*Suu/Suv);
    uc = (0.5*(Svvv + Svuu) - vc*Svv)/Suv;
    
    xc = uc + xbar;
    yc = vc + ybar;
    
    r = sqrt(uc*uc + vc*vc + (Suu + Svv)/edges[0].size());
    
    lstcircle c1;
    c1.r = r;
    c1.xc = xc;
    c1.yc = yc;
    
    return c1;
//    return make_tuple(xc, yc, r);
}

float subpixelEst(float int1, float intensity2, float threshold)
{
    //  Inputs:     int1        =   intensity before edge pixel
    //              intensity2        =   intensity after edge pixel
    //              threshold   =   threshold for edge
    //
    //  Outputs:    fractX      =   fractional location withing edge pixel where
    //                              edge begins
    //
    //  Method:     Linear interpolation between int1 and int3
    //
    //  Purpose:    to determine the fractional location of edge within edge
    //              pixel
    
    float fractX = (threshold - int1)*3/(intensity2 - int1) - 1;
    return fractX;
}

// This is a first attempt at subpixel estimation
vector<vector<float> > performSubpixelEstimation(Mat image, vector<vector<float> > edges, float threshold, string side)
{
    vector<vector<float> > modEdges;
    vector<float> row;
    modEdges.push_back(row);
    modEdges.push_back(row);
    float int1;
    float intensity2;
    
    if (side == "top") {
        for (int i=0; i<edges[0].size(); i++) {
            int1 = image.at<uchar>(edges[1][i]-1,edges[0][i]);
            intensity2 = image.at<uchar>(edges[1][i],edges[0][i]);
            modEdges[1].push_back(edges[1][i] + subpixelEst(int1, intensity2, threshold));
            modEdges[0].push_back(edges[0][i]);
        }
    }
    else if (side == "bottom") {
        for (int i=0; i<edges[0].size(); i++) {
            int1 = image.at<uchar>(edges[1][i]+1,edges[0][i]);
            intensity2 = image.at<uchar>(edges[1][i],edges[0][i]);
            modEdges[1].push_back(edges[1][i] - subpixelEst(int1, intensity2, threshold));
            modEdges[0].push_back(edges[0][i]);
        }
    }
    else if (side == "left") {
        for (int i=0; i<edges[0].size(); i++) {
            int1 = image.at<uchar>(edges[1][i],edges[0][i]-1);
            intensity2 = image.at<uchar>(edges[1][i],edges[0][i]);
            modEdges[1].push_back(edges[1][i]);
            modEdges[0].push_back(edges[0][i] + subpixelEst(int1, intensity2, threshold));
        }
    }
    else if (side == "right") {
        for (int i=0; i<edges[0].size(); i++) {
            int1 = image.at<uchar>(edges[1][i],edges[0][i]+1);
            intensity2 = image.at<uchar>(edges[1][i],edges[0][i]);
            modEdges[1].push_back(edges[1][i]);
            modEdges[0].push_back(edges[0][i] - subpixelEst(int1, intensity2, threshold));
        }
    }
    return modEdges;
}

float calculatePitch2(pair<float, float> center, float R, float fLength, float pixelPitch)
{
    //  Inputs:     center      =   a float pair specifying the center of the
    //                              circle found by least squares method. Must
    //                              be in the transformed reference frame.
    //                              (xc, yc)
    //
    //  Outputs:    pitchAngle  =   pitch angle in degrees
    //
    //  Purpose:    to determine the pitch angle of the image
    //
    //  To Do:      currently outputting in degrees. Should eventually change
    //              this to radians
    float h = (sqrt(center.first*center.first + center.second*center.second) - R)*pixelPitch;
    float pitchAngle = atan2(h, fLength)*180/PI;
    return pitchAngle;
}

float reducePitchError(float pitch, coefficients coeff)
{
    float pitchCor = pitch*coeff.Pa + coeff.Pb;
    return pitchCor;
}

float reduceRollError(float roll, coefficients coeff)
{
    float rollCor = roll*coeff.Ra + coeff.Rb;
    return rollCor;
}

coefficients getCoefficients(float height)
{
    coefficients coeffs;
    
    coeffs.Pa = 0.000016047790803*height + 1.000845684214970;
    coeffs.Pb = -0.000260519527204*height + 0.004200564089650;
    coeffs.Ra = -0.000166626504366*height + 1.056448694368721;
    coeffs.Rb = -0.000321027334648*height + 0.093219958268063;
    
    return coeffs;
}

#endif