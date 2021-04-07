#ifndef _RIDER_HPP_
#define _RIDER_HPP_

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

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

cg::Shader shaderProgram;
cg::Node* rider[34];
cg::Node* track[38];
cg::Node* bike[28];

glm::mat4 ViewProjection;
std::vector<glm::mat4> matrixStack;
bool perspectiveSwitch = 1;

void renderGL();
void initBuffersGL();

//root
float hipPoints[] = { SECTORS, 0.2,0.1, 0.25,0.18, 0.3 };
float hip[NUM_CYLINDER];

//upper body
float bendjointPoints[] = {SECTORS, STACKS, 0.25, 0.18, 0.18};
float bendjoint[NUM_ELLIPSOID];
float f2Points[] = { SECTORS, 0.25,0.185, 0.4,0.16, 0.8 };
float f2[NUM_CYLINDER];
float f1Points[] = { SECTORS, 0.4,0.16, 0.1,0.1, 0.2 };
float f1[NUM_CYLINDER];
float C1Points[] = { SECTORS, STACKS, 0.1, 0.1, 0.1 };
float C1[NUM_ELLIPSOID];
float E1Points[] = { SECTORS, STACKS, 0.2, 0.2, 0.3};
float E1[NUM_ELLIPSOID];
float S1Points[] = {SECTORS, STACKS, 0.1,0.1,0.1};
float S1[NUM_ELLIPSOID];
float A1Points[] = { SECTORS, 0.07,0.07, 0.1,0.1, 0.4 };
float A1[NUM_CYLINDER];
float S2Points[] = {SECTORS, STACKS, 0.08,0.08,0.1};
float S2[NUM_ELLIPSOID];
float A2Points[] = { SECTORS, 0.04,0.04, 0.07,0.07, 0.36};
float A2[NUM_CYLINDER];
//hat
float pipePoints[] = {SECTORS/10, 0.1,0.1, 0.5,0.5, 0.1,0.1, 0.5,0.5, 0.05};
float pipe[NUM_PIPE/10];
float hPoints[] =  {4, 0.32, 0.32, 0.2,0.2, 0.23};
float h[NUM_PRISM(4)];
float nosePoints[] = {0.01,0.1,0.05};
float nose[NUM_CUBOID];
float eyePoints[] = {0.05,0.01,0.01};
float eyes[NUM_CUBOID];
//lower body
float mainbuttPoints[] = {SECTORS,STACKS,0.2, 0.12, 0.1 };
float main_butt[NUM_ELLIPSOID];
float buttPoints[] = { SECTORS,STACKS,0.1, 0.1, 0.1 };
float butt[NUM_ELLIPSOID];
float thighPoints[] = { SECTORS, 0.07,0.07, 0.125,0.1, 0.45 };
float thigh[NUM_CYLINDER];
float S3Points[] = { SECTORS, STACKS, 0.07, 0.07, 0.07 };
float S3[NUM_ELLIPSOID];
float shinPoints[] = { SECTORS, 0.04,0.04, 0.07,0.07, 0.45};
float shin[NUM_CYLINDER];
float handPoints[] = { SECTORS, STACKS, 0.07, 0.07, 0.07 };
float hand[NUM_ELLIPSOID];
float shoes[] = {0.7,0.7,0.2,0.2,0.5,0.5};
float anklePoints[] = {SECTORS, STACKS, 0.07, 0.07, 0.07};
float ankle[NUM_ELLIPSOID];
float feetPoints[] = {4, 0.08,0.08,0.07,0.07, 0.25};
float feet[NUM_PRISM(4)];

//colour
float shirt[] = {0.3, 0.9,  0.8,0.8, 0.9, 0.5};
float shirt2[] = {0.6,0.5,0.6,0.5,0.5,0.5};
float pants[] = {0,0,0,0,0,0};
float colour[] = {0.7f,0.5f,0.5f,0.2f,0.3f,0.5f};
float skin[] = {0.5,0.5,0.5,0.5,0.5,0.3};
float hat[] = {0.2, 0.9,  0.8,0.8, 0.9, 0.5};
float seat[] = { 0.2,0.1,0.3,0.4,0.1,0.1};
// float colour[]={0.4,0.5,0.5,0.6,0.6,0.5};
#endif