#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1 ) in vec3 nNormal;


uniform mat4 model;
uniform mat4 view;

out VS_OUT{
	vec3 normal;
} vs_out;

layout(std140) uniform Matrices{
	mat4 projection;
};

void main(){
	vs_out.normal = normalize(mat3(transpose(inverse(model*view)))*nNormal);
	gl_Position = model*view*vec4(position,1.0f);
}
