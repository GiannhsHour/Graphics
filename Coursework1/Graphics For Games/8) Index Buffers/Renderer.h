#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/Frustum.h"
#include "../../nclgl/HeightMap.h"
#include "PlanetSystem.h"
#include "Planet1Scene.h"
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
	void setRoot(int scene) {
		switch (scene)
		{
		case 1: root = root1;
			break;
		case 2: root = root2;
			break;
		case 3: root = root3;
			break;
		default:
			break;
		}
	}

	void jump() { jump_bool = true; }

protected:
	void DrawNode(SceneNode * n);
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawSkybox();
	void DrawShadowScene();
	void DrawSun();

	void jump(float msec);
	clock_t t;
	SceneNode* root;
	SceneNode * root1;
	SceneNode * root2;
	SceneNode * root3;
	Camera * camera;
	Mesh* quad;
	HeightMap* heightMap1;
	HeightMap* heightMap2;
	Mesh* earth_sun;
	Shader* sceneShader;
	Shader* planetShader;
	Shader* skyboxShader;
	Shader * shadowShader;

	GLuint shadowTex;
	GLuint shadowFBO;

	Frustum frameFrustum;
	
	GLuint cubeMap;
	GLuint cubeMap2;

	vector<Light *> lights;
	vector<Light *> planetSystemLights;
	vector<Light *> planet1Lights;
	vector<Light *> planet2Lights;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;


	bool jump_bool = false;
	float timeIn = 0.0f;
	float timeIn2 = 0.0f;
	bool time_flag = true;
	bool time_flag2 = true;
	bool alt_bool = false;
};