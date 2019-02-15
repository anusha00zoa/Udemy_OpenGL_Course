#version 330

in vec4 vertexColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct light {
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct directionalLight {
	light base;
	vec3 direction;
};

struct pointLight {
	light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct spotLight {
	pointLight base;
	vec3 direction;
	float edge;
};

struct material {
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount, spotLightCount;

uniform directionalLight dl;
uniform pointLight pl[MAX_POINT_LIGHTS];
uniform spotLight sl[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform material mat;

uniform vec3 eyePosition;

out vec4 color;


vec4 CalculateLightByDirection(light l, vec3 direction) {
	vec4 ambientColor = vec4(l.color, 1.0f) * l.ambientIntensity;

	float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(l.color, 1.0f) * l.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	if (diffuseFactor > 0.0f) {
		vec3 fragToEye = normalize(eyePosition - fragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));
		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f) {
			specularFactor = pow(specularFactor, mat.shininess);
			specularColor = vec4(l.color, 1.0f) * mat.specularIntensity * specularFactor;
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalculateDirectionalLight() {
	return CalculateLightByDirection(dl.base, dl.direction);
}

vec4 CalculatePointLight(pointLight pLight) {
	vec3 direction = fragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	vec4 color = CalculateLightByDirection(pLight.base, direction);
	float attenuation = (pLight.exponent * distance * distance) + (pLight.linear * distance) + pLight.constant;

	return (color / attenuation);

}

vec4 CalculateSpotLight(spotLight sLight) {
	vec3 rayDirection = normalize(fragPos - sLight.base.position);
	float spotLightFactor = dot(rayDirection, sLight.direction);
	if (spotLightFactor > sLight.edge) {
		vec4 color = CalculatePointLight(sLight.base);
		float spotLightFade = (1.0f - ((1.0f - spotLightFactor) / (1.0f - sLight.edge)));
		return color * spotLightFade;
	}
	else {
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

vec4 CalculatePointLights() {
	vec4 totalColor = vec4 (0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < pointLightCount; i++) {
		totalColor += CalculatePointLight(pl[i]);
	}

	return totalColor;
}

vec4 CalculateSpotLights() {
	vec4 totalColor = vec4 (0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < spotLightCount; i++) {
		totalColor += CalculateSpotLight(sl[i]);
	}

	return totalColor;
}

void main() {
	vec4 finalColor = CalculateDirectionalLight();
	finalColor += CalculatePointLights();
	finalColor += CalculateSpotLights();

	color = texture(theTexture, texCoord) * finalColor;
}
