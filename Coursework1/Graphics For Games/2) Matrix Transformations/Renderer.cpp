#include "Renderer.h"

Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	 triangle = Mesh::GenerateTriangle();
	 currentShader = new Shader("../../Shaders/MatrixVertex.glsl",
	 "../../Shaders/colourFragment.glsl");

	 if (!currentShader -> LinkProgram()) {
		 return;
	}
	 camera = new Camera();
	 init = true;

	 SwitchToOrthographic();
}

 Renderer ::~Renderer(void) {
	 delete triangle;
}

 void Renderer::SwitchToPerspective() {
	 projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
 }

 void Renderer::SwitchToOrthographic() {
	 projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
 }

 void Renderer::RenderScene() {
	 glClear(GL_COLOR_BUFFER_BIT);
	 
	 glUseProgram(currentShader -> GetProgram());
	 
	 glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
	 , "projMatrix"), 1, false, (float *)& projMatrix);
	 
	 glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
	 , "viewMatrix"), 1, false, (float *)& viewMatrix);

	 
	 
	 for (int i = 0; i < 3; ++i) {
		 Vector3 tempPos = position;
		 tempPos.z += (i *500.0f);
		 tempPos.x -= (i *100.0f);
		 tempPos.y -= (i *100.0f);
		
		 modelMatrix = Matrix4::Rotation(rotation, Vector3(x, y, z)) * Matrix4::Translation(tempPos) * Matrix4::Scale(Vector3(scale, scale, scale));
		 glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float *)& modelMatrix);
		
		 triangle->Draw(); 
	 }
	 glUseProgram(0);
	 SwapBuffers();
	 
 }

 void Renderer::UpdateScene(float msec) {
	 camera->UpdateCamera(msec);
	 viewMatrix = camera->BuildViewMatrix();
 }

 void Renderer::SetRotationAxis(char r) {
	 switch (r)
	 {
	 case 'y': {
		 x = 0.0f; y = 1.0f; z = 0.0f;
		 break;
	 }
	 case 'x': {
		 x = 1.0f; y = 0.0f; z = 0.0f;
		 break;
	 }
	 case 'z': {
		 x = 0.0f; y = 0.0f; z = 1.0f;
		 break;
	 }
	 default:
		 break;
	 }
 }