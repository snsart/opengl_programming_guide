#version 330 core
out vec4 color;	

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

in vec3 normal;
in vec3 fragPosition;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {	
    vec3 ambient = lightColor * material.ambient *0.2f;//环境光影响最小

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPosition);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse)*0.5f;

    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = lightColor *(spec*material.specular);

    vec3 result =ambient+diffuse+specular;
    color = vec4(result,1.0f);
}									