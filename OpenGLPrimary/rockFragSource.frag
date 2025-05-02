#version 400 core
out vec4 color;	

struct Material{
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light{
    vec3 lightPos;
    vec3 lightColor;
    vec3 lightDirection;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionLight{
    vec3 position;
    vec3 color;
    vec3 direction;
};

struct PointLight{
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight{
    vec3 position;
    vec3 color;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

uniform DirectionLight directionLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

in vec3 normal;
in vec3 fragPosition;
in vec2 texCoord;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform samplerCube skybox;

void main() {
     vec3 ambient = texture(material.diffuse,texCoord).rgb*0.2;//环境光影响最小

     vec3 norm = normalize(normal);
     vec3 viewDir = normalize(viewPos - fragPosition);

    //direction light
    vec3 dirLightDir = -normalize(directionLight.direction);
    float dirDiff = max(dot(norm,dirLightDir),0.0);
    vec3 dirDiffuse = texture(material.diffuse,texCoord).rgb * dirDiff *directionLight.color*0.4;

    vec3 dirReflectDir = reflect(-dirLightDir,norm);
    float dirSpec = pow(max(dot(viewDir,dirReflectDir),0.0),32);
   // if(dirDiff==0) dirSpec = 0;//否则当入射光跟法向量夹角大于90度时（即要渲染的点处于眼睛和光源之间），会产生奇怪的现象；
    vec3 dirSpecular = texture(material.specular,texCoord).rgb * dirSpec * directionLight.color;

    vec3 dirResult = dirDiffuse + dirSpecular;

    //point light
     float pointDis = length(pointLight.position-fragPosition);
     float pointAttenuation = 1.0 / (pointLight.constant + pointLight.linear * pointDis + pointLight.quadratic * (pointDis * pointDis));

     vec3 pointLightDir = -normalize(fragPosition-pointLight.position);
     float pointDiff = max(dot(norm,pointLightDir),0.0);
     vec3 pointDiffuse = texture(material.diffuse,texCoord).rgb * pointDiff * pointLight.color *0.4;

     vec3 pointReflectDir = reflect(-pointLightDir,norm);
     float pointSpec = pow(max(dot(viewDir,pointReflectDir),0.0),32);
     vec3 pointSpecular = texture(material.specular,texCoord).rgb * pointSpec * pointLight.color;

     vec3 pointResult = (pointDiffuse + pointSpecular)*pointAttenuation;

     //spot light
    float spotDis = length(spotLight.position-fragPosition);
    float spotAttenuation = 1.0 / (spotLight.constant + spotLight.linear * spotDis + spotLight.quadratic * (spotDis *spotDis));
 
    vec3 spotLightDir = -normalize(fragPosition-spotLight.position);
    float theta = dot(spotLightDir,normalize(-spotLight.direction));
    float spotDiff = max(dot(norm,spotLightDir),0.0);
    vec3 spotDiffuse = texture(material.diffuse,texCoord).rgb * spotDiff *spotLight.color*0.8;

    vec3 spotReflectDir = reflect(-spotLightDir,norm);
    float spotSpec = pow(max(dot(viewDir,spotReflectDir),0.0),32);
    vec3 spotSpecular = texture(material.specular,texCoord).rgb * spotSpec * spotLight.color;

    float intensity = clamp( (theta - spotLight.outerCutOff)/(spotLight.cutOff-spotLight.outerCutOff),0.0,1.0);

    vec3 spotResult = (spotDiffuse*intensity+spotSpecular*intensity);

    vec3 result = ambient+dirResult+pointResult;

    vec3 r = reflect(-viewDir,norm);
    vec3 r2 = refract(-viewDir,norm,1.00/1.52);

     color = vec4(result,1.0f);
}									