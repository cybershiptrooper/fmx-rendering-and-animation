#ifndef _BIKE_HPP_
#define _BIKE_HPP_

#include "mesh.hpp"
#include "node.hpp"
#include "shader_util.hpp"
#include "gl_framework.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

const int SCR_W = 800;
const int SCR_H = 600;

char menu;
bool globalSwitch = 1;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  5.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

cg::Shader shaderProgram;
cg::Node* bike[28];
cg::Node* rider[34];
cg::Node* track[38];

glm::mat4 ViewProjection;
std::vector<glm::mat4> matrixStack;
bool perspectiveSwitch = 1;

void renderGL();
void initBuffersGL();

//engine
float enginePoints[] = { SECTORS, 0.3,0.3, 0.3,0.3, 0.4 };
float engine[NUM_CYLINDER];
float frustumPoints[] = {4, 0.2,0.2, 0.1,0.1, 0.9};
float frustum[NUM_PRISM(4)];

//body
float P1Points[] = {4, 0.1, 0.1, 0.4, 0.4, 0.62};
float P1[NUM_PRISM(4)];
float cubePoints[] = {0.56,0.4,0.56};
float cube[NUM_CUBOID];
float decorPoints[] ={3,0.2, 0.4,  0.0,0.0, 0.4};
float decor[NUM_PRISM(3)];
//float decorPoints[] ={0.3,0.4,0.56};
//float decor[NUM_CUBOID];
//seat
float seat1Points[] = {0.2,0.8,0.6};
float seat1[NUM_CUBOID];
float seatcurvePoints[] = {3, 0.12, 0.12, 0.12, 0.12, 0.6};
float seatcurve[NUM_PRISM(3)];
float seat2Points[] = {0.2,1.4,0.6};
float seat2[NUM_CUBOID];

//handle
float CH2Points[] = {SECTORS, 0.12,0.12, 0.12,0.12, 1.4};
float CH2[NUM_CYLINDER];
float CH1Points[] = {SECTORS, 0.07,0.07, 0.04,0.04, 0.6};
float CH1[NUM_CYLINDER];
float dialPoints[] = {SECTORS, STACKS, 0.12,0.12,0.12};
float dial[NUM_ELLIPSOID];

//exhaust
float exhaust1Points[] = { SECTORS, 0.1,0.1, 0.1,0.1, 0.6 };
float exhaust1[NUM_CYLINDER];
float exhaust2Points[] = { SECTORS, 0.12,0.12, 0.12,0.12, 0.6 };
float exhaust2[NUM_CYLINDER];
float exhaust3Points[] = { SECTORS, 0.1, 0.1, 0.1,0.1,0.01};
float exhaust3[NUM_CYLINDER];
//back wheel
float C2Points[] = { SECTORS, 0.12,0.12, 0.12,0.12, 0.6 };
float C2[NUM_CYLINDER]; 

//wheel
float wheelconnectPoints[] = {0.1, 0.7, 0.1};
float wheelConnect[NUM_CUBOID];
float wheelcylinderPoints[] = { SECTORS, 0.1,0.1, 0.1,0.1, 0.25 };
float wheelcylinder[NUM_CYLINDER];
float spokePoints[] = {SECTORS, 0.35, 0.35};
float spokes[NUM_DISC]; //!!!MAKE SPOKES SPARSE
float tyrePoints[] = {SECTORS, 0.35,0.35,0.45,0.45,  0.35,0.35,0.45,0.45, 0.1}; 
float tyre[NUM_PIPE]; //three rings instead of one?

//colours
float body[] = {0.2, 0.9, 0.9, 0.5, 0.8,0.8};
float seat[] = { 0.2,0.1,0.3,0.1,0.1,0.4};
float colour[] = {0.2, 0.2, 0.5, 0.5, 0.5,0.2};
float white[] = {0.8, 0.8, 0.8, 0.5, 0.8,0.8};
float enginecol[]={0.75,0.75, 0.75, 0.1,0.75, 0.75};
float handle_color[] = {0.9, 0.9, 0.9, 0.5, 0.8,0.8};
float black[] = {0,0,0,0,0,0};
float yellow[] = {0.0,0.5,0.5};
#endif