#include "Renderer.h"

Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	CubeRobot::CreateCube(); // Important !
	camera = new Camera();
	heightMap = new HeightMap("../../Textures/terrain.raw");
	currentShader = new Shader("../../Shaders/TexturedVertexTest.glsl", "../../Shaders/TexturedFragmentTest.glsl");
	
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	
	camera -> SetPosition(Vector3(500, 200, 500));

	quad = Mesh::GenerateQuad();
	quad->SetTexture(SOIL_load_OGL_texture("../../Textures/stainedglass.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0),0);

	if (!currentShader->LinkProgram() || !quad->GetTexture(0)) {
		return;
	}

	heightMap->SetTexture(SOIL_load_OGL_texture("../../Textures/Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0),0);
	heightMap->SetTexture(SOIL_load_OGL_texture("../../Textures/snow2.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0), 1);
	heightMap->SetTexture(SOIL_load_OGL_texture("../../Textures/grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0), 2);


	if (!heightMap->GetTexture(0)) {
		return;
	}

	SetTextureRepeating(heightMap->GetTexture(0), true); SetTextureRepeating(heightMap->GetTexture(1), true); SetTextureRepeating(heightMap->GetTexture(2), true);
	root = new SceneNode();
	
	for (int i = 0; i < 1; i++) {
		SceneNode* s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		//s->SetTransform(Matrix4::Translation(Vector3(0, 100.0f, -300.0f + 100.0f + 100 * i)));
		s->SetTransform(Matrix4::Translation(Vector3(0, 0.0f, 0.0f)));
		s->SetModelScale(Vector3(1.0f, 1.0f, 1.0f));
		s->SetBoundingRadius(10000.0f);
		s->SetMesh(heightMap);
		root->AddChild(s);
	}
	CubeRobot *robotaki = new CubeRobot();
	robotaki->SetTransform(Matrix4::Translation(Vector3(800, 50.0f, 800)));
	root->AddChild(robotaki);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	init = true;
}

Renderer ::~Renderer(void) {
	delete root;
	CubeRobot::DeleteCube(); // Also important !
	delete heightMap;
}

void Renderer::UpdateScene(float msec) {
	camera -> UpdateCamera(msec);
	viewMatrix = camera -> BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);
	root -> Update(msec);
}

void Renderer::RenderScene() {
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	glUseProgram(currentShader -> GetProgram());
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader -> GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader -> GetProgram(), "diffuseTex1"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex2"), 2);
	//heightMap->Draw();
	DrawNodes();

	glUseProgram(0);
	SwapBuffers();
	ClearNodeLists();
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::DrawNode(SceneNode * n) {
	if (n -> GetMesh()) {
		Matrix4 transform = n -> GetWorldTransform()*Matrix4::Scale(n -> GetModelScale());

		glUniformMatrix4fv(glGetUniformLocation(currentShader -> GetProgram(), "modelMatrix"), 1, false, (float *)& transform);

		glUniform4fv(glGetUniformLocation(currentShader -> GetProgram(), "nodeColour"), 1, (float *)& n -> GetColour());

		glUniform1i(glGetUniformLocation(currentShader -> GetProgram(), "useTexture"), (int)n -> GetMesh() -> GetTexture(0));

		n -> Draw(*this);
	}
}

void Renderer::BuildNodeLists(SceneNode* from) {
	if (frameFrustum.InsideFrustum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));
		/*
		float alpha = 0;
		glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_ALPHA_SIZE, &alpha);
		if (alpha > 0.0f) {
		}*/
		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else {
			nodeList.push_back(from);
		}
		
    }
	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i) {
		SceneNode* thisNode = (*i);
		BuildNodeLists(thisNode);
	}
	
}

void Renderer::SortNodeLists() {
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes() {
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i) {
		DrawNode((*i));
	}
	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i) {
		DrawNode((*i));
	}
}