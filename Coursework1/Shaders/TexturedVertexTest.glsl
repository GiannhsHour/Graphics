#version 150

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform vec4 nodeColour;

in vec3 position;
in vec2 texCoord;

out Vertex {
	vec2 texCoord;
	vec4 colour;
	vec4 position;
} OUT;

void main(void) {
	
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(position, 1.0);
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	OUT.colour = nodeColour;
	OUT.position = projMatrix*vec4(position, 1.0);
}