#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace cg
{
    class Node{
        Node* parent;
        std::vector<Node*> children;
        unsigned int VAO,VBO,TEX,numVertices;
        float tx,ty,tz,rx,ry,rz,sx,sy,sz;
        float xh,xl,yh,yl,zh,zl;    
        bool textureFlag = 0;
        glm::mat4 rotate,translate,scale;

    public:
        Node(Node* parent, unsigned int numVertices, unsigned int TEX = 0, bool textureFlag = 0);
        void render();
        void renderTree();
        void updateModelParams(float tx, float ty, float tz, float rx, float ry, float rz,float sx=1,float sy=1,float sz=1);
        void updateModelConstraints(float xh,float xl,float yh,float yl,float zh,float zl);
        void updateModel();
        void addChild(Node* child);
        void addObj(float* vertices, int size);
        void updateBuffer();
        void addRX(float theta);
        void addRY(float theta);
        void addRZ(float theta);
        void addTX(float theta);
        void addTY(float theta);
        void addTZ(float theta);

        void storeNodeParams(std::ofstream& values);
        void updateFrameParams(std::vector<float>& frame);
    };

    glm::mat4* stackProduct(std::vector<glm::mat4> matrixStack);
    glm::mat4* getModelMatrix(std::vector<glm::mat4> matrixStack);
};

#endif