 #version 330 core
 layout(location = 0) in vec3 position;	
 layout(location = 1) in vec3 aNormal;
  layout(location = 2) in vec2 atexCoord;

 out vec3 normal;
out vec3 fragPosition;
out  vec2 texCoord;
 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;

 void main(){
	normal =  mat3(transpose(inverse(model)))*aNormal;
	fragPosition = vec3(model * vec4(position,1.0f));
	texCoord = atexCoord;
	gl_Position = projection*view*model*vec4(position,1.0f); 
}