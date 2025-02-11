#version 330 core
uniform sampler2D texSample;
uniform vec3 lightColor;
uniform vec3 lightPos; 

in vec2 texCoord;
in vec3 normal;
in vec3 fragPosition;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightPos -fragPosition);
    float lightIntesity = max( dot(norm, lightDirection), 0.0 );
    FragColor = lightIntesity * vec4(lightColor, 1.0f) * texture(texSample, texCoord);
}