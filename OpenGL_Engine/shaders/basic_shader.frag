#version 330 core
uniform sampler2D texSample;
uniform sampler2D specularSample;
uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 cameraPos;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPosition;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

void main()
{
// ambient
    vec3 ambient = lightColor * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(cameraPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);  
        
    FragColor = vec4(ambient + diffuse, 1.0) * texture(texSample, texCoord) 
    + vec4(specular, 1.0) * texture(specularSample, texCoord);
}