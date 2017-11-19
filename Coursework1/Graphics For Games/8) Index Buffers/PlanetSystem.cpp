#include "PlanetSystem.h"

Mesh * PlanetSystem::sphere = NULL;
Mesh * PlanetSystem::spaceship = NULL;

PlanetSystem::PlanetSystem(void) {
	
	sun = new SceneNode(sphere, Vector4(1, 0, 0, 1));
	sun->SetModelScale(Vector3(1800, 1700, 1800));
	sun->SetTransform(Matrix4::Translation(Vector3(200, 0, 0)));
	sun->SetBoundingRadius(3000.0f);
	sun->setType(0);
	AddChild(sun);

	planet1 = new SceneNode(sphere, Vector4(1, 0, 0, 1));
	planet1->SetModelScale(Vector3(1000, 950, 1000));
	planet1->SetTransform(Matrix4::Translation(Vector3(7000, 0, 0)));
	planet1->SetBoundingRadius(2200);
	planet1->setType(1);
	sun->AddChild(planet1);

	planet2 = new SceneNode(sphere, Vector4(1, 0, 0, 1));
	planet2->SetModelScale(Vector3(300, 270, 300));
	planet2->SetTransform(Matrix4::Translation(Vector3(1500, 0, 0)));
	planet2->SetBoundingRadius(500);
	planet2->setType(3);
	planet1->AddChild(planet2);

	planet3 = new SceneNode(sphere, Vector4(1, 0, 0, 1));
	planet3->SetModelScale(Vector3(800, 750, 800));
	planet3->SetTransform(Matrix4::Rotation(180, Vector3(1, 0, 0))*Matrix4::Translation(Vector3(-6000, 0, -4000)));
	planet3->SetBoundingRadius(2000);
	planet3->setType(2);
	sun->AddChild(planet3);

	planet4 = new SceneNode(sphere, Vector4(1, 0, 0, 1));
	planet4->SetModelScale(Vector3(200, 180, 200));
	planet4->SetTransform(Matrix4::Translation(Vector3(1200, 0, 2000)));
	planet4->SetBoundingRadius(300);
	planet4->setType(3);
	planet3->AddChild(planet4);

	planet5 = new SceneNode(sphere, Vector4(1, 0, 0, 1));
	planet5->SetModelScale(Vector3(150, 140, 150));
	planet5->SetTransform(Matrix4::Translation(Vector3(-1000, 0, -1000)));
	planet5->SetBoundingRadius(200);
	planet5->setType(3);
	planet3->AddChild(planet5);


	

}

void PlanetSystem::Update(float msec) {
	//transform = transform * Matrix4::Rotation(-msec / 500.0f, Vector3(0, 1, 0));
	sun -> SetTransform(sun -> GetTransform() *Matrix4::Rotation(-msec / 400.0f, Vector3(0, 1, 0)));

	/*planet1->SetTransform(Matrix4::Translation(sun->GetWorldTransform().GetPositionVector())
		*Matrix4::Rotation(1.0f, Vector3(0, 1, 0))
		*Matrix4::Translation(-(sun->GetWorldTransform().GetPositionVector()))
		*planet1->GetTransform());*/

	planet1 -> SetTransform(planet1 -> GetTransform() *Matrix4::Rotation(-msec / 300.0f, Vector3(1, 1, 0)));
	planet2 ->  SetTransform(planet2-> GetTransform() *Matrix4::Rotation(-msec / 250.0f, Vector3(0, 1, 0)));
	planet3 -> SetTransform(planet3->GetTransform() *Matrix4::Rotation(-msec / 200.0f, Vector3(0, 1, 0)));
	planet4 ->SetTransform(planet4->GetTransform() *Matrix4::Rotation(-msec / 180.0f, Vector3(0, 1, 0)));
	planet5->SetTransform(planet5->GetTransform() *Matrix4::Rotation(-msec / 170.0f, Vector3(0, 1, 0)));
	SceneNode::Update(msec);
}