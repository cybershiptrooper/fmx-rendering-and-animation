#version 330 core

out vec3 texel;

layout(location = 0) in vec3 vPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 scalesky;

void main(){
	texel = vPos;
	gl_Position = projection * view * scalesky * vec4(vPos,1.0);
}