#include "Planet1Scene.h"
#include <time.h> 

Mesh * Planet1Scene::wall = NULL;
Mesh * Planet1Scene::plant = NULL;

Planet1Scene::Planet1Scene(void) {

	srand(time(NULL));

	wallLeft = new SceneNode(wall, Vector4(1, 0, 0, 1));
	wallLeft->SetModelScale(Vector3(500, 300, 35));
	wallLeft->SetTransform(Matrix4::Translation(Vector3(0, 200, 0)));
	wallLeft->SetBoundingRadius(2000.0f);
	wallLeft->setType(0);
	AddChild(wallLeft);

	wallBack = new SceneNode(wall, Vector4(1, 0, 0, 1));
	wallBack->SetModelScale(Vector3(380, 300, 35));
	wallBack->SetTransform(Matrix4::Translation(Vector3(-265, 200, 175))*Matrix4::Rotation(90,Vector3(0, 1, 0)));
	wallBack->SetBoundingRadius(2000.0f);
	wallBack->setType(0);
	AddChild(wallBack);

	wallFloor = new SceneNode(wall, Vector4(1, 0, 0, 1));
	wallFloor->SetModelScale(Vector3(550, 400, 20));
	wallFloor->SetTransform(Matrix4::Translation(Vector3(0, 50, 175))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
	wallFloor->SetBoundingRadius(2000.0f);
	wallFloor->setType(0);
	AddChild(wallFloor);

	wallRight = new SceneNode(wall, Vector4(1, 0, 0, 1));
	wallRight->SetModelScale(Vector3(500, 300, 35));
	wallRight->SetTransform(Matrix4::Translation(Vector3(0, 200, 350)));
	wallRight->SetBoundingRadius(2000.0f);
	wallRight->setType(0);
	AddChild(wallRight);

	wallCeil = new SceneNode(wall, Vector4(1, 0, 0, 1));
	wallCeil->SetModelScale(Vector3(550, 400, 20));
	wallCeil->SetTransform(Matrix4::Translation(Vector3(0, 350, 175))*Matrix4::Rotation(90, Vector3(1, 0, 0)));
	wallCeil->SetBoundingRadius(100.0f);
	wallCeil->setType(0);
	AddChild(wallCeil);
	


}

void Planet1Scene::Update(float msec) {
	

	SceneNode::Update(msec);
}