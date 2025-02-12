#version 330 core
uniform sampler2D texSample;
uniform sampler2D texFace;
uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 cameraPos;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPosition;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(normal);
    vec3 ambient = vec3(0.4, 0.4, 0.4);
    vec3 lightDirection = normalize(lightPos -fragPosition);
    vec3 camDirection = normalize(cameraPos -fragPosition);

    float lightIntesity = max( dot(norm, lightDirection), 0.0 );
    float specularIntensity = max( dot(reflect(-lightDirection, norm), camDirection), 0.0);
    specularIntensity = pow(specularIntensity, 64);
    float totalIntensity = lightIntesity + specularIntensity;

    FragColor = vec4(ambient + totalIntensity * lightColor, 1.0) * mix(texture(texSample, texCoord), texture(texFace, texCoord), 0.8);
}