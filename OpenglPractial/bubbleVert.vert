#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1 ) in vec3 nNormal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 pro;

out vec3 normal;
out vec3 fragPosition;

layout(std140) uniform Matrices{
	mat4 projection;
};

void main(){
	normal = normalize(mat3(transpose(inverse(model)))*nNormal);
	fragPosition =vec3(model*vec4(position,1.0f));
	gl_Position = pro*view*model*vec4(position,1.0f);
}