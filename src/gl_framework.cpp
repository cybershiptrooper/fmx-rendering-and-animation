#include "gl_framework.hpp"
#include "node.hpp"

extern glm::vec3 cameraPos1;
extern glm::vec3 cameraPos2;
extern glm::vec3 cameraPos3;
extern glm::vec3 cameraFront1;
extern glm::vec3 cameraFront2;
extern glm::vec3 cameraFront3;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern float deltaTime;
extern float lastFrame;

extern bool animationSwitch;
extern bool globalSwitch;
extern char menu;
extern char camera;

extern float trackL1;
extern float trackL2;
extern float riderSL;
extern float bikeHL;
extern glm::vec3 slPos;
extern glm::vec3 hlPos;

extern void storeFrame();
extern void readkeyFrame();

extern cg::Node* bike[28];
extern cg::Node* rider[34];
extern cg::Node* track[38];
cg::Node* currentNode;

namespace cg
{
    void initGL(void)
    {
        //Set framebuffer clear color
        glClearColor(0.15f, 0.15f, 0.15f, 0.1f);
        //Set depth buffer furthest depth
        glClearDepth(1.0);
        //Set depth test to less-than
        glDepthFunc(GL_LESS);
        //Enable depth testing
        glEnable(GL_DEPTH_TEST);
    }

    void error_callback(int error, const char* desc)
    {
        std::cerr << desc << std::endl;
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0,0,width,height);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window,GLFW_TRUE);
        }
        if(key == GLFW_KEY_1 && action == GLFW_PRESS && !globalSwitch)
        {
            trackL1 = -1 * trackL1 + 1;
        }
        if(key == GLFW_KEY_2 && action == GLFW_PRESS && !globalSwitch)
        {
            trackL2 = -1 * trackL2 + 1;
        }
        if(key == GLFW_KEY_3 && action == GLFW_PRESS && !globalSwitch)
        {
            riderSL = -1 * riderSL + 1;
        }
        if(key == GLFW_KEY_4 && action == GLFW_PRESS && !globalSwitch)
        {
            bikeHL = -1 * bikeHL + 1;
        }
        if(key == GLFW_KEY_C && action == GLFW_PRESS && !globalSwitch)
        {
            if(camera == '1')
            {
                camera = '2';
                std::cout << "Camera 2 : 3rd Person\n";  
            }
            else if(camera == '2')
            {
                camera = '3';
                std::cout << "Camera 3 : POV Camera\n"; 
            }
            else if(camera == '3')
            {
                camera = '1';
                std::cout << "Camera 1 : Full View\n";   
            }
        }
        if(key == GLFW_KEY_V && action == GLFW_PRESS && globalSwitch)
        {
            if(menu == 'R')
            {
                menu = 'B';
                std::cout << "Now you can control the Bike\n";
            }
            else
            {
                menu = 'R';
                std::cout << "Now you can control the Rider\n";
            }
        }
        if(key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            storeFrame();
        }
        if(key == GLFW_KEY_X && action == GLFW_PRESS)
        {
            if(!remove("./data/keyframes.txt"))
                std::cout << "keyframes.txt deleted successfully\n";
            else
                std::cout << "keyframes.txt does not exist\n";
        }
        if(key == GLFW_KEY_L && action == GLFW_PRESS)
        {
            readkeyFrame();
        }
        if(key == GLFW_KEY_P && action == GLFW_PRESS)
        {
            animationSwitch = !animationSwitch;
        }
        if(key == GLFW_KEY_Z && action == GLFW_PRESS)
        {
            globalSwitch = !globalSwitch;
            if(globalSwitch)
                std::cout << "Press V to toggle control between Rider and Bike\n";
            else
                std::cout << "Use RDFG and Arrow Keys to control the camera.\nPress C to toggle between cameras.\n";            
        }

        float cameraSpeed = 20 * deltaTime;
        switch(camera)
        {
            case '1':

            if(!globalSwitch)
            {
                if(key == GLFW_KEY_G && action == GLFW_PRESS)
                {
                    cameraPos1 += glm::normalize(glm::cross(cameraFront1, cameraUp)) * cameraSpeed;
                }
                if(key == GLFW_KEY_D && action == GLFW_PRESS)
                {
                    cameraPos1 -= glm::normalize(glm::cross(cameraFront1, cameraUp)) * cameraSpeed;
                }
                if(key == GLFW_KEY_R && action == GLFW_PRESS)
                {
                    cameraPos1 += cameraSpeed * cameraFront1;
                }
                if(key == GLFW_KEY_F && action == GLFW_PRESS)
                {
                    cameraPos1 -= cameraSpeed * cameraFront1;
                }
                if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
                {
                    cameraFront1.x -= 0.05;
                }
                if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
                {
                    cameraFront1.x += 0.05;
                }
                if(key == GLFW_KEY_UP && action == GLFW_PRESS)
                {
                    cameraFront1.y += 0.05;
                }
                if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
                {
                    cameraFront1.y -= 0.05;
                } 
            } 

            else
            {
                controlModels(key,action);
            }
            break;

            case '2': case '3':

            if(!globalSwitch)
            {
                if(key == GLFW_KEY_G && action == GLFW_PRESS)
                {
                    cameraPos2 += glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), cameraUp)) * cameraSpeed;
                    cameraPos3 += glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), cameraUp)) * cameraSpeed;
                    slPos.x += cameraSpeed;
                    rider[0]->addTZ(-cameraSpeed);
                    
                }
                if(key == GLFW_KEY_D && action == GLFW_PRESS)
                {
                    cameraPos2 -= glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), cameraUp)) * cameraSpeed;
                    cameraPos3 -= glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), cameraUp)) * cameraSpeed;
                    rider[0]->addTZ(cameraSpeed);
                    slPos.x -= cameraSpeed;
                }
                if(key == GLFW_KEY_R && action == GLFW_PRESS)
                {
                    cameraPos2 += cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f);
                    cameraPos3 += cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f);
                    rider[0]->addTX(-cameraSpeed);
                    slPos.z -= cameraSpeed;
                }
                if(key == GLFW_KEY_F && action == GLFW_PRESS)
                {
                    cameraPos2 -= cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f);
                    cameraPos3 -= cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f);
                    rider[0]->addTX(cameraSpeed);
                    slPos.z += cameraSpeed;
                }
                if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
                {
                    cameraFront2.x -= 0.01;
                    cameraFront2.z += 0.01;
                    cameraFront3.x -= 0.01;
                    cameraFront3.z += 0.01;
                    
                    rider[0]->addRY(0.2);
                }
                if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
                {
                    cameraFront2.x += 0.01;
                    cameraFront2.z -= 0.01;
                    cameraFront3.x += 0.01;
                    cameraFront3.z -= 0.01;
                    
                    rider[0]->addRY(-0.2);
                }
                if(key == GLFW_KEY_UP && action == GLFW_PRESS)
                {
                    cameraFront2.y += 0.05;
                    cameraFront3.y += 0.05;
                }
                if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
                {
                    cameraFront2.y -= 0.05;
                    cameraFront3.y -= 0.05;
                } 
            }

            else
            {
                controlModels(key,action);
            }
            break;
        }
    }

    void controlModels(int key, int action)
    {
        switch (menu)
        {
        case 'B':
            if(key == GLFW_KEY_SLASH && action == GLFW_PRESS){
                currentNode = bike[0];
            }
            if(key == GLFW_KEY_1 && action == GLFW_PRESS){
                currentNode = bike[18];
            }
            if(key == GLFW_KEY_2 && action == GLFW_PRESS){
                currentNode = bike[21];//front wheel
            }
            if(key == GLFW_KEY_3 && action == GLFW_PRESS){
                currentNode = bike[6];//back wheel
            }

            if(currentNode == bike[0])
            {
                if(key == GLFW_KEY_G && action == GLFW_PRESS){
                    bike[0]->addTX(-0.5);
                }
                if(key == GLFW_KEY_D && action == GLFW_PRESS){
                    bike[0]->addTX(0.5);
                }
                if(key == GLFW_KEY_R && action == GLFW_PRESS){
                    bike[0]->addTY(-0.5);
                }
                if(key == GLFW_KEY_F && action == GLFW_PRESS){
                    bike[0]->addTY(0.5);
                }
                if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
                    bike[0]->addRZ(3);
                }
                if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
                    bike[0]->addRZ(-3);
                }
                if(key == GLFW_KEY_UP && action == GLFW_PRESS){
                    bike[0]->addRX(3);
                }
                if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
                    bike[0]->addRX(-3);
                }  
            }
            else
            {
                if(key == GLFW_KEY_UP && action == GLFW_PRESS){
                    if(currentNode==bike[18])
                        currentNode->addRX(3);
                    else
                        currentNode->addRY(3);
                }
                if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
                    if(currentNode==bike[18])
                        currentNode->addRX(-3);
                    else
                        currentNode->addRY(-3);
                } 
            }
            break;

        case 'R':
            if(key == GLFW_KEY_1 && action == GLFW_PRESS){
                     
                currentNode = rider[28];//torso
            }
            if(key == GLFW_KEY_2 && action == GLFW_PRESS){
                    
                currentNode = rider[10];//neck
            }
            if(key == GLFW_KEY_3 && action == GLFW_PRESS){
                    
                currentNode = rider[12];//left shoulder
            }
            if(key == GLFW_KEY_4 && action == GLFW_PRESS){
                    
                currentNode = rider[13];//right shoulder
            }
            if(key == GLFW_KEY_5 && action == GLFW_PRESS){
                    
                currentNode = rider[16];//left elbow
            }
            if(key == GLFW_KEY_6 && action == GLFW_PRESS){
                    
                currentNode = rider[17];//right elbow
            }
            if(key == GLFW_KEY_7 && action == GLFW_PRESS){
                    
                currentNode = rider[20];//left leg
            }
            if(key == GLFW_KEY_8 && action == GLFW_PRESS){
                    
                currentNode = rider[21];//right leg
            }
            if(key == GLFW_KEY_9 && action == GLFW_PRESS){
                    
                currentNode = rider[6];//left knee
            }
            if(key == GLFW_KEY_0 && action == GLFW_PRESS){
                    
                currentNode = rider[7];//right knee
            }
            if(key == GLFW_KEY_MINUS && action == GLFW_PRESS){
                    
                currentNode = rider[24];//left ankle
            }
            if(key == GLFW_KEY_EQUAL && action == GLFW_PRESS){
                    
                currentNode = rider[25];//right ankle
            }
            if(key == GLFW_KEY_SLASH && action == GLFW_PRESS){
                    
                currentNode = rider[0];//right ankle
            }

            if(currentNode == rider[0])
            {
                if(key == GLFW_KEY_R && action == GLFW_PRESS){
                    rider[0]->addTX(-1);
                }
                if(key == GLFW_KEY_F && action == GLFW_PRESS){
                    rider[0]->addTX(1);
                }
                if(key == GLFW_KEY_D && action == GLFW_PRESS){
                    rider[0]->addTY(-1);
                }
                if(key == GLFW_KEY_G && action == GLFW_PRESS){
                    rider[0]->addTY(1);
                }
                if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
                    rider[0]->addRY(-3);
                }
                if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
                    rider[0]->addRY(3);
                }
                if(key == GLFW_KEY_UP && action == GLFW_PRESS){
                    rider[0]->addRX(3);
                }
                if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
                    rider[0]->addRX(-3);
                }  
                if(key == GLFW_KEY_Z && action == GLFW_PRESS){
                    rider[0]->addTZ(-1);
                }
                if(key == GLFW_KEY_X && action == GLFW_PRESS){
                    rider[0]->addTZ(1);
                }
            }
            else
            {
                if(key == GLFW_KEY_UP && action == GLFW_PRESS){
                    currentNode->addRX(-3);
                }
                if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
                    currentNode->addRX(3);
                }  
                if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
                    currentNode->addRY(-3);
                }  
                if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
                    currentNode->addRY(3);
                }  
            }
        
        default:
            break;
        }
    }
};