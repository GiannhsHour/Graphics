#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/Frustum.h"
#include "../../nclgl/HeightMap.h"
#include "PlanetSystem.h"
#include <algorithm>

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
protected:
	void DrawNode(SceneNode * n);
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawSkybox();

	SceneNode* root;
	SceneNode * root1;
	SceneNode * root2;
	SceneNode * root3;
	Camera * camera;
	Mesh* quad;
	HeightMap* heightMap1;
	HeightMap* heightMap2;
	Shader* sceneShader;
	Shader* planetShader;
	Shader* skyboxShader;
	Frustum frameFrustum;
		GLuint cubeMap;
	vector<Light *> lights;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};