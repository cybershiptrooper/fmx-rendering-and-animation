#version 330 core

in vec3 fragpos;
in vec4 colour;
in vec3 normal;
in vec2 tex;
in float usetex;

in float trackLight1;
in float trackLight2;
in float riderSpotlight;
in float headLight;

uniform sampler2D ourTexture;
uniform vec3 viewPos;
uniform vec3 spotlightPos;
uniform vec3 headlightPos;

out vec4 fragColour;

void main()
{
    vec3 result = vec3(0.0);
    vec4 texImage = colour;
    if(usetex > 0)
        texImage = texture(ourTexture,tex);
    
    if(trackLight1 > 0)
    {
        vec3 lightColour = vec3(1.0);
        vec3 lightPos = vec3(10,10,20);

        // ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColour;
        
        // diffuse 
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - fragpos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColour;
        
        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - fragpos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColour;  
            
        result += (ambient + diffuse + specular) * vec3(texImage);
    }

    if(trackLight2 > 0)
    {
        vec3 lightColour = vec3(1.0);
        vec3 lightPos = vec3(-20,10,-40);

        // ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColour;
        
        // diffuse 
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - fragpos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColour;
        
        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - fragpos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColour;  
            
        result += (ambient + diffuse + specular) * vec3(texImage);
    }

    if(riderSpotlight > 0)
    {
        vec3 lightColour = vec3(1.0);
        vec3 lightPos = spotlightPos;

        // ambient
        float ambientStrength = 0;
        vec3 ambient = ambientStrength * lightColour;
        
        // diffuse 
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - fragpos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColour;
        
        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - fragpos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColour;  
        
        vec3 sd = normalize(-1 * vec3(0,-1,0));
        if(dot(sd,lightDir) > 0.97 && diff > 0)
        {
            result += (diffuse + specular) * vec3(texImage);    
        }
        else
            result += ambient * vec3(texImage);
    }

    if(headLight > 0)
    {
        vec3 lightColour = vec3(1.0);
        vec3 lightPos = headlightPos;

        // ambient
        float ambientStrength = 0;
        vec3 ambient = ambientStrength * lightColour;
        
        // diffuse 
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - fragpos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColour;
        
        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - fragpos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColour;  
        
        vec3 sd = normalize(-1 * vec3(1.5,-1,0));
        if(dot(sd,lightDir) > 0.97 && diff > 0)
        {
            result += (diffuse + specular) * vec3(texImage);    
        }
        else
            result += ambient * vec3(texImage);
    }

    fragColour = vec4(result,1.0f);
}