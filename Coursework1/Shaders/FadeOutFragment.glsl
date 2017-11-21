#version 150
uniform sampler2D diffuseTex;
uniform float alpha;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 FragColor;

void main(void){
	FragColor = texture(diffuseTex, IN.texCoord);
	FragColor.a = alpha;
}