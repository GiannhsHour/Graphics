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
		cube = m;
	}
	static void DeleteCube() { delete cube; }
	Mesh * getMesh(){return cube;}

protected:
	static Mesh * cube;
	SceneNode * center;
	SceneNode * wall1;
	SceneNode * wall2;
	SceneNode * wall3;

};
