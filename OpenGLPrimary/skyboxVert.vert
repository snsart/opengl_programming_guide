 #version 400 core
 layout(location = 0) in vec3 position;	

out  vec3 texCoord;
uniform mat4 view;

layout(std140) uniform Matrices{
	mat4 projection;
};


 void main(){
	texCoord = position;
	vec4 pos = projection*view*vec4(position,1.0f); 
	gl_Position = pos.xyww; 
}