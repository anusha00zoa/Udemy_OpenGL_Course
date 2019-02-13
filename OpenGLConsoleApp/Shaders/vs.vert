#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec4 vertexColor;
out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vertexColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	texCoord = tex;
	
	// NORMAL DOES NOT CHANGE IF OBJECT IS ONLY BEING TRANSLATED
	// NORMAL CHANGES IF OBJECT IS BEING ROTATED OR SCALED
	// THEREFORE CAN IGNORE FOURTH COLUMN OF MODEL MATRIX
	
	// ANISOTROPIC SCALING (IN ONE OR TWO DIMENSIONS ONLY) CAUSES CHANGE IN THE NORMALS AS WELL
	// THEREFORE WE INVERT THE SCALINGS BY APPLYING TRANSPOSE AND THEN INVERSE
	normal = mat3(inverse(transpose(model))) * norm;
	
	fragPos = (model * vec4(pos, 1.0)).xyz;
}