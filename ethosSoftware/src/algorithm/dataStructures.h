//  Author: Taylor Dean
//  Email: Taylor.Dean@Colorado.Edu
//  File: dataStructures.h
//  Purpose: This file contains all of the required data structures used in the edge 
//           detection and attitude determination.
//
//  Created by Taylor Dean on 11/17/14.
//  Copyright (c) 2015 Taylor Dean. All rights reserved.
//

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

struct lstcircle {
    float xc;
    float yc;
    float r;
};

struct coefficients {
    float Ra;
    float Rb;
    float Rc;
    float Pa;
    float Pb;
    float Pc;
};

struct attitude {
    float roll;
    float pitch;
    float rollCor;
    float pitchCor;
};

#endif