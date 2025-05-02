#version 400 core

out vec4 fragColor;
in vec3 normal;
in vec3 fragPosition;

uniform samplerCube skybox;
uniform vec3 viewPos;

void main(){
	vec3 viewDir = fragPosition-viewPos;
	vec3 r = reflect(viewDir,normal);
	fragColor = vec4(texture(skybox,r).rgb,1.0);
}