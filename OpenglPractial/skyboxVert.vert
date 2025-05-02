 #version 400 core
 layout(location = 0) in vec3 position;	

out  vec3 texCoord;
uniform mat4 view;
uniform mat4 pro;

layout(std140) uniform Matrices{
	mat4 projection;
};


 void main(){
	texCoord = vec3(position.x,-position.y,position.z);
	vec4 pos = pro*view*vec4(position,1.0f); 
	gl_Position = pos.xyww; 
}