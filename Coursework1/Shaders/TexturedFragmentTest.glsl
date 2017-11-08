#version 150
uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex1;
uniform sampler2D diffuseTex2;
uniform int useTexture;

in Vertex {
	vec2 texCoord;
	vec4 colour;
	vec4 position;
} IN;

out vec4 gl_FragColor;

void main(void){
	gl_FragColor = IN.colour;
	if(useTexture > 0) {
		if(IN.position.y>604){
			gl_FragColor *= texture(diffuseTex1, IN.texCoord);
		}
		else if(IN.position.y<181){
			gl_FragColor *= texture(diffuseTex2, IN.texCoord);
		}
		else{
			gl_FragColor *= texture(diffuseTex, IN.texCoord);
		}
		
	}
}