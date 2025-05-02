#version 400 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 normal;
	vec3 fragPosition;
	vec2 texCoord;
} gs_in[];

out vec3 normal;
out vec3 fragPosition;
out  vec2 texCoord;

uniform float time;

vec3 getNormal(){
	vec3 a = vec3(gl_in[0].gl_Position-gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
	return normalize(cross(a,b));
}

vec4 explode(vec4 position,vec3 normal){
	float magnitude = 2.1;
	vec3 direction = normal*magnitude*((sin(time)+1.0)/2.0);
	return position + vec4(direction,0.0);
}

void main(){
	vec3 expNormal = getNormal();

	gl_Position = explode(gl_in[0].gl_Position,expNormal);
	normal = gs_in[0].normal;
	fragPosition =vec3(explode(vec4(gs_in[0].fragPosition,0),expNormal));
	texCoord = gs_in[0].texCoord;
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position,expNormal);
	normal = gs_in[1].normal;
	fragPosition =vec3(explode(vec4(gs_in[1].fragPosition,0),expNormal));
	texCoord = gs_in[1].texCoord;
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position,expNormal);
	normal = gs_in[2].normal;
	fragPosition =vec3(explode(vec4(gs_in[2].fragPosition,0),expNormal));
	texCoord = gs_in[2].texCoord;
	EmitVertex();

	EndPrimitive();
}