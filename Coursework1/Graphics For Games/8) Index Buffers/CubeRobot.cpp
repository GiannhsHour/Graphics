#include "CubeRobot.h"

Mesh * CubeRobot::cube = NULL;

CubeRobot::CubeRobot(void) {
	// Optional , uncomment if you want a local origin marker !
	// SetMesh ( cube );
	
	center = new SceneNode(cube, Vector4(1, 0, 0, 1)); // Red !
	center->SetModelScale(Vector3(200, 200, 200));
	center->SetTransform(Matrix4::Translation(Vector3(4000, 600, 4100)));
	center->SetBoundingRadius(1000.0f);
	AddChild(center);

	//wall1 = new SceneNode(cube, Vector4(1, 0, 0, 1)); // Red !
	//wall1->SetModelScale(Vector3(20, 800, 2840));
	//wall1->SetTransform(Matrix4::Translation(Vector3(-2000, 0, 2000))*Matrix4::Rotation(135.5f, Vector3(0, 1, 0)));
	//wall1->SetBoundingRadius(2000.0f);
	//center->AddChild(wall1);

	//wall2 = new SceneNode(cube, Vector4(1, 0, 0, 1)); // Red !
	//wall2->SetModelScale(Vector3(20, 800, 2100));
	//wall2->SetTransform(Matrix4::Translation(Vector3(2100, 0, 0))*Matrix4::Rotation(90.0f, Vector3(0,1,0)));
	//wall2->SetBoundingRadius(2000.0f);
	//center->AddChild(wall2);

	//wall3 = new SceneNode(cube, Vector4(1, 0, 0, 1)); // Red !
	//wall3->SetModelScale(Vector3(20, 800, 2820));
	//wall3->SetTransform(Matrix4::Translation(Vector3(-1900, 0, -2060))*Matrix4::Rotation(223.0f, Vector3(0, 1, 0)));
	//wall3->SetBoundingRadius(2000.0f);
	//center->AddChild(wall3);

}

void CubeRobot::Update(float msec) {
	/*transform = transform * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));
	head -> SetTransform(head -> GetTransform() *Matrix4::Rotation(-msec / 10.0f, Vector3(0, 1, 0)));
	leftArm -> SetTransform(leftArm -> GetTransform() *Matrix4::Rotation(-msec / 10.0f, Vector3(1, 0, 0)));
	rightArm -> SetTransform(rightArm -> GetTransform() *Matrix4::Rotation(msec / 10.0f, Vector3(1, 0, 0)));*/
	SceneNode::Update(msec);
}