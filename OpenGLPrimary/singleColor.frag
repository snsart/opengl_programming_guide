#version 330 core
out vec4 color;	
in vec2 texCoord;

uniform sampler2D texture1;
void main() {
    //color = vec4(vec3(1.0f,1.0f,0.0f),1.0f);
    vec4 texColor =  texture(texture1,texCoord);
    color = texColor;
}									