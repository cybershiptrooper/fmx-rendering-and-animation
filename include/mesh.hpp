#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "node.hpp"
#include <vector>
#include <math.h>

//MESH UTILS
void generatePrismVertices      (float* givenPoints, float* givenColours, float* vertices);
void generateDiscVertices       (float* givenPoints, float* givenColours, float* vertices);
void generateCuboidVertices     (float* givenPoints, float* givenColours, float* vertices);
void generateEllipsoidVertices  (float* givenPoints, float* givenColours, float* vertices);
void generateRingVertices       (float* givenPoints, float* givenColours, float* vertices);
void generatePipeVertices       (float* givenPoints, float* givenColours, float* vertices);
void generateTrackVertices      (float* givenPoints, float* givenColours, float* vertices);

//NUMBER OF POINTS IN MESH AND HELPERS
#define   NUM_NODES(POINT) (POINT/11)
#define   NUM_PRISM(SIDES) (SIDES*3*2*11 + SIDES*6*11)
#define   NUM_PIPED_PRISM(SIDES) (SIDES*4*6*11)

#define SECTORS        60
#define STACKS         60
#define NUM_CYLINDER   NUM_PRISM(SECTORS)
#define NUM_DISC       SECTORS*3*11
#define NUM_CUBOID     6*6*11
#define NUM_ELLIPSOID  SECTORS*2*3*11 + SECTORS*(STACKS-2)*6*11
#define NUM_RING       SECTORS*6*11
#define NUM_PIPE       SECTORS*4*6*11

#define TRACK_STEP     0.02
#define FIND_NUM_TRACK(L,W) (int)(((L*W + L*TRACK_STEP*2 + W*TRACK_STEP*2)/(TRACK_STEP*TRACK_STEP) + 1)*6*2*11)
#define NUM_TRACK(L,W)      FIND_NUM_TRACK(L,W) 

/*
DATA DECLARATIONS

For each shape we have 3 basic arrays
    1. Mesh vertices
    2. Scaling Parameters
    3. Colour

* Size of the mesh vertices depends on the shape
* Form of the Scaling Parameters array is different for shapes, but quite easy to understand
* Then colours are personal

PRISM/CYLINDER      :   Sectors(Sides), BottomRadius(X,Y), TopRadius(X,Y), Height
DISC /ELLIPSE       :   Sectors(Sides), RadiusAlongX, RadiusAlongY
SPHERE/ELLIPSOID    :   Sectors(Sides), Stacks, RadiusAlongX, RadiusAlongY, RadiusAlongZ
RING                :   Sectors(Sides), InnerRadius(X,Y), OuterRadius(X,Y)
PIPE                :   Sectors(Sides), Bottom_InnerRadius(X,Y), Bottom_OuterRadius(X,Y), Top_InnerRadius(X,Y), Top_OuterRadius(X,Y), Height
CUBE /CUBOID        :   ScalingFactors along X,Y,Z

TRACK               :   Length, Width

** Figure out a way to convert this data into a mesh object? **
*/

#endif



