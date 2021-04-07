#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

//opengl stuff
#include "gl_framework.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//methods
GLuint loadTexture(const char * filename);
GLuint loadCubemap(const char * files[6]);

#endif