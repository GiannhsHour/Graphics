# version 150 core
uniform float alpha;

in Vertex {
  vec4 colour ;
} IN ;

out vec4 FragColor ;

void main ( void ) {
  FragColor =  vec4(0 , 0 , 0 , alpha ) ;
}