#pragma once
#include "..\..\nclgl\scenenode.h"
#include "..\..\nclgl\OBJMesh.h"

class PlanetSystem : public SceneNode {
public:
	PlanetSystem(void);
	~PlanetSystem(void) {};
	virtual void Update(float msec);

	static void CreatePlanetSystem() {
		OBJMesh * m = new OBJMesh();
		m -> LoadOBJMesh(MESHDIR"sphere_earth.obj");
		sphere = m;
		sphere->GenerateNormals();
	}
	static void DeletePlanetSystem() { delete sphere; }
	Mesh * getMesh(){return sphere;}

protected:
	static Mesh * sphere;
	SceneNode * sun;
	SceneNode * planet1;
	SceneNode * planet2;
	SceneNode * planet3;
	SceneNode * planet4;
	SceneNode * planet5;

};
