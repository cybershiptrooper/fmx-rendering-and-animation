#include "shader_util.hpp"

namespace cg
{
    Shader::Shader(){}

    void Shader::addShaders(const char* vPath, const char* fPath)
    {
        std::string vertexCodeString,fragmentCodeString;
        std::ifstream vShaderFile,fShaderFile;
        vShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        fShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

        try
        {
            vShaderFile.open(vPath);
            fShaderFile.open(fPath);

            std::stringstream vStream,fStream;
            vStream << vShaderFile.rdbuf();
            fStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCodeString = vStream.str();
            fragmentCodeString = fStream.str();
        }

        catch(std::ifstream::failure &e)
        {
            std::cout << "ERROR::SHADER__FILE_FAILURE" << std::endl;
        }

        const char* vertexCode = vertexCodeString.c_str();
        const char* fragmentCode = fragmentCodeString.c_str();
        GLuint vertex,fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex,1,&vertexCode,NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex,"VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment,1,&fragmentCode,NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment,"FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID,vertex);
        glAttachShader(ID,fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID,"PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::checkCompileErrors(GLuint ID,std::string type)
    {
        int success;
        char infolog[1024];
        if(type == "PROGRAM")
        {
            glGetProgramiv(ID,GL_LINK_STATUS,&success);
            if(!success)
            {
                glGetProgramInfoLog(ID,1024,NULL,infolog);
                std::cout << "ERROR::LINKING_FAILED " << infolog << std::endl;
            }
        }
        else
        {
            glGetShaderiv(ID,GL_COMPILE_STATUS,&success);
            if(!success)
            {
                glGetShaderInfoLog(ID,1024,NULL,infolog);
                std::cout << "ERROR::SHADER_COMPILE_FAILED " << infolog << std::endl;
            }
        }
    }

    void Shader::useShader()
    {
        glUseProgram(ID);
    }

    void Shader::deleteShader()
    {
        glDeleteProgram(ID);
    }

    void Shader::setFloat(float f, const std::string &uniform)
    {
        glUniform1f(glGetUniformLocation(ID, uniform.c_str()), f);
    }

    void Shader::setInt(int f, const std::string &uniform)
    {
        glUniform1i(glGetUniformLocation(ID, uniform.c_str()), f);
    }

    void Shader::setInt(unsigned int f, const std::string &uniform)
    {
        glUniform1i(glGetUniformLocation(ID, uniform.c_str()), (int)f);
    }

    void Shader::setMat4(glm::mat4 trans, const std::string &uniform)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, uniform.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
    }

    void Shader::setMat3(glm::mat3 trans, const std::string &uniform)
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, uniform.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
    }

    void Shader::setVec3(glm::vec3 &trans, const std::string &uniform)
    {
        glUniform3fv(glGetUniformLocation(ID, uniform.c_str()), 1, &trans[0]);
    }
};