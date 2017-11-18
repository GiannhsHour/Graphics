#pragma once
#include "..\..\nclgl\scenenode.h"
#include "..\..\nclgl\OBJMesh.h"

class Planet1Scene : public SceneNode {
public:
	Planet1Scene(void);
	~Planet1Scene(void) {};
	virtual void Update(float msec);

	static void CreatePlanet1Scene() {
		OBJMesh * m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"cube2.obj");
		wall = m;
		OBJMesh * p = new OBJMesh();
		p->LoadOBJMesh(MESHDIR"planet1/plant.obj");
		plant = p;

	}
	static void DeletePlanetSystem() { delete wall; }
	Mesh * getWallMesh() { return wall; }
	Mesh * getPlantMesh() { return plant; }

protected:
	static Mesh * wall;
	static Mesh * plant;
	SceneNode * wallLeft;
	SceneNode * wallBack;
	SceneNode * wallFloor;
	SceneNode * wallRight;
	SceneNode * wallCeil;
	SceneNode * wallFront1;
	SceneNode * wallFront2;
	SceneNode * wallFront3;
	SceneNode*** plants;


};
