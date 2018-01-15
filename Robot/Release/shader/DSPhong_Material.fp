// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 430
struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 UV;
in vec3 V;
in vec4 shadow_coord;
vec4 ambientColor = vec4(0.1,0.1,0.1,1);
vec4 diffuseColor = vec4(0.8,0.8,0.8,1);   
vec4 specularColor = vec4(1,1,1,1);
float Shininess = 50.0;

layout (binding = 0) uniform sampler2DShadow shadow_tex;
uniform int displayMode;
uniform MaterialInfo Material;
uniform float time;
uniform vec3 vLightPosition;

out vec4 vFragColor;

void main(void){
	float diff = max(0.0, dot(normalize(vVaryingNormal),normalize(vVaryingLightDir)))*1.2;
	if(displayMode == 2)//Solarization
		diff = 1-diff;
	vec4 diffuse = diff * diffuseColor * vec4(Material.Kd,1);
	vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),normalize(vVaryingNormal)));
	float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
	spec = pow(spec, Shininess);
	vec4 specular = spec * specularColor * vec4(Material.Ks,1);
	vFragColor = diffuse + specular + ambientColor;

	if(displayMode == 3)//Negative
		vFragColor = vec4(1-vFragColor.x, 1-vFragColor.y, 1-vFragColor.z, vFragColor.w);
	if(displayMode == 4){//black&white
		float gray = 0.2126 * vFragColor.r + 0.7152 * vFragColor.g + 0.0722 * vFragColor.b;
		gray = floor(gray * float(2));
		vFragColor = vec4(gray, gray, gray, vFragColor.w);
	}
}