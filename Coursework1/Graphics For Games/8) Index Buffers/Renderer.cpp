#include "Renderer.h"




Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	t = clock();
	planetEnter = false;
	canEnterPlanet = false;
    transition = false;
	fadeOut = false;
	fadeIn = false;
	goToSpace = false;
	fadeOutAlpha = 0.0f;
	PlanetSystem::CreatePlanetSystem(); 
	Planet1Scene::CreatePlanet1Scene();
	quad = Mesh::GenerateQuad();
	camera = new Camera(0,180,Vector3(3000,400,4500));
	heightMap1 = new HeightMap("../../Textures/terrain.raw");
	heightMap2 = new HeightMap("../../Textures/terrain2.raw");
	textShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	fadeOutShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"FadeOutFragment.glsl");
	sceneShader = new Shader("../../Shaders/PerPixelVertex.glsl", "../../Shaders/PerPixelFragmentMultiLight.glsl");
	planetShader = new Shader("../../Shaders/PerPixelVertex.glsl", "../../Shaders/PerPixelFragmentMultiLightPlanets.glsl");
	skyboxShader = new Shader("../../Shaders/skyboxVertex.glsl", "../../Shaders/skyboxFragment.glsl");
	shadowShader = new Shader("../../Shaders/shadowVert.glsl", "../../Shaders/shadowFrag.glsl");
	particleShader = new Shader(SHADERDIR"particleVertex.glsl",SHADERDIR"particleFragment.glsl",SHADERDIR"particleGeometry.glsl");
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_COMPRESS_TO_DXT),16,16);
	currentShader = sceneShader;
	projMatrix = Matrix4::Perspective(1.0f, 20000.0f, (float)width / (float)height, 45.0f);
	 
	//draw cubamps and fade out (in) screens
	quad->SetTexture(SOIL_load_OGL_texture("../../Textures/black.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0), 0);


	OBJMesh * m = new OBJMesh();
	m->LoadOBJMesh(MESHDIR"sphere_earth.obj");
	earth_sun = m;

	Light *sunLight = new Light(Vector3(0, 0, 0), Vector4(1, 1, 1, 1), 15000.0f);
	sunLight->SetAmbient(0.01f);
	planetSystemLights.push_back(sunLight);

	Light *earthlight = new Light(Vector3(10000, 4000, 4600), Vector4(1, 1, 1, 1), (RAW_WIDTH * HEIGHTMAP_X)*2.5f);
	earthlight->SetAmbient(0.01f);
	planet1Lights.push_back(earthlight);
	lights = planetSystemLights;

	if (!sceneShader->LinkProgram() || !planetShader->LinkProgram() || !skyboxShader->LinkProgram() || !shadowShader->LinkProgram() 
		||!textShader->LinkProgram() || !particleShader->LinkProgram() || !fadeOutShader->LinkProgram()){
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

	planet1Scene = new Planet1Scene();
	planet1Scene->SetTransform(Matrix4::Translation(Vector3(3500, 85.0f, 5000.0f))*Matrix4::Rotation(-10,Vector3(0,1,0))*Matrix4::Scale(Vector3(1.0f, 0.6f, 0.6f)));
	planet1Scene->getWallMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/wall.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	planet1Scene->getWallMesh()->SetBumpMap(SOIL_load_OGL_texture("../../Textures/wall_normal.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	planet1Scene->getPlantMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	SetTextureRepeating(planet1Scene->getWallMesh()->GetTexture(0), true);
	SetTextureRepeating(planet1Scene->getWallMesh()->GetBumpMap(0), true);
	SetTextureRepeating(planet1Scene->getPlantMesh()->GetTexture(0), true);
	scene1->AddChild(planet1Scene);

	emitter = new ParticleEmitter();
	SceneNode * rain = new SceneNode();
	rain->SetTransform(Matrix4::Translation(Vector3(0, 500, 0)));
	rain->SetMesh(emitter);
	rain->SetBoundingRadius(500.0f);
	scene1->AddChild(rain);
	

	SceneNode* scene2 = new SceneNode();
	scene2->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	scene2->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
	scene2->SetModelScale(Vector3(2.0f, 2.0f, 2.0f));
	scene2->SetBoundingRadius(10000.0f);
	scene2->SetMesh(heightMap2);
	scene2->setType(2);
	root2->AddChild(scene2);


	planetSystem = new PlanetSystem();
	Mesh* sphere = planetSystem->getMesh();
	sphere->SetTexture(SOIL_load_OGL_texture("../../Textures/sun.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
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
	root3->AddChild(planetSystem);
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
	
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	init = true;
}

Renderer ::~Renderer(void) {
	delete root1;
	delete root2;
	delete root3;
	delete root;
	PlanetSystem::DeletePlanetSystem();
	delete heightMap1;
	delete heightMap2;
	delete emitter;
	currentShader = 0;
}

void Renderer::UpdateScene(float msec) {
	
	emitter->Update(msec);
	camera -> UpdateCamera(msec);
	viewMatrix = camera -> BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);
	root -> Update(msec);
}

void Renderer::DrawSkybox() {
	 glDepthMask(GL_FALSE);
	 SetCurrentShader(skyboxShader);

	 UpdateShaderMatrices();
	 quad->Draw();
	
	 glUseProgram(0);
	 glDepthMask(GL_TRUE);
}

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(1, 1, SHADOWSIZE - 2, SHADOWSIZE - 2);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glDisable(GL_CULL_FACE);
	SetCurrentShader(textShader);	
												
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	string fps = to_string(1000 / sinceLastTime);
	fps = fps.substr(0,4);
	DrawText("FPS : " + fps, Vector3(0, 0, 0), 16.0f);
	if(root== root1 || root == root2) DrawText("Go to Space (Q)", Vector3(width/3, 0, 0), 20.0f);
	float distanceFromPlanetEarth = (camera->GetPosition() - planetSystem->getEarthPosition()).Length();
	float distanceFromPlanetRed = (camera->GetPosition() - planetSystem->getRedPlanetPosition()).Length();
	if (distanceFromPlanetEarth < 2000.0f && root == root3) {
		DrawText("Enter Planet Earth (E)", Vector3(width/4, height / 4, 0), 20.0f);
		canEnterPlanet = true;
		if (planetEnter) {
			transition = true;
			fadeOut = true;
			visitPlanet = 1;
		}
	}
	else if (distanceFromPlanetRed < 3000.0f && root == root3) {
		DrawText("Enter Red Planet (E)", Vector3(width /4, height / 4,  0), 20.0f);
		canEnterPlanet = true;
		if (planetEnter) {
			transition = true;
			fadeOut = true;
			visitPlanet = 2;
		}
	}
	else canEnterPlanet = false;
	if (goToSpace) {
		if(!transition) {
			transition = true;
			fadeOut = true;
		}
		goToSpace = false;
		visitPlanet = 3;
	}

	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 17000.0f, (float)width / (float)height, 45.0f);
	
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
	glUseProgram(currentShader->GetProgram());
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
	
	
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	
	viewMatrix = camera->BuildViewMatrix();
	
	UpdateShaderMatrices();
	SetShaderLight(lights);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	DrawNodes();
	DrawSun();

	SetCurrentShader(particleShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	if (root == root1) {
		emitter->SetParticleSize(8.0f);
		emitter->SetParticleVariance(0.10f);
		emitter->SetParticleLifetime(2500.0f);
		emitter->SetLaunchParticles(1.0f);
		emitter->SetParticleSpeed(1.8f);
		SetShaderParticleSize(emitter->GetParticleSize());
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				emitter->SetDirection(Vector3(0, -1, 0));
				int ran = rand();
				modelMatrix = Matrix4::Translation(Vector3(350*i +ran%350, ran % 500 + 2500, 350 * j + ran % 350));
				UpdateShaderMatrices();
				emitter->Draw();
			}
		}
	}
	
	teleport();
	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glUseProgram(0);
	SwapBuffers();
	ClearNodeLists();
}
void	Renderer::SetShaderParticleSize(float f) {
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), f);
}

void Renderer::teleport() {
	if (transition) {
		if (fadeOut) {
			fadeOutAlpha += 0.008;
			if (fadeOutAlpha >= 1.0) {
				if (visitPlanet == 1) root = root1;
				else if (visitPlanet == 2) root = root2;
				else root = root3;
				camera->SetPosition(Vector3(3500, 500, 4000));
				fadeIn = true;
				fadeOut = false;
			}
		}
		else if (fadeIn) {
			fadeOutAlpha -= 0.008;
			if (fadeOutAlpha <= 0.0) {
				fadeIn = false;
				transition = false;
				planetEnter = false;
				canEnterPlanet = false;
			}
		}
		SetCurrentShader(fadeOutShader);
		modelMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
		viewMatrix.ToIdentity();
		glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "alpha"), fadeOutAlpha);
		UpdateShaderMatrices();
		quad->Draw();
		projMatrix = Matrix4::Perspective(1.0f, 17000.0f, (float)width / (float)height, 45.0f);
	}
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


void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);

	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	}
	else {
		//In ortho mode, we subtract the y from the height, so that a height of 0
		//is at the top left of the screen, which is more intuitive
		//(for me anyway...)
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	//Either way, we update the matrices, and draw the mesh
	UpdateShaderMatrices();
	mesh->Draw();

	delete mesh; //Once it's drawn, we don't need it anymore!
}
