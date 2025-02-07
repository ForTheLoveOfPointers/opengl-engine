#version 330 core
uniform sampler2D texSample;
in vec2 texCoord;

out vec4 FragColor;

void main()
{
    FragColor = texture(texSample, texCoord);
}