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
 vec4 diffuse;
 
	if(IN.worldPos.y>500){
			diffuse = texture ( diffuseTex1 , IN.texCoord ) * (1.0-(750-IN.worldPos .y)/250) + texture ( diffuseTex , IN.texCoord ) * ((750-IN.worldPos .y)/250);
		}
		else if(IN.worldPos.y<200){
			diffuse = texture ( diffuseTex2 , IN.texCoord ) * (1.0-IN.worldPos.y/200) + texture ( diffuseTex , IN.texCoord ) * (IN.worldPos.y/200);
		}
		else{
			diffuse  = texture ( diffuseTex , IN.texCoord );
		}


  vec3 incident = normalize ( lightPos - IN.worldPos );
  float lambert = max (0.0 , dot ( incident , IN.normal ));
  float dist = length ( lightPos - IN.worldPos );
  float atten = 1.0 - clamp ( dist / lightRadius , 0.0 , 1.0);
  vec3 viewDir = normalize ( cameraPos - IN.worldPos );
  vec3 halfDir = normalize ( incident + viewDir );

  float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
  float sFactor = pow ( rFactor , 20.0 );
  vec3 colour = ( diffuse.rgb * lightColour.rgb );
  colour += ( lightColour.rgb * sFactor ) * 0.33;
  gl_FragColor = vec4 ( colour * atten * lambert , diffuse.a );
  gl_FragColor.rgb += ( diffuse.rgb * lightColour.rgb ) * ambient;
}


