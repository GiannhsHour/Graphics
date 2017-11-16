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
		OBJMesh * s = new OBJMesh();
		s->LoadOBJMesh(MESHDIR"skeleton02.obj");
		int x = s->getNumVertices();
		spaceship = s;
	}
	static void DeletePlanetSystem() { delete sphere; }
	Mesh * getMesh(){return sphere;}

protected:
	static Mesh * sphere;
	static Mesh * spaceship;
	SceneNode * sun;
	SceneNode * planet1;
	SceneNode * planet2;
	SceneNode * planet3;
	SceneNode * planet4;
	SceneNode * planet5;

	SceneNode * ship1;

};
