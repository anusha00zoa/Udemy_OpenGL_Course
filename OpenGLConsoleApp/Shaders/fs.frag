#version 330

in vec4 vertexColor;
in vec2 texCoord;


struct directionalLight {
	vec3 color;
	float ambientIntensity;
};

uniform sampler2D theTexture;
uniform directionalLight dl;

out vec4 color;

void main() {
	vec4 ambientColor = vec4(dl.color, 1.0f) * dl.ambientIntensity;

	color = texture(theTexture, texCoord) * ambientColor;
}
