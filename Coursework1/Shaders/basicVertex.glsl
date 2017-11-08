#version 150 core

in vec3 position ;
in vec4 colour ;


smooth out vec4 VertexColor;


void main ( void ) {
  gl_Position = vec4 ( position , 1.0f);
  VertexColor = colour;
}