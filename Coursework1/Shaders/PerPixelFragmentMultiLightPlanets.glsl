# version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex1;
uniform sampler2D diffuseTex2;
uniform sampler2D diffuseTex3;
uniform sampler2D diffuseTex4;

#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light {
	 vec4 lightColour ;
	 vec3 lightPos ;
	 float lightRadius ;
	 float ambient ;
} allLights[MAX_LIGHTS];

uniform vec3 cameraPos ;

uniform int type;

 in Vertex {
 vec3 colour ;
 vec2 texCoord ;
 vec3 normal ;
 vec3 worldPos ;
 } IN ;

 out vec4 FragColor ;
void main ( void ) {
 vec4 diffuse;
 
if(type == 0){
		diffuse = texture ( diffuseTex , IN.texCoord ) * 150 ;
	}
else if(type == 1){
			diffuse = texture ( diffuseTex1 , IN.texCoord ) ;
				   }
else if(type == 2){
			diffuse = texture ( diffuseTex2 , IN.texCoord ) * 10 ;
				   }
else if(type == 3){
			diffuse = texture ( diffuseTex3 , IN.texCoord ) ;
				   }

  FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
  for(int i =0; i< numLights; i++){
      vec4 tempColour;
	  vec3 incident = normalize ( allLights[i].lightPos - IN.worldPos );
	  float lambert = max (0.0 , dot ( incident , IN.normal ));
	  if(lambert == 0 && type == 2) {
		diffuse  = texture ( diffuseTex4 , IN.texCoord ) * 10 ;
	  }
	  float dist = length ( allLights[i].lightPos - IN.worldPos );
	  float atten = 1.0 - clamp ( dist / allLights[i].lightRadius , 0.0 , 1.0);
	  vec3 viewDir = normalize ( cameraPos - IN.worldPos );
	  vec3 halfDir = normalize ( incident + viewDir );

	  float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
	  float sFactor = pow ( rFactor , 2.0 );
	  vec3 colour = ( diffuse.rgb * allLights[i].lightColour.rgb );
	  colour += ( allLights[i].lightColour.rgb * sFactor ) * 0.33;
	  tempColour = vec4 ( colour * atten * lambert , diffuse.a );
	  tempColour.rgb += ( diffuse.rgb * allLights[i].lightColour.rgb ) * allLights[i].ambient;
	  FragColor += tempColour;
  }
}


