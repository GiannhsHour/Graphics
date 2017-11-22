#version 150 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 shadowMatrix1;
uniform mat4 shadowMatrix2;

in vec3 position ;
in vec4 colour ;
in vec3 normal ;
in vec3 tangent ; 
in vec2 texCoord ;

out Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent; 
	vec3 binormal; 
	vec3 worldPos;
	vec4 shadowProj1;
	vec4 shadowProj2;
} OUT;

void main(void) {
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	OUT.colour = colour ;
	OUT.texCoord = (textureMatrix * vec4 (texCoord , 0.0 , 1.0)).xy;
	

	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.tangent = normalize(normalMatrix * normalize(tangent));
	OUT.binormal = normalize(normalMatrix * normalize(cross(normal , tangent)));

	OUT.worldPos = (modelMatrix * vec4 (position ,1)).xyz;
	OUT.shadowProj1 = ( shadowMatrix1 * modelMatrix * vec4 ( position +( normal * 20) ,1)); //10.5 relative to map size
	OUT.shadowProj2 = ( shadowMatrix2 * modelMatrix * vec4 ( position +( normal * 20) ,1));
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4 (position , 1.0);
}