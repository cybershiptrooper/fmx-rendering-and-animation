#include "node.hpp"
#include "shader_util.hpp"

extern cg::Shader shaderProgram;
extern std::vector<glm::mat4> matrixStack;

namespace cg
{
    Node::Node(Node* parent, unsigned int numVertices, unsigned int TEX, bool textureFlag)
    {
        this->TEX = TEX;
        this->textureFlag = textureFlag;
        this->numVertices = numVertices;

        if(!parent)
            this->parent = NULL;
        else
        {
            this->parent = parent;
            parent->addChild(this); 
        }

        tx = ty = tz = rx = ry = rz = 0;
        xh = xl = yh = yl = zh = zl = 0;
        sx = sy = sz = 1;
        this->updateModel();
    }

    void Node::addChild(Node* child)
    {
        this->children.push_back(child);
    }

    void Node::updateModelParams(float tx, float ty, float tz, float rx, float ry, float rz,float sx,float sy, float sz)
    {
        this->tx = tx;
        this->ty = ty;
        this->tz = tz;
        this->rx = rx;
        this->ry = ry;
        this->rz = rz;
        this->sx = sx;
        this->sy = sy;
        this->sz = sz;
        updateModel();
    }

    void Node::updateModelConstraints(float xh,float xl,float yh,float yl,float zh,float zl)
    {
        this->xh = xh;
        this->xl = xl;
        this->yh = yh;
        this->yl = yl;
        this->zh = zh;
        this->zl = zl;
    }

    void Node::updateModel()
    {
        rotate = glm::mat4(1.0f);
        rotate = glm::rotate(rotate, glm::radians(rx), glm::vec3(1.0f,0.0f,0.0f));
        rotate = glm::rotate(rotate, glm::radians(ry), glm::vec3(0.0f,1.0f,0.0f));
        rotate = glm::rotate(rotate, glm::radians(rz), glm::vec3(0.0f,0.0f,1.0f));

        translate = glm::mat4(1.0f);
        translate = glm::translate(translate,glm::vec3(tx,ty,tz));

        scale = glm::mat4(1.0f);
        scale = glm::scale(scale,glm::vec3(this->sx,this->sy,this->sz));
    }

    void Node::addObj(float* vertices, int size)
    {
        updateBuffer();
        glBufferData(GL_ARRAY_BUFFER,size,vertices,GL_STATIC_DRAW);
    }

    void Node::updateBuffer()
    {
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)(3*(sizeof(float))));
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)(6*(sizeof(float))));
        glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)(9*(sizeof(float))));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glBindVertexArray(0);
    }

    void Node::render()
    {
        glBindVertexArray(VAO);
        // glm::mat4* currStackState = stackProduct(matrixStack);
        glm::mat4* currModelState = getModelMatrix(matrixStack);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(*currModelState)));
        // shaderProgram.setMat4(*currStackState,"MVP");
        shaderProgram.setMat4(*currModelState,"model");
        shaderProgram.setMat3(normalMatrix,"normalMatrix");

        if(this->textureFlag)
        {
            glBindTexture(GL_TEXTURE_2D,this->TEX);
            shaderProgram.setFloat(1,"useTexture");
        }
        else
        {
            shaderProgram.setFloat(0,"useTexture");
        }

        glDrawArrays(GL_TRIANGLES,0,this->numVertices);
        // delete(currStackState);
        delete(currModelState);
    }

    void Node::renderTree()
    {
        glm::mat4 model = this->translate * this->rotate * this->scale;
        matrixStack.push_back(model);
        render();
        for(unsigned int i=0;i<children.size();i++)
            children[i]->renderTree();
        matrixStack.pop_back();
    }
    void Node::addRX(float theta)
    {
        this->rx = (int)(rx+theta)%360;
        if(this->xh)
            this->rx = std::min(this->xh,this->rx);
        if(this->xl)
            this->rx = std::max(this->xl,this->rx);
        updateModel();
    }
    void Node::addRY(float theta)
    {
        this->ry = (int)(ry+theta)%360;
        if(this->yh)
            this->ry = std::min(this->yh,this->ry);
        if(this->yl)
            this->ry = std::max(this->yl,this->ry);
        updateModel();
    }
    void Node::addRZ(float theta)
    {
        this->rz = (int)(rz+theta)%360;
        if(this->zh)
            this->rz = std::min(this->zh,this->rz);
        if(this->zl)
            this->rz = std::max(this->zl,this->rz);
        updateModel();
    }
    void Node::addTX(float theta)
    {
        this->tx += theta;
        updateModel();
    }
    void Node::addTY(float theta)
    {
        this->ty += theta;
        updateModel();
    }
    void Node::addTZ(float theta)
    {
        this->tz += theta;
        updateModel();
    }

    std::string itos(int k) {
        return std::to_string(k) + " ";
    }

    std::string itos(float k) {
        return std::to_string(k) + " ";
    }

    std::string itos(bool k) {
        return std::to_string(k) + " ";
    }

    void Node::storeNodeParams(std::ofstream& values)
    {
        std::queue<Node*> q;
        q.push(this);
        while(!q.empty())
        {
            Node* top = q.front();
            q.pop();
            values << itos(top->tx) + itos(top->ty) + itos(top->tz) + 
                    itos(top->rx) + itos(top->ry) + itos(top->rz) + 
                    itos(top->sx) + itos(top->sy) + itos(top->sz);

            for(Node* child : top->children)
                q.push(child);
        }
    }

    void Node::updateFrameParams(std::vector<float>& frame)
    {
        int i = 10;
        std::queue<Node*> q;
        q.push(this);
        while(!q.empty())
        {
            Node* top = q.front();
            q.pop();
            top->tx = frame[i++];
            top->ty = frame[i++];
            top->tz = frame[i++];
            top->rx = frame[i++];
            top->ry = frame[i++];
            top->rz = frame[i++];
            top->sx = frame[i++];
            top->sy = frame[i++];
            top->sz = frame[i++];
            top->updateModel();    

            for(Node* child : top->children)
                q.push(child);
        }
    }

    glm::mat4* stackProduct(std::vector<glm::mat4> matrixStack)
    {
		glm::mat4* mult = new glm::mat4(1.0f);
		for(unsigned int i=0;i<matrixStack.size();i++)
			*mult = (*mult) * matrixStack[i];
		return mult;
	}

    glm::mat4* getModelMatrix(std::vector<glm::mat4> matrixStack)
    {
		glm::mat4* mult = new glm::mat4(1.0f);
		for(unsigned int i=1;i<matrixStack.size();i++)
			*mult = (*mult) * matrixStack[i];
		return mult;
	}
};