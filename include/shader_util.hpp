#ifndef _SHADER_UTIL_HPP_
#define _SHADER_UTIL_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cg
{
    class Shader{
    public:
        GLuint ID;
        Shader();
        void addShaders(const char* vPath, const char* fPath);
        void useShader();
        void deleteShader();
        void setInt(int f, const std::string &uniform);
        void setInt(unsigned int f, const std::string &uniform);
        void setFloat(float f, const std::string &uniform);
        void setVec3(glm::vec3 &trans, const std::string &uniform);
        void setMat4(glm::mat4 trans, const std::string &uniform);
        void setMat3(glm::mat3 trans, const std::string &uniform);

    private:
        void checkCompileErrors(GLuint ID,std::string type);
    };
};
#endif