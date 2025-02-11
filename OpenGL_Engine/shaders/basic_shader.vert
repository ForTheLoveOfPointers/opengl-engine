#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord_;
layout (location = 2) in vec3 aNormal;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //THIS TRANSPOSE INVERSE THING IS HERE IN CASE I WANT TO SCALE MODELS
    //NON-UNIFORMLY, ELSE THE NORMAL VECTOR WILL BE WRONG. IF I DON'T IMPLEMENT
    // SUCH THING, THIS OPERATION CAN BE ELIMINATED
    normal =  mat3( transpose(inverse(model)) ) * aNormal;
    fragPosition = vec3( model * vec4(aPos, 1.0) );
    texCoord = texCoord_;
}       