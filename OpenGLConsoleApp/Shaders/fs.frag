#version 330

in vec4 vertexColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

struct directionalLight {
	vec3 color;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct material {
	float specularIntensity;
	float shininess;
};

uniform sampler2D theTexture;
uniform directionalLight dl;
uniform material mat;

uniform vec3 eyePosition;

out vec4 color;

void main() {
	vec4 ambientColor = vec4(dl.color, 1.0f) * dl.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(normal), normalize(dl.direction)), 0.0f);
	vec4 diffuseColor = vec4(dl.color, 1.0f) * dl.diffuseIntensity * diffuseFactor;
	
	vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	if (diffuseFactor > 0.0f) {
		vec3 fragToEye = normalize(eyePosition - fragPos);
		vec3 reflectedVertex = normalize(reflect(dl.direction, normalize(normal)));
		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f) {
			specularFactor = pow(specularFactor, mat.shininess);
		}
		specularColor = vec4(dl.color, 1.0f) * mat.specularIntensity * specularFactor;
	}

	color = texture(theTexture, texCoord) * (ambientColor + diffuseColor + specularColor);
}
