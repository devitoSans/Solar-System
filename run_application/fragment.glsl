#version 330 core

out vec4 fragColor;

struct Attenuation {
	float constant;
	float linear;
	float quadratic;
};

in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;

uniform sampler2D texSampler;
uniform sampler2D texSampler_night;
uniform sampler2D texSampler_cloud;

uniform int set_nightOff;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform Attenuation att;

uniform bool no_texture;
uniform bool begin;
uniform bool set_cloudOff;

void main() {
	//Texture
	vec4 mainTexture = texture(texSampler, vec2(texCoord.x * -1, texCoord.y));
	vec4 cloudTexture = texture(texSampler_cloud, vec2(texCoord.x * -1, texCoord.y));
	vec3 emissionTexture = vec3(texture(texSampler_night, texCoord));
	vec3 loadingTexture = vec3(texture(texSampler, texCoord));

	//check if the planet has cloud or not
	vec3 texture;
	if(!set_cloudOff)
		texture = vec3(mainTexture);
	else texture = vec3(mainTexture + cloudTexture);

	//properties of lights
	vec3 norm_normal = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	//ambient
	float ambient_strength = 0.1f;
	vec3 ambient = ambient_strength * lightColor * texture;

	//diffuse
	float diff = max(dot(norm_normal, lightDir), 0.f);
	vec3 diffuse = diff * lightColor * texture;

	//emission
	float em = max(dot(norm_normal, -lightDir), 0.f);
	vec3 emission = emissionTexture * em * set_nightOff;	//-> set if the planet has changed at night time

	//Attenuation
	float distance = length(lightPos - fragPos);
	float distance_quadrat = distance * distance;
	float attenuation = 1.f / (att.constant + att.linear * distance + att.quadratic * distance_quadrat);

	//result
	vec3 result;
	if(no_texture) {	//-> orbit result
		result = vec3(1.f);
	}
	else if(begin) {	//-> loading result
		result = loadingTexture;
	}
	else {	//-> planets result
		result = ((ambient + diffuse) * attenuation) + emission;
	}

	//output
	fragColor = vec4(result, 1.f);
}
