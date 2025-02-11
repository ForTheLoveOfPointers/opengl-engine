#version 330 core
uniform sampler2D texSample;
uniform vec3 lightColor;

in vec2 texCoord;


out vec4 FragColor;

void main()
{
    FragColor = vec4(lightColor, 1.0f) * texture(texSample, texCoord);
}