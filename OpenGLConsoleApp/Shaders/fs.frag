#version 330

in vec4 vertexColor;
in vec2 texCoord;
in vec3 normal;

struct directionalLight {
	vec3 color;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

uniform sampler2D theTexture;
uniform directionalLight dl;

out vec4 color;

void main() {
	vec4 ambientColor = vec4(dl.color, 1.0f) * dl.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(normal), normalize(dl.direction)), 0.0f);
	vec4 diffuseColor = vec4(dl.color, 1.0f) * dl.diffuseIntensity * diffuseFactor;

	color = texture(theTexture, texCoord) * (ambientColor + diffuseColor);
}
