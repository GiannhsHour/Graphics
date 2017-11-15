#include "Renderer.h"


Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	PlanetSystem::CreatePlanetSystem(); // Important !
	camera = new Camera(0,180,Vector3(0,0,0));
	heightMap1 = new HeightMap("../../Textures/terrain.raw");
	heightMap2 = new HeightMap("../../Textures/terrain2.raw");
	sceneShader = new Shader("../../Shaders/PerPixelVertex.glsl", "../../Shaders/PerPixelFragmentMultiLight.glsl");
	planetShader = new Shader("../../Shaders/PerPixelVertex.glsl", "../../Shaders/PerPixelFragmentMultiLightPlanets.glsl");
	currentShader = sceneShader;
	projMatrix = Matrix4::Perspective(1.0f, 20000.0f, (float)width / (float)height, 45.0f);
	lights.push_back(new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X ) * 1.5f, 1500.0f, (RAW_HEIGHT * HEIGHTMAP_Z )), Vector4(1, 1, 1, 1), (RAW_WIDTH * HEIGHTMAP_X *2)));
	Light* light2 = new Light(Vector3(1600,1700,1100), Vector4(1, 1, 1, 1), (RAW_WIDTH * HEIGHTMAP_X));
	//light2->SetColour(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	lights.push_back(light2);


	if (!sceneShader->LinkProgram() || !planetShader->LinkProgram()){
		return;
	}
	
	heightMap1->SetTexture(SOIL_load_OGL_texture("../../Textures/wall.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS),0);
	heightMap1->SetTexture(SOIL_load_OGL_texture("../../Textures/snow2.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	heightMap1->SetTexture(SOIL_load_OGL_texture("../../Textures/new_grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 2);

	heightMap2->SetTexture(SOIL_load_OGL_texture("../../Textures/wall.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);


	if (!heightMap1->GetTexture(0)) {
		return;
	}
	for (int i = 0; i < 3; i++) {
		SetTextureRepeating(heightMap1->GetTexture(i), true); 	
	}
	SetTextureRepeating(heightMap2->GetTexture(0), true);

	root1 = new SceneNode();
	root2 = new SceneNode();
	root3 = new SceneNode();
	root = root1;
	

	SceneNode* scene1 = new SceneNode();
	scene1->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	scene1->SetTransform(Matrix4::Translation(Vector3(0, 0.0f, 0.0f)));
	scene1->SetModelScale(Vector3(2.0f, 3.0f, 2.0f));
	scene1->SetBoundingRadius(10000.0f);
	scene1->SetMesh(heightMap1);
	scene1->setType(1);
	root1->AddChild(scene1);

	SceneNode* scene2 = new SceneNode();
	scene2->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	scene2->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
	scene2->SetModelScale(Vector3(2.0f, 2.0f, 2.0f));
	scene2->SetBoundingRadius(10000.0f);
	scene2->SetMesh(heightMap2);
	scene2->setType(2);
	root2->AddChild(scene2);

	PlanetSystem * system = new PlanetSystem();
	Mesh* sphere = system->getMesh();
	system->setType(2);
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/sun.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/planet_red.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/planet_green.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 2);
	if (!sphere->GetTexture(0)) {
		return;
	}
	SetTextureRepeating(sphere->GetTexture(0), true); SetTextureRepeating(sphere->GetTexture(1), true); SetTextureRepeating(sphere->GetTexture(2), true);
	root3->AddChild(system);

	/*CubeRobot *wall = new CubeRobot();
	Mesh* cube = wall->getMesh();
	cube->SetTexture(SOIL_load_OGL_texture("../../Textures/wall.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	cube->SetTexture(SOIL_load_OGL_texture("../../Textures/wall.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	cube->SetTexture(SOIL_load_OGL_texture("../../Textures/wall.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 2);
	if (!cube->GetTexture(0)) {
		return;
	}
	SetTextureRepeating(cube->GetTexture(0), true); SetTextureRepeating(cube->GetTexture(1), true); SetTextureRepeating(cube->GetTexture(2), true);
	wall->SetTransform(Matrix4::Translation(Vector3(800.0f, 50.0f, 800.0f)));
	wall->setType(2);*/
	//root->AddChild(wall);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	init = true;
}

Renderer ::~Renderer(void) {
	delete root1;
	delete root2;
	PlanetSystem::DeletePlanetSystem(); // Also important !
	delete heightMap1;
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

	if(root == root1 || root == root2) currentShader = sceneShader;
	else if (root == root3) currentShader = planetShader;
	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex1"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex2"), 2);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());


	UpdateShaderMatrices();
	SetShaderLight(lights);
	glEnable(GL_DEPTH_TEST);
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
	 
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "type"), (int)n->getType());
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