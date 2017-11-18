#include "Renderer.h"



void Renderer::jump(float time) {
	if (jump_bool) {
		if (time_flag2) {
			//timeIn2 einai h xronikh stigmh pou ksekinaei to alma
			//ypologizetai mia fora logw tou time_flag2
			timeIn2 = time;
			time_flag2 = false;
		}
		//taxuthta almatos
		float jump_speed = 5.0f*0.0008f;
		//ti tha prosthesei ston aksona "y"
		float sine = sin((time - timeIn2)*jump_speed);

		//an einai arnhtiko shmainei oti h camera ksana sikwnetai
		//alla pio grhgora kai me mikroteri apostash
		if (sine<0.0f) {
			//jump_speed * 4.0f gia na sikwthei 4 fores pio grhgora kai olo dia 4 gia 4 fores mikroterh apostash
			sine = -sin((time - timeIn2)*jump_speed*4.0f) / 4.0f;
		}
		else sine = abs(sine);

		//to alma oloklirwnetai se:
		// PI o xronos gia na sikwthei kai na katevei kai
		// PI/4 o xronos ths epanaforas, sunolika 5PI/4
		// kai olo auto dia to jump_speed gia na metatrepsoume se pragmatiko xrono
		if (time - timeIn2 < (5.0f / 4.0f)*PI / jump_speed) {
			float high = 0.6f*sine;
			//float x = cameraTransform->GetTranslation().x;
			float x = 0;
			//float y = 1.6f + high;
			float y = 1.6f + high;
			//float z = cameraTransform->GetTranslation().z;
			float z = 0;
			camera->SetPosition(Matrix4::Translation(Vector3(x, y, z))*camera->GetPosition());
			//cameraTransform->SetTranslation(x, y, z);
		//	if (alt_bool&&time - timeIn2 < (PI / jump_speed)) camera_rotate -= speed*0.75;
		}
		else {
			jump_bool = false;
			time_flag2 = true;
			alt_bool = false;
			float x = 0;
			float y = 1.6f;
			float z = 0;
			camera->SetPosition(Matrix4::Translation(Vector3(x, y, z))*camera->GetPosition());
		}
	}
}

Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	t = clock();
	PlanetSystem::CreatePlanetSystem(); 
	Planet1Scene::CreatePlanet1Scene();
	quad = Mesh::GenerateQuad();
	camera = new Camera(0,180,Vector3(3000,400,4500));
	heightMap1 = new HeightMap("../../Textures/terrain.raw");
	heightMap2 = new HeightMap("../../Textures/terrain2.raw");
	sceneShader = new Shader("../../Shaders/PerPixelVertex.glsl", "../../Shaders/PerPixelFragmentMultiLight.glsl");
	planetShader = new Shader("../../Shaders/PerPixelVertex.glsl", "../../Shaders/PerPixelFragmentMultiLightPlanets.glsl");
	skyboxShader = new Shader("../../Shaders/skyboxVertex.glsl", "../../Shaders/skyboxFragment.glsl");
	currentShader = sceneShader;
	projMatrix = Matrix4::Perspective(1.0f, 20000.0f, (float)width / (float)height, 45.0f);

	Light *sunLight = new Light(Vector3(0, 0, 0), Vector4(1, 1, 1, 1), 10000.0f);
	sunLight->SetAmbient(0.01f);
	planetSystemLights.push_back(sunLight);

	Light *earthlight = new Light(Vector3(1600, 1700, 1200), Vector4(1, 1, 1, 1), (RAW_WIDTH * HEIGHTMAP_X));
	earthlight->SetAmbient(0.001f);
	planet1Lights.push_back(earthlight);
	lights = planetSystemLights;

	if (!sceneShader->LinkProgram() || !planetShader->LinkProgram() || !skyboxShader->LinkProgram()){
		return;
	}

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"/cubemap_galaxy/bkg1_right.JPG", TEXTUREDIR"/cubemap_galaxy/bkg1_left.JPG",
		TEXTUREDIR"/cubemap_galaxy/bkg1_top.JPG", TEXTUREDIR"/cubemap_galaxy/bkg1_bot.JPG",
		TEXTUREDIR"/cubemap_galaxy/bkg1_front.JPG", TEXTUREDIR"/cubemap_galaxy/bkg1_back.JPG",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0);

	cubeMap2 = SOIL_load_OGL_cubemap(
		TEXTUREDIR"rusted_west.JPG", TEXTUREDIR"rusted_east.JPG",
		TEXTUREDIR"rusted_up.JPG", TEXTUREDIR"rusted_down.JPG",
		TEXTUREDIR"rusted_south.JPG", TEXTUREDIR"rusted_north.JPG",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0);

	
	
	quad -> SetTexture(SOIL_load_OGL_texture("../../Textures/wall.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS),0);
	SetTextureRepeating(quad->GetTexture(0), true);
	
	heightMap1->SetTexture(SOIL_load_OGL_texture("../../Textures/wall.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS),0);
	heightMap1->SetTexture(SOIL_load_OGL_texture("../../Textures/snow2.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	heightMap1->SetTexture(SOIL_load_OGL_texture("../../Textures/grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 2);

	heightMap2->SetTexture(SOIL_load_OGL_texture("../../Textures/wall.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);


	if (!heightMap1->GetTexture(0) || !cubeMap || !quad->GetTexture(0)) {
		return;
	}
	for (int i = 0; i < 3; i++) {
		SetTextureRepeating(heightMap1->GetTexture(i), true); 	
	}
	SetTextureRepeating(heightMap2->GetTexture(0), true);

	root1 = new SceneNode();
	root2 = new SceneNode();
	root3 = new SceneNode();
	root = root3;
	

	SceneNode* scene1 = new SceneNode();
	scene1->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	scene1->SetTransform(Matrix4::Translation(Vector3(0, 0.0f, 0.0f)));
	scene1->SetModelScale(Vector3(2.0f, 3.0f, 2.0f));
	scene1->SetBoundingRadius(10000.0f);
	scene1->SetMesh(heightMap1);
	scene1->setType(1);
	root1->AddChild(scene1);

	Planet1Scene * planet1scene = new Planet1Scene();
	planet1scene->SetTransform(Matrix4::Translation(Vector3(3500, 40.0f, 5000.0f))*Matrix4::Scale(Vector3(2.0f,1.2f,1.2f)));
	planet1scene->getWallMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/house/wall.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	planet1scene->getPlantMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	SetTextureRepeating(planet1scene->getWallMesh()->GetTexture(0), true);
	SetTextureRepeating(planet1scene->getPlantMesh()->GetTexture(0), true);
	scene1->AddChild(planet1scene);
	

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
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/sun.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/planet_red.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/4096_earth.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 2);
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/moon.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 3);
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/4096_night_lights.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 4);
	for (int i = 0; i < 5; i++){
		if (!sphere->GetTexture(i)) {
			return;
		}
	}
	SetTextureRepeating(sphere->GetTexture(0), true); SetTextureRepeating(sphere->GetTexture(1), true); SetTextureRepeating(sphere->GetTexture(2), true);
	SetTextureRepeating(sphere->GetTexture(3), true); SetTextureRepeating(sphere->GetTexture(4), true);
	root3->AddChild(system);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	init = true;
}

Renderer ::~Renderer(void) {
	delete root1;
	delete root2;
	PlanetSystem::DeletePlanetSystem();
	delete heightMap1;
	delete heightMap2;
	delete skyboxShader;
	currentShader = 0;
}

void Renderer::UpdateScene(float msec) {
	
	camera -> UpdateCamera(msec);
	viewMatrix = camera -> BuildViewMatrix();
	//jump((float)(clock() - t));
	frameFrustum.FromMatrix(projMatrix*viewMatrix);
	root -> Update(msec);
}

void Renderer::DrawSkybox() {
	 glDepthMask(GL_FALSE);
	 SetCurrentShader(skyboxShader);

	 UpdateShaderMatrices();
	 quad->Draw(false);
	
	 glUseProgram(0);
	 glDepthMask(GL_TRUE);
}


void Renderer::RenderScene() {
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	
	if (root == root1 || root == root2) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap2);
		DrawSkybox();
		lights = planet1Lights;
		SetCurrentShader(sceneShader);
	}
	else if (root == root3) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
		DrawSkybox();
		lights = planetSystemLights;
		SetCurrentShader(planetShader);
	}
	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex1"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex2"), 2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex3"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex4"), 4);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());


	UpdateShaderMatrices();
	SetShaderLight(lights);
	glEnable(GL_DEPTH_TEST);
	//heightMap1->Draw();
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
