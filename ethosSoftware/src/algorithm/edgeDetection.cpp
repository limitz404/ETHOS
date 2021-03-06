//
//  edgeDetection.h
//  EdgeDetection
//
//  Created by Taylor Dean on 11/5/14.
//  Copyright (c) 2014 Taylor Dean. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "dataStructures.h"
#include "edgeDetection.h"
#include "common.h"

using namespace std;

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

vector<vector<int> > topEdges(int image[][NUMCOLS], float threshold, int searchRange, int lookForward)
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
    vector<vector<int> > edges;
    vector<int> row;
    edges.push_back(row);
    edges.push_back(row);
    int intensity;
    int intensity2;
    int startLoc = 0;
    int jTemp;
    
    // Scan through columns to find edge pixels
    for (int i=0; i<NUM_COLS; i++){
        for (int j=startLoc; j<NUM_ROWS; j++){
            jTemp = j;
            intensity = image[j][i];
            
            if (j < NUM_ROWS - lookForward){
                    intensity2 = image[j - lookForward][i];
            }
            else{
                    intensity2 = intensity;
            }
            
            if (intensity >= threshold && intensity2 >= threshold){
                edges[0].push_back(i);
                edges[1].push_back(j);
                image[j][i] = 0;                
                if (j > searchRange){
                    startLoc = j - searchRange;
                }
                else{
                    startLoc = 0;
                }
                
                break;
            }
        }
        
        if (jTemp == NUM_ROWS - 1){
                startLoc = 0;
        }
    }
    return edges;
}

vector<vector<int> > bottomEdges(int image[][NUMCOLS], float threshold, int searchRange, int lookForward)
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
    vector<vector<int> > edges;
    vector<int> row;
    edges.push_back(row);
    edges.push_back(row);
    int intensity;
    int intensity2;
    int startLoc = 0;
    int jTemp;
    
    // Scan through columns to find edge pixels
    for (int i=0; i<NUM_COLS; i++){
        for (int j=startLoc; j<NUM_ROWS; j++){
            jTemp = j;
            intensity = image[NUM_ROWS - j - 1][i];
            
            if (j < NUM_ROWS - lookForward){
                    intensity2 = image[NUM_ROWS - j - 1 - lookForward][i];
            }
            else{
                    intensity2 = intensity;
            }
            
            if (intensity >= threshold && intensity2 >= threshold){
                edges[0].push_back(i);
                edges[1].push_back(NUM_ROWS - j - 1);
                image[NUM_ROWS-j-1][i] = 0; 
                if (j > searchRange){
                    startLoc = j - searchRange;
                }
                else{
                    startLoc = 0;
                }
                
                break;
            }
        }
        
        if (jTemp == NUM_ROWS - 1){
                startLoc = 0;
        }
    }
    return edges;
}

// Detect edges starting from the left of the image, moving right through the
// rows
vector<vector<int> > leftEdges(int image[][NUMCOLS], float threshold, int searchRange, int lookForward)
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
    vector<vector<int> > edges;
    vector<int> row;
    edges.push_back(row);
    edges.push_back(row);
    int intensity;
    int intensity2;
    int startLoc = 0;
    int iTemp;
    
    // Scan through rows to find edge pixels
    for (int j=0; j<NUM_ROWS; j++){
        for (int i=startLoc; i<NUM_COLS; i++){
            iTemp = i;
            intensity = image[j][i];
            
            if (i < NUM_COLS - lookForward){
                    intensity2 = image[j][i + lookForward];
            }
            else{
                    intensity2 = intensity;
            }
            
            if (intensity >= threshold && intensity2 >= threshold){
                edges[0].push_back(i);
                edges[1].push_back(j);
                image[j][i] = 0;
                if (i > searchRange){
                    startLoc = i - searchRange;
                }
                else{
                    startLoc = 0;
                }
                
                break;
            }
        }
        
        if (iTemp == NUM_COLS - 1){
                startLoc = 0;
        }
    }
    return edges;
}

vector<vector<int> > rightEdges(int image[][NUMCOLS], float threshold, int searchRange, int lookForward)
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
    vector<vector<int> > edges;
    vector<int> row;
    edges.push_back(row);
    edges.push_back(row);
    int intensity;
    int intensity2;
    int startLoc = 0;
    int iTemp;
    
    // Scan through rows to find edge pixels
    for (int j=0; j<NUM_ROWS; j++){
        for (int i=startLoc; i<NUM_COLS; i++){
                iTemp = i;
            intensity = image[j][NUM_COLS - i - 1];
            
            if (i < NUM_COLS - lookForward){
                    intensity2 = image[j][NUM_COLS - i - 1 - lookForward];
            }
            else{
                    intensity2 = intensity;
            }
            
            if (intensity >= threshold && intensity2 >= threshold){
                edges[0].push_back(NUM_COLS - i -1);
                edges[1].push_back(j);
                image[j][NUM_COLS-i-1] = 0;
                if (i > searchRange){
                    startLoc = i - searchRange;
                }
                else{
                    startLoc = 0;
                }
                
                break;
            }
        }
        
        if (iTemp == NUM_COLS - 1){
                startLoc = 0;
        }
    }
    
    return edges;
}
// END EDGE DETECTION

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
pair<string, float> startSide(int image[][NUMCOLS], int bufferSize)
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
    
    for (int i=0;i<NUM_COLS;i++){
        for (int j=0;j<bufferSize;j++){
            topMean += image[j][i];
            bottomMean += image[NUM_ROWS-j-1][i];
        }
    }
    for (int i=0;i<NUM_ROWS;i++){
        for (int j=0;j<bufferSize;j++){
            leftMean += image[i][j];
            rightMean += image[i][NUM_COLS-j-1];
        }
    }
    
    topMean = topMean/(NUM_COLS*bufferSize);
    bottomMean = bottomMean/(NUM_COLS*bufferSize);
    leftMean = leftMean/(NUM_ROWS*bufferSize);
    rightMean = rightMean/(NUM_ROWS*bufferSize);
    
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

lstcircle circularLeastSquares(vector<vector<int> > edges, float width, float height)
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
        
    int upperBound = edges[0].size() - 1;
    
    for (int i=0;i<upperBound;i++) {
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
    
    for (int i=0;i<upperBound;i++) {
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
    
    r = sqrt(uc*uc + vc*vc + (Suu + Svv)/upperBound);
    
    lstcircle c1;
    c1.r = r;
    c1.xc = xc;
    c1.yc = yc;
    
    return c1;
}

lstcircle circularLeastSquaresSubPix(vector<vector<float> > edges, float width, float height)
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
        
    int upperBound = edges[0].size() - 1;
    
    for (int i=0;i<upperBound;i++) {
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
    
    for (int i=0;i<upperBound;i++) {
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
    
    r = sqrt(uc*uc + vc*vc + (Suu + Svv)/upperBound);
    
    lstcircle c1;
    c1.r = r;
    c1.xc = xc;
    c1.yc = yc;
    
    return c1;
}

float subpixelEst(float intensity1, float intensity2, float threshold)
{
    //  Inputs:     intensity1        =   intensity before edge pixel
    //              intensity2        =   intensity after edge pixel
    //              threshold   =   threshold for edge
    //
    //  Outputs:    fractX      =   fractional location withing edge pixel where
    //                              edge begins
    //
    //  Method:     Linear interpolation between intensity1 and int3
    //
    //  Purpose:    to determine the fractional location of edge within edge
    //              pixel
    
    float fractX = (threshold - intensity1)*3/(intensity2 - intensity1) - 1;
    return fractX;
}

// This is a first attempt at subpixel estimation
vector<vector<float> > performSubpixelEstimation(int image[][NUMCOLS], vector<vector<int> > edges, float threshold, string side)
{
    vector<vector<float> > modEdges;
    vector<float> row;
    modEdges.push_back(row);
    modEdges.push_back(row);
    float intensity1;
    float intensity2;
    
    if (side == "top") {
        for (int i=0; i<edges[0].size(); i++) {
            intensity1 = image[edges[1][i]-1][edges[0][i]];
            intensity2 = image[edges[1][i]][edges[0][i]];
            modEdges[1].push_back(edges[1][i] + subpixelEst(intensity1, intensity2, threshold));
            modEdges[0].push_back(edges[0][i]);
        }
    }
    else if (side == "bottom") {
        for (int i=0; i<edges[0].size(); i++) {
            intensity1 = image[edges[1][i]+1][edges[0][i]];
            intensity2 = image[edges[1][i]][edges[0][i]];
            modEdges[1].push_back(edges[1][i] - subpixelEst(intensity1, intensity2, threshold));
            modEdges[0].push_back(edges[0][i]);
        }
    }
    else if (side == "left") {
        for (int i=0; i<edges[0].size(); i++) {
            intensity1 = image[edges[1][i]][edges[0][i]-1];
            intensity2 = image[edges[1][i]][edges[0][i]];
            modEdges[1].push_back(edges[1][i]);
            modEdges[0].push_back(edges[0][i] + subpixelEst(intensity1, intensity2, threshold));
        }
    }
    else if (side == "right") {
        for (int i=0; i<edges[0].size(); i++) {
            intensity1 = image[edges[1][i]][edges[0][i]+1];
            intensity2 = image[edges[1][i]][edges[0][i]];
            modEdges[1].push_back(edges[1][i]);
            modEdges[0].push_back(edges[0][i] - subpixelEst(intensity1, intensity2, threshold));
        }
    }
    return modEdges;
}

float calculatePitch(pair<float, float> center, float R, float fLength, float pixelPitch)
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
        //  Inputs:     pitch      =   a float containing the raw pitch angle in degrees
    //
    //  Outputs:    pitchCor   =   corrected pitch angle in degrees
    //
    //  Purpose:    to correct the pitch angle
    //
    //  To Do:      Not currently working well. Debug
    
    float pitchCor = pitch*pitch*coeff.Pa + pitch*coeff.Pb + coeff.Pc;
    return pitchCor;
}

float reduceRollError(float roll, coefficients coeff)
{
        //  Inputs:     roll      =   a float containing the raw roll angle in degrees
    //
    //  Outputs:    rollCor   =   corrected roll angle in degrees
    //
    //  Purpose:    to correct the pitch angle
    //
    //  To Do:      Not currently working well. Debug
    
    float rollCor = roll*roll*coeff.Ra + roll*coeff.Rb + coeff.Rc;
    return rollCor;
}

coefficients getCoefficients(float height)
{
        //  Inputs:     height    =   a float containing height of the sensor
    //
    //  Outputs:    coeffs    =   a coefficient structure containing the
    //                                                        error correction polynomial coeffiients
    //
    //  Purpose:    to correct determine the correction coefficients for a
    //                            given altitude
    //
    //  To Do:      Not currently working well. Debug
    
    coefficients coeffs;
    
    coeffs.Pa = -0.000000934515451134*height + -0.000100992634531045;
    coeffs.Pb = 0.000018835792135*height + 1.229688828883790;
    coeffs.Pc = -0.000139687646810*height + -2.263872473634770;
    coeffs.Ra = 0.000000344122178389*height + -0.000174147348325762;
    coeffs.Rb = -0.000120468449106*height + 1.050698358643706;
    coeffs.Rc = -0.000030000373588*height + 0.018288177661476;
    
    return coeffs;
}
