#version 330 core
//precision high float;
out vec4 fragColor;//gl_FragColor is deprecated

in vec3 texel;

uniform samplerCube skybox;

void main(){
	fragColor = texture(skybox, texel);
}
