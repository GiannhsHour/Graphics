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
vec4 rockColor;

void main(void){
	gl_FragColor = IN.colour;
	if(useTexture > 0) {
		rockColor = gl_FragColor * texture(diffuseTex, IN.texCoord);
		if(IN.position.y>500){
			gl_FragColor *= texture(diffuseTex1, IN.texCoord) * (1.0-(750-IN.position.y)/250) + rockColor * ((750-IN.position.y)/250);
		}
		else if(IN.position.y<180){
			gl_FragColor *= texture(diffuseTex2, IN.texCoord) * (1.0-IN.position.y/180) + rockColor * (IN.position.y/180);
			
		}
		else{
			gl_FragColor  = rockColor ;
		}
		
	}
}