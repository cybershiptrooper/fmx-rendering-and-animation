#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTex;

uniform float useTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform float trackL1;
uniform float trackL2;
uniform float riderSL;
uniform float bikeHL;

out float trackLight1;
out float trackLight2;
out float riderSpotlight;
out float headLight;

out vec3 normal;
out vec4 colour;
out vec3 fragpos;
out vec2 tex;
out float usetex;

void main()
{
    colour      = vec4(aColour,1.0);
    fragpos     = vec3(model * vec4(aPos,1.0f));
    normal      = normalMatrix * normalize(aNormal);
    usetex      = useTexture;
    tex         = aTex;

    trackLight1    = trackL1;
    trackLight2    = trackL2;
    headLight      = bikeHL;
    riderSpotlight = riderSL;

    gl_Position = projection * view * vec4(fragpos,1.0f);
}
