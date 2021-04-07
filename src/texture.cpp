#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "texture.hpp"

GLuint loadTexture(const char * filename)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int width, height, dims;
	unsigned char* data = stbi_load(filename, &width, &height, &dims, 0);
	if(data)
	{
		GLenum format = GL_RGB;
		switch(dims)
		{
			case 1:format = GL_RED;break;
			case 3:format = GL_RGB;break;
			case 4:format = GL_RGBA;break;
		}
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		std::cerr<<"Failed to load texture"<<std::endl;
	}
	stbi_image_free(data);
	return texture;
}


GLuint loadCubemap(const char * files[6]){
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	for (int i = 0; i < 6; i++)
	{
		int width, height, dims;
		const char * filename = files[i];
		unsigned char* data = stbi_load(filename, &width, &height, &dims, 4);
		if(data)
		{
			GLenum format = GL_RGB;
			switch(dims){
				case 1:format = GL_RED;break;
				case 3:format = GL_RGB;break;
				case 4:format = GL_RGBA;break;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
				GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else{
			std::cerr<<"File not found:"<<files[i]<<std::endl;
			return texture;
		}
		
		stbi_image_free(data);
	}

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}