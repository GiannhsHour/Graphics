#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/Frustum.h"
#include "../../nclgl/HeightMap.h"
#include "CubeRobot.h"
#include <algorithm>

class Renderer : public OGLRenderer {
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();
	Light* getLight(int index) { return lights[index]; }
	Camera* getCamera() { return camera; }
protected:
	void DrawNode(SceneNode * n);
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();

	SceneNode * root;
	Camera * camera;
	Mesh* quad;
	HeightMap* heightMap1;
	HeightMap* heightMap2;
	Shader* sceneShader;
	Shader* generalShader;
	Frustum frameFrustum;

	vector<Light *> lights;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};