#ifndef _TRACK_HPP_
#define _TRACK_HPP_

#include "mesh.hpp"
#include "node.hpp"
#include "texture.hpp"
#include "shader_util.hpp"
#include "gl_framework.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

cg::Shader skyboxShader;
//--------------------------------------------------------------------------------------
//texture paths
// cubemap
unsigned int skyboxTexture;
const char* skybox[6] = {
	"data/px.png",
	"data/nx.png",
	"data/py.png",
	"data/ny.png",
	"data/pz.png",
	"data/nz.png"
};

//meshes
glm::mat4 scalesky;
unsigned int skyboxVAO, skyboxVBO;
float skyboxVertices[] = {
    // positions = texture coords         
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

//--------------------------------------------------------------------------------------

const int SCR_W = 800;
const int SCR_H = 600;
int keyframeNumber = 0;

char menu;
char camera;
bool animationSwitch = 0;
bool globalSwitch = 0;

float trackL1 = 1;
float trackL2 = 1;
float riderSL = 0;
float bikeHL  = 0;
glm::vec3 slPos = glm::vec3(-3.45,3,6);
glm::vec3 hlPos = glm::vec3(-1.2,0.7,6);

glm::mat4 ViewProjection;
std::vector<glm::mat4> matrixStack;

glm::vec3 cameraPos1 = glm::vec3(-2.7f, 3.0f,  20.0f);
glm::vec3 cameraPos2 = glm::vec3(-3.35f, 2.0f,  12.0f);
glm::vec3 cameraPos3 = glm::vec3(-3.45f, 1.0f,  5.9f);

glm::vec3 cameraFront1 = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraFront2 = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraFront3 = glm::vec3(0.0f, 0.0f, -1.0f);

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

cg::Shader shaderProgram;
cg::Node* track[38];
cg::Node* rider[34];
cg::Node* bike[28];

std::vector<std::vector<float>> keyFrames, allFrames;
void storeCanvas();         // storing screen as an image
void storeFrame();          // storing value in the text file
void applyFrame(int n);     // applying value from the text file
void readkeyFrames();       // reading from the text file
void interpolateFrames();   // creating bichke frames
std::vector<std::vector<float>> interpolateFramesHelper(int n);
std::vector<float> parseFrame(std::string s); // converting line from text file to float

void renderGL();
void initBuffersGL();

float Path[NUM_TRACK(5,2)];
float Pipe1[NUM_PIPED_PRISM(6)];
float jumpPlatform[NUM_CUBOID];
float jumpRing1[NUM_RING];
float CuboidStopper[NUM_CUBOID];
float FrustumSlope[NUM_CYLINDER];
float FlyingPipe[NUM_PIPE];
float TriangularBreakers[NUM_PRISM(3)];

float PathPoints[] = {5,2};
float Pipe1Points[] = {6, 0.4,0.4 ,0.5,0.5, 0.5,0.5 ,0.6,0.6, 1.2};
float jumpPlatformPoints[] = {2-4*TRACK_STEP,0.3,2*TRACK_STEP};
float jumpRing1Points[] = {SECTORS, 0.5,0.5, 0.65,0.65};
float cuboidStopperPoints[] = {0.2,0.4,1};
float FrustumSlopePoints[] = {SECTORS, 1-2*TRACK_STEP,0.5 , 0.5,0.3, 0.4};
float FlyingPipePoints[] = {SECTORS, 0.5,0.5, 0.8,0.8, 0.5,0.5, 0.8,0.8, 2};
float TriangularBreakersPoints[] = {3, 0.2,0.2, 0.2,0.2, 2-4*TRACK_STEP};

float pathColour[] = {
    0.18f, 0.18f, 0.18f,
    0.2f,0.3f,0.5f,
};
float Colour[] = {
    0.7f,0.5f,0.5f,
    0.2f,0.3f,0.5f,
};

////////////

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
float seatOne[] = { 0.2,0.1,0.3,0.1,0.1,0.4};
float colour1[] = {0.2, 0.2, 0.5, 0.5, 0.5,0.2};
float white[] = {0.8, 0.8, 0.8, 0.5, 0.8,0.8};
float enginecol[]={0.75,0.75, 0.75, 0.1,0.75, 0.75};
float handle_color[] = {0.9, 0.9, 0.9, 0.5, 0.8,0.8};
float black[] = {0,0,0,0,0,0};
float yellow[] = {0.0,0.5,0.5};

/////////////////
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

#endif