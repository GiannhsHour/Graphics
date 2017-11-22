#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/Frustum.h"
#include "../../nclgl/HeightMap.h"
#include "PlanetSystem.h"
#include "Planet1Scene.h"
#include "TextMesh.h"
#include "ParticleEmitter.h"
#include <algorithm>
#include <time.h> 

#define SHADOWSIZE 2048

class Renderer : public OGLRenderer {
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();
	Light* getLight(int index) { return lights[index]; }
	Camera* getCamera() { return camera; }
	void setRoot(int s) {
		switch (s)
		{
		case 1: scene = 1;
			break;
		case 2: scene = 2;
			break;
		case 3: scene = 3;
			break;
		case 4: scene = 4;
			break;
		default:
			break;
		}
	}

	bool getPlanetEnter() { return planetEnter; }
	void setPlanetEnter(bool flag) { 
		if(canEnterPlanet) planetEnter = flag; 
		}

	bool getCanEnterPlanet() { return canEnterPlanet; }
	void setCanEnterPlanet(bool flag) { canEnterPlanet = flag; }

	void setGoToSpace(bool flag) { goToSpace = flag; }
	void setSinceLastTime(float s) { sinceLastTime = s; }

	void teleport();

	void	DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

protected:
	void DrawNode(SceneNode * n);
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawSkybox();
	void DrawShadowScene();
	void DrawSun();
	void SetShaderParticleSize(float f);	
	void drawScene(int scene);

	ParticleEmitter*	emitter;	

	clock_t t;
	SceneNode* root;
	SceneNode * root1;
	SceneNode * root2;
	SceneNode * root3;

	Planet1Scene * planet1Scene;
	Planet1Scene * planet2Scene;
	PlanetSystem * planetSystem;


	Camera * camera;
	Mesh* quad;
	HeightMap* heightMap1;
	HeightMap* heightMap2;
	Mesh* earth_sun;
	Shader* sceneShader;
	Shader* planetShader;
	Shader* skyboxShader;
	Shader * shadowShader;
	Shader * textShader;
	Shader * particleShader;
	Shader * fadeOutShader;

	GLuint shadowTex;
	GLuint shadowFBO;

	Frustum frameFrustum;
	
	GLuint cubeMap;
	GLuint cubeMap2;
	GLuint cubeMap3;

	vector<Light *> lights;
	vector<Light *> planetSystemLights;
	vector<Light *> planet1Lights;
	vector<Light *> planet2Lights;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;

	Font*	basicFont;

	bool planetEnter;
	bool canEnterPlanet;
	bool goToSpace;

	float sinceLastTime;
	bool fadeOut;
	bool fadeIn;
	bool transition;
	
	float fadeOutAlpha;
	int visitPlanet;

	int scene;

	int currentWidth;
	int currentHeight;

};