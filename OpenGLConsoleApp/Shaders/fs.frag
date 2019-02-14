#version 330

in vec4 vertexColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

const int MAX_POINT_LIGHTS = 3;

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

struct material {
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;

uniform directionalLight dl;
uniform pointLight pl[MAX_POINT_LIGHTS];

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

vec4 CalculatePointLights() {
	vec4 totalColor = vec4 (0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < pointLightCount; i++) {
		vec3 direction = fragPos - pl[i].position;
		float distance = length(direction);
		direction = normalize(direction);

		vec4 color = CalculateLightByDirection(pl[i].base, direction);
		float attenuation = (pl[i].exponent * distance * distance) + (pl[i].linear * distance) + pl[i].constant;

		totalColor += (color / attenuation);
	}

	return totalColor;
}

void main() {
	vec4 finalColor = CalculateDirectionalLight();
	finalColor += CalculatePointLights();

	color = texture(theTexture, texCoord) * finalColor;
}
