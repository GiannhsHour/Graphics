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
	shadowShader = new Shader("../../Shaders/shadowVert.glsl", "../../Shaders/shadowFrag.glsl");
	currentShader = sceneShader;
	projMatrix = Matrix4::Perspective(1.0f, 20000.0f, (float)width / (float)height, 45.0f);
	 
	OBJMesh * m = new OBJMesh();
	m->LoadOBJMesh(MESHDIR"sphere_earth.obj");
	earth_sun = m;

	Light *sunLight = new Light(Vector3(0, 0, 0), Vector4(1, 1, 1, 1), 10000.0f);
	sunLight->SetAmbient(0.01f);
	planetSystemLights.push_back(sunLight);

	Light *earthlight = new Light(Vector3(10000, 4000, 4600), Vector4(1, 1, 1, 1), (RAW_WIDTH * HEIGHTMAP_X)*2.5f);
	earthlight->SetAmbient(0.01f);
	planet1Lights.push_back(earthlight);
	lights = planetSystemLights;

	if (!sceneShader->LinkProgram() || !planetShader->LinkProgram() || !skyboxShader->LinkProgram() || !shadowShader->LinkProgram()){
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

	
	
	heightMap1->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"rock.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS),0);
	heightMap1->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"snow.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	heightMap1->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"new_grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 2);
	heightMap1->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"rock_norm.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS),0);
	heightMap1->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"snow_normal.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	heightMap1->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"new_grass_norm.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 2);


	heightMap2->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"red_planet.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	heightMap2->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"red_planet_normal.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);


	//Planet 1
	if (!heightMap1->GetTexture(0) || !cubeMap ) {
		return;
	}
	for (int i = 0; i < 3; i++) {
		SetTextureRepeating(heightMap1->GetTexture(i), true); 
		SetTextureRepeating(heightMap1->GetBumpMap(i), true);
	}
	SetTextureRepeating(heightMap2->GetTexture(0), true); SetTextureRepeating(heightMap2->GetBumpMap(0), true);

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
	planet1scene->SetTransform(Matrix4::Translation(Vector3(3500, 40.0f, 5000.0f))*Matrix4::Rotation(-10,Vector3(0,1,0))*Matrix4::Scale(Vector3(1.0f, 0.6f, 0.6f)));
	planet1scene->getWallMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/wall.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	planet1scene->getWallMesh()->SetBumpMap(SOIL_load_OGL_texture("../../Textures/wall_normal.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	planet1scene->getPlantMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	SetTextureRepeating(planet1scene->getWallMesh()->GetTexture(0), true);
	SetTextureRepeating(planet1scene->getWallMesh()->GetBumpMap(0), true);
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
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/sun.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/red_planet.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/4096_earth.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 2);
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/moon.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 3);
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/4096_night_lights.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 4);
	sphere->SetBumpMap(SOIL_load_OGL_texture("../../Textures/sun_normal.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	sphere->SetBumpMap(SOIL_load_OGL_texture("../../Textures/red_planet_normal.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 1);
	sphere->SetBumpMap(SOIL_load_OGL_texture("../../Textures/4096_normal.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 2);
	for (int i = 0; i < 5; i++){
		if (!sphere->GetTexture(i)) {
			return;
		}
		SetTextureRepeating(sphere->GetTexture(i), true);
	}
	if (!sphere->GetBumpMap(2)) {
		return;
	}
	SetTextureRepeating(sphere->GetBumpMap(0),true); SetTextureRepeating(sphere->GetBumpMap(1), true); SetTextureRepeating(sphere->GetBumpMap(2), true);
	root3->AddChild(system);
	//shadow
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShader);
	Vector3 target = Vector3(0, 0, 0);
	if (root == root1) {
		target = Vector3(3500, 400, 5000);
	}
	viewMatrix = Matrix4::BuildViewMatrix(lights[0]->GetPosition(), target);
	shadowMatrix = biasMatrix *(projMatrix * viewMatrix);

	UpdateShaderMatrices();

	DrawNodes();

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::DrawSun() {
	modelMatrix = Matrix4::Translation(Vector3(lights[0]->GetPosition().x, lights[0]->GetPosition().y, lights[0]->GetPosition().z))*
	Matrix4::Scale(Vector3(200, 200, 200));
	Matrix4 tempMatrix = shadowMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *& tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *& modelMatrix.values);

	earth_sun->Draw();

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
		DrawShadowScene();
		SetCurrentShader(sceneShader);
	}
	else if (root == root3) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
		DrawSkybox();
		lights = planetSystemLights;
		DrawShadowScene();
		SetCurrentShader(planetShader);
	}
	//glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex1"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex2"), 2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex3"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex4"), 4);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 5);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex1"), 6);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex2"), 7);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex3"), 8);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex4"), 9);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 10);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());
	SetShaderLight(lights);

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	viewMatrix = camera->BuildViewMatrix();

	UpdateShaderMatrices();
	SetShaderLight(lights);

	DrawNodes();
	DrawSun();

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
