#version 330 core
out vec4 color;	

struct Material{
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    sampler2D xxx;
    float shininess;
};

uniform sampler2D spectest;

uniform Material material;

in vec3 normal;
in vec3 fragPosition;
in vec2 texCoord;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {	
    vec3 ambient = lightColor *texture(material.diffuse,texCoord).rgb* material.ambient *0.2f;//环境光影响最小

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPosition);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = texture(material.diffuse,texCoord).rgb * diff *0.4;

    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = texture(material.specular,texCoord).rgb * spec * lightColor;

    vec3 emission = texture(material.emission,texCoord).rgb;

    vec3 result = ambient+diffuse+specular;

    color = vec4(result,1.0f);
}									