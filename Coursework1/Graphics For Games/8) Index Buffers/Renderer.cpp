#include "Renderer.h"




Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	t = clock();
	changeScene = true;
	rain = true;
	fog = 1;
	planetEnter = false;
	total_time = 0;
	thunderStart = false;
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
	projMatrix = Matrix4::Perspective(1.0f, 17000.0f, (float)width / (float)height, 45.0f);
	 
	//draw cubemaps and fade out (in) screens
	quad->SetTexture(SOIL_load_OGL_texture("../../Textures/black.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0), 0);


	OBJMesh * m = new OBJMesh();
	m->LoadOBJMesh(MESHDIR"sphere_earth.obj");
	earth_sun = m;

	//lights

	Light *sunLight = new Light(Vector3(0, 0, 0), Vector4(1, 1, 1, 1), 15000.0f);
	sunLight->SetAmbient(0.01f);
	planetSystemLights.push_back(sunLight);

	Light *earthlight = new Light(Vector3(10000, 4000, 4600), Vector4(1, 1, 1, 1), 12000);
	thunderLight = new Light(Vector3(9000, 3500, 5500), Vector4(1, 1, 1, 1), 14000);
	thunderLight->SetColour(Vector4(0, 0, 0,1));
	earthlight->SetAmbient(0.01f);
	thunderLight->SetAmbient(0.00f);
	planet1Lights.push_back(earthlight);
	planet1Lights.push_back(thunderLight);

	Light *redPlanetLight = new Light(Vector3(15000, 7000, 15000), Vector4(1, 1, 1, 1), 35000);
	redPlanetLight->SetAmbient(0.02f);
	planet2Lights.push_back(redPlanetLight);
	
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

	cubeMap3 = SOIL_load_OGL_cubemap(
		TEXTUREDIR"cubemap_redPlanet/mandaris_ft.tga", TEXTUREDIR"cubemap_redPlanet/mandaris_bk.tga",
		TEXTUREDIR"cubemap_redPlanet/mandaris_up.tga", TEXTUREDIR"cubemap_redPlanet/mandaris_dn.tga",
		TEXTUREDIR"cubemap_redPlanet/mandaris_rt.tga", TEXTUREDIR"cubemap_redPlanet/mandaris_lf.tga",
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
	scene = 3;
	

	//planet earth
	SceneNode* scene1 = new SceneNode();
	scene1->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	scene1->SetTransform(Matrix4::Translation(Vector3(0, 0.0f, 0.0f)));
	scene1->SetModelScale(Vector3(2.0f, 3.0f, 2.0f));
	scene1->SetBoundingRadius(10000.0f);
	scene1->SetMesh(heightMap1);
	scene1->setType(1);
	root1->AddChild(scene1);

	//house in planet earth
	planet1Scene = new Planet1Scene();
	planet1Scene->SetTransform(Matrix4::Translation(Vector3(3500, 85.0f, 5000.0f))*Matrix4::Rotation(-10,Vector3(0,1,0))*Matrix4::Scale(Vector3(1.0f, 0.6f, 0.6f)));
	planet1Scene->getWallMesh()->SetTexture(SOIL_load_OGL_texture("../../Textures/wall.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	planet1Scene->getWallMesh()->SetBumpMap(SOIL_load_OGL_texture("../../Textures/wall_normal.PNG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS), 0);
	SetTextureRepeating(planet1Scene->getWallMesh()->GetTexture(0), true);
	SetTextureRepeating(planet1Scene->getWallMesh()->GetBumpMap(0), true);
	scene1->AddChild(planet1Scene);

	//rain particles emitter
	emitter = new ParticleEmitter();
	SceneNode * rain = new SceneNode();
	rain->SetTransform(Matrix4::Translation(Vector3(0, 500, 0)));
	rain->SetMesh(emitter);
	rain->SetBoundingRadius(500.0f);
	scene1->AddChild(rain);
	
	//red planet
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
	for (int i = 0; i < 2; i++) {
		glGenTextures(1, &shadowTex[i]);
		glBindTexture(GL_TEXTURE_2D, shadowTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
			GL_COMPARE_R_TO_TEXTURE);

		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &shadowFBO[i]);

		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex[i], 0);
		glDrawBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	



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
	for (int i = 0; i < lights.size(); i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO[i]);

		glClear(GL_DEPTH_BUFFER_BIT);

		glViewport(1, 1, SHADOWSIZE - 2, SHADOWSIZE - 2);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		SetCurrentShader(shadowShader);
		Vector3 target = Vector3(0, 0, 0);
		if (root == root1) {
			target = Vector3(3500, 400, 5000);
		}
	
		viewMatrix = Matrix4::BuildViewMatrix(lights[i]->GetPosition(), target);
		shadowMatrix[i] = biasMatrix *(projMatrix * viewMatrix);

		UpdateShaderMatrices();

		DrawNodes();

		glUseProgram(0);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glViewport(0, 0, width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}

/* Draw scenes
   param sc: scene to draw*/
void Renderer::drawScene(int sc) {
	glDisable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE0);
	if (sc == 1) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap2);
	}
	else if (sc == 2) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap3);
	}
	else {
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	}
	DrawSkybox();

	SetCurrentShader(textShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	
	string fps = to_string(1000 / sinceLastTime);
	fps = fps.substr(0, 4);
	DrawText("FPS : " + fps, Vector3(0, 0, 0), 16.0f);
	DrawText("Change scenes ( 1, 2, 3, 4 ) ", Vector3(0, 15, 0), 15.0f);
	DrawText("1 : Planet Earth ", Vector3(0, 30, 0), 15.0f);
	DrawText("2 : Red Planet ", Vector3(0, 45, 0), 15.0f);
	DrawText("3 : Space ", Vector3(0, 60, 0), 15.0f);
	DrawText("4 : Split Screen Both Planets", Vector3(0, 75, 0), 15.0f);
	DrawText("(O,P) : Adjust ambient light", Vector3(0, 90, 0), 15.0f);
	DrawText("(K,L) : Adjust light radius", Vector3(0, 105, 0), 15.0f);
	if (sc == 1) {
		root = root1;
		BuildNodeLists(root);
		SortNodeLists();
		DrawText("(R) : Rotate sun", Vector3(0, 120, 0), 15.0f);
		DrawText("(G , F) : Toggle rain , Toggle Fog", Vector3(0, 135, 0), 15.0f);
		DrawText("Go to Space (Q)", Vector3(width / 3, height - 18, 0), 18.0f);
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 17000.0f, (float)width / (float)height, 45.0f);
		
		lights = planet1Lights;
		DrawShadowScene();
		if (scene == 4) {
			glViewport(0, 0, width / 2, height);
			projMatrix = Matrix4::Perspective(1.0f, 17000.0f, (float)width / 2 / (float)height, 45.0f);
		}
		SetCurrentShader(sceneShader);
	}
	else if (sc == 2) {
		root = root2;
		BuildNodeLists(root);
		SortNodeLists();
		DrawText("Go to Space (Q)", Vector3(width / 3, height - 18, 0), 18.0f);
		DrawText("(R) : Rotate sun", Vector3(0, 120, 0), 15.0f);
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 17000.0f, (float)width / (float)height, 45.0f);
	
		lights = planet2Lights;
		DrawShadowScene();
		if (scene == 4) {
			glViewport(width / 2, 0, width / 2, height);
			projMatrix = Matrix4::Perspective(1.0f, 17000.0f, (float)width / 2 / (float)height, 45.0f);
		}
		SetCurrentShader(sceneShader);

	}
	else if (sc == 3) {
		root = root3;
		BuildNodeLists(root);
		SortNodeLists();
		float distanceFromPlanetEarth = (camera->GetPosition() - planetSystem->getEarthPosition()).Length();
		float distanceFromPlanetRed = (camera->GetPosition() - planetSystem->getRedPlanetPosition()).Length();
		if (distanceFromPlanetEarth < 2000.0f && root == root3) {
			DrawText("Enter Planet Earth (E)", Vector3(width / 4, height / 4, 0), 20.0f);
			canEnterPlanet = true;
			if (planetEnter) {
				transition = true;
				fadeOut = true;
				visitPlanet = 1;
			}
		}
		else if (distanceFromPlanetRed < 3000.0f && root == root3) {
			DrawText("Enter Red Planet (E)", Vector3(width / 4, height / 4, 0), 20.0f);
			canEnterPlanet = true;
			if (planetEnter) {
				transition = true;
				fadeOut = true;
				visitPlanet = 2;
			}
		}
		else canEnterPlanet = false;
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 17000.0f, (float)width / (float)height, 45.0f);

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
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex1"), 10);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex2"), 11);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "fog"), fog);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());



	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, shadowTex[0]);

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, shadowTex[1]);


	viewMatrix = camera->BuildViewMatrix();

	UpdateShaderMatrices();
	SetShaderLight(lights);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	DrawNodes();

	SetCurrentShader(particleShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	//rain and thunder only in planet earth
	if (root == root1 && rain) {
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
				modelMatrix = Matrix4::Translation(Vector3(350 * i + ran % 350, ran % 500 + 2500, 350 * j + ran % 350));
				UpdateShaderMatrices();
				emitter->Draw();
			}
		}
		if(!thunderStart) thunderTime = rand()*20;
		if (thunderTime%283 == 0 && !thunderStart) {
			thunderStart = true;
			thunderLight->SetColour(Vector4(1, 1, 1, 1));	
		}
		thunder();
		
	}
	ClearNodeLists();
}

void Renderer::thunder() {
	if (thunderStart) {
		total_time += sinceLastTime*1000;
		if (total_time > (float)thunderTime) {
			thunderLight->SetColour(Vector4(0, 0, 0, 1));
			thunderStart = false;
			total_time = 0;
		}
	}
}

void Renderer::RenderScene() {

	if (goToSpace) {
		if (!transition) {
			transition = true;
			fadeOut = true;
		}
		goToSpace = false;
		visitPlanet = 3;
	}

	
	if (scene == 1) {
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawScene(1);
		teleport();
	}
	if (scene == 2) {
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawScene(2);
		teleport();
	}
	if (scene == 3) {
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawScene(3);
		teleport();
	}
	if (scene == 4) {
		glEnable(GL_SCISSOR_TEST);
		glViewport(0, 0, width/2, height);
		glScissor(0, 0, width / 2, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawScene(1);
		teleport();
		glViewport(width / 2, 0, width / 2, height);
		glScissor(width / 2, 0, width / 2, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawScene(2);
		teleport();
	}

	sceneChange();

	glUseProgram(0);
	SwapBuffers();
	
}
void	Renderer::SetShaderParticleSize(float f) {
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), f);
}

//Fade out / In
void Renderer::teleport() {
	if (transition) {
		if (fadeOut) {
			fadeOutAlpha += 0.008;
			if (fadeOutAlpha >= 1.0) {
				if (visitPlanet == 1) scene = 1;
				else if (visitPlanet == 2) scene = 2;
				else scene = 3;
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

void Renderer::sceneChange() {
	if (changeScene) {
		total_scene_time += sinceLastTime;
		if (total_scene_time > 3000) {
			scene = scene % 4 + 1;
			total_scene_time = 0;
		}
	}
}
