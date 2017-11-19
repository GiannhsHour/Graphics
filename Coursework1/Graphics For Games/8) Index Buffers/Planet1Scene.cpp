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

	/*plants = new SceneNode**[70];
	for (int i = 0; i < 70; i++) {
		plants[i] = new SceneNode*[90];
	}

	for (int i = 0; i < 70; i++) {
		for (int j = 0; j < 90; j++) {
			plants[i][j] = new SceneNode(plant, Vector4(1, 0, 0, 1));
			plants[i][j]->SetModelScale(Vector3(3, 50, 3));
			plants[i][j]->SetTransform(Matrix4::Translation(Vector3(330 + i * 9 *(rand()%4 + 1), 30, -300 + j * 9  *(rand() % 4 + 1))));
			plants[i][j]->SetBoundingRadius(2000.0f);
			plants[i][j]->setType(0);
			AddChild(plants[i][j]);
		}
	}*/
	


}

void Planet1Scene::Update(float msec) {
	
	/*for (int i = 0; i < 70; i++) {
		for (int j = 0; j < 90; j++) {
			plants[i][j]->SetTransform(plants[i][j]->GetTransform()*Matrix4::Rotation((rand()%3-rand()%3)/80.0f, Vector3(0, 0, 1)));
		}
	}*/
	SceneNode::Update(msec);
}