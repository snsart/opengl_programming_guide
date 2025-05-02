#version 400 core
out vec4 color;	
in vec3 texCoord;

uniform samplerCube skybox;

void main() {
    //vec4 color1 = vec4(vec3(1.0f,1.0f,0.0f),1.0f);
    vec4 texColor =  texture(skybox,texCoord);
    color = texColor;
}									