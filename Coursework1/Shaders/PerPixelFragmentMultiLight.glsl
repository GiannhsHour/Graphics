# version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex1;
uniform sampler2D diffuseTex2;
uniform sampler2D bumpTex ;
uniform sampler2D bumpTex1 ;
uniform sampler2D bumpTex2 ;
uniform sampler2DShadow shadowTex ;

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
 vec3 tangent ; 
 vec3 binormal ; 
 vec3 worldPos ;
 vec4 shadowProj ;
 } IN ;

 out vec4 FragColor ;
void main ( void ) {
 vec4 diffuse;
  vec3 normal = IN.normal;
  mat3 TBN = mat3 ( IN . tangent , IN . binormal , IN . normal );
  
  //if it is terrain
if(type == 1){
	if(IN.worldPos.y>500){
			diffuse = texture ( diffuseTex1 , IN.texCoord ) * (1.0-(750-IN.worldPos .y)/250) + texture ( diffuseTex , IN.texCoord ) * ((750-IN.worldPos .y)/250);
			normal = normalize ( TBN * ( texture ( bumpTex1 ,IN . texCoord ). rgb * 2.0 - 1.0))* (1.0-(750-IN.worldPos .y)/250) + normalize ( TBN * ( texture ( bumpTex ,IN . texCoord ). rgb * 2.0 - 1.0))*((750-IN.worldPos .y)/250);
		}
		else if(IN.worldPos.y<250){
			diffuse = texture ( diffuseTex2 , IN.texCoord ) * (1.0-IN.worldPos.y/250) + texture ( diffuseTex , IN.texCoord ) * (IN.worldPos.y/250);
			normal = normalize ( TBN * ( texture ( bumpTex2 ,IN . texCoord ). rgb * 2.0 - 1.0))* (1.0-IN.worldPos.y/250) + (normalize ( TBN * ( texture ( bumpTex ,IN . texCoord ). rgb * 2.0 - 1.0))*(IN.worldPos.y/250));
	
		}
		else{
			diffuse  = texture ( diffuseTex , IN.texCoord) ;
			normal = normalize ( TBN * ( texture ( bumpTex ,IN . texCoord ). rgb * 2.0 - 1.0));
		}
}
//other objects
  else {diffuse  = texture ( diffuseTex , IN.texCoord );
		normal = normalize ( TBN * ( texture ( bumpTex ,IN . texCoord ). rgb * 2.0 - 1.0));
  }
  
   diffuse.a = 1.0f;
  FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
  for(int i =0; i< numLights; i++){
      vec4 tempColour;
	  vec3 incident = normalize ( allLights[i].lightPos - IN.worldPos );
	  float lambert = max (0.0 , dot ( incident , normal ));
	  float dist = length ( allLights[i].lightPos - IN.worldPos );
	  float atten = 1.0 - clamp ( dist / allLights[i].lightRadius , 0.0 , 1.0);
	  vec3 viewDir = normalize ( cameraPos - IN.worldPos );
	  vec3 halfDir = normalize ( incident + viewDir );

	  float rFactor = max (0.0 , dot ( halfDir , normal )) * 0.2;
	  float sFactor = pow ( rFactor , 5.0 );
	  float shadow = 1.0; // New !

	//  if( IN . shadowProj . w > 0.0) { // New !
	//	shadow = textureProj ( shadowTex , IN . shadowProj );
	//  }

	  lambert *= shadow ; // New !
	  vec3 colour = ( diffuse.rgb * allLights[i].lightColour.rgb );
	  colour += ( allLights[i].lightColour.rgb * sFactor ) * 0.33;
	  tempColour = vec4 ( colour * atten * lambert , diffuse.a );
	  tempColour.rgb += ( diffuse.rgb * allLights[i].lightColour.rgb ) * allLights[i].ambient;
	  FragColor += tempColour;
  }
}


