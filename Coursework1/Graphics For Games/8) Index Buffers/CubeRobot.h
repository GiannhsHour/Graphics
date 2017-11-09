#pragma once
#include "..\..\nclgl\scenenode.h"
#include "..\..\nclgl\OBJMesh.h"

class CubeRobot : public SceneNode {
public:
	CubeRobot(void);
	~CubeRobot(void) {};
	virtual void Update(float msec);

	static void CreateCube() {
		OBJMesh * m = new OBJMesh();
		m -> LoadOBJMesh(MESHDIR"cube.obj");
		OBJMesh * s = new OBJMesh();
		s->LoadOBJMesh(MESHDIR"sphere.obj");
		cube = m;
		sphere = s;
	}
	static void DeleteCube() { delete cube; delete sphere; }

protected:
	static Mesh * cube;
	static Mesh * sphere;
	SceneNode * head;
	SceneNode * leftArm;
	SceneNode * rightArmTop;
	SceneNode * rightArmBot;
	SceneNode * leftShoulder;
	SceneNode * rightShoulder;
	SceneNode * rightElbow;
};
