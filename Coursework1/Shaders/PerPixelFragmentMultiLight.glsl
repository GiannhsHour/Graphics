# version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex1;
uniform sampler2D diffuseTex2;

uniform vec3 cameraPos ;
uniform vec4 lightColour ;
uniform vec3 lightPos ;
uniform float lightRadius ;
uniform float ambient ;
uniform int type;

 in Vertex {
 vec3 colour ;
 vec2 texCoord ;
 vec3 normal ;
 vec3 worldPos ;
 } IN ;

 out vec4 gl_FragColor ;
void main ( void ) {

  gl_FragColor = lightColour;

}


