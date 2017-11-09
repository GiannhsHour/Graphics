#include "CubeRobot.h"

Mesh * CubeRobot::cube = NULL;
Mesh * CubeRobot::sphere = NULL;

CubeRobot::CubeRobot(void) {
	// Optional , uncomment if you want a local origin marker !
	// SetMesh ( cube );

	SceneNode * body = new SceneNode(cube, Vector4(1, 0, 0, 1)); // Red !
	body -> SetModelScale(Vector3(10, 15, 5));
	body -> SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	body->SetBoundingRadius(15.0f);
	AddChild(body);

	head = new SceneNode(cube, Vector4(0, 1, 0, 1)); // Green !
	head -> SetModelScale(Vector3(5, 5, 5));
	head -> SetTransform(Matrix4::Translation(Vector3(0, 30, 0)));
	head->SetBoundingRadius(5.0f);
	body -> AddChild(head);

	leftArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	leftArm -> SetModelScale(Vector3(3, -18, 3));
	leftArm -> SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	leftArm->SetBoundingRadius(18.0f);
	body -> AddChild(leftArm);

	rightArmTop = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	rightArmTop-> SetModelScale(Vector3(2.5, -7, 2.5));
	rightArmTop-> SetTransform(Matrix4::Translation(Vector3(15, 30, -1)));
	rightArmTop->SetBoundingRadius(18.0f);
	body -> AddChild(rightArmTop);

	rightShoulder = new SceneNode(sphere, Vector4(0.5, 0.5, 1, 1));
	rightShoulder->SetModelScale(Vector3(3, 3, 3));
	rightShoulder->SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
	rightShoulder->SetBoundingRadius(18.0f);
	body->AddChild(rightShoulder);

	rightElbow = new SceneNode(sphere, Vector4(0.5, 0.5, 1, 1));
	rightElbow->SetModelScale(Vector3(3, 3, 3));
	rightElbow->SetTransform(Matrix4::Translation(Vector3(15, 13, -1)));
	rightElbow->SetBoundingRadius(18.0f);
	body->AddChild(rightElbow);

	SceneNode * leftLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	leftLeg -> SetModelScale(Vector3(3, -17.5, 3));
	leftLeg -> SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	leftLeg->SetBoundingRadius(18.0f);
	body -> AddChild(leftLeg);

	SceneNode * rightLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	rightLeg -> SetModelScale(Vector3(3, -17.5, 3));
	rightLeg -> SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	rightLeg->SetBoundingRadius(18.0f);
	body -> AddChild(rightLeg);
}

void CubeRobot::Update(float msec) {
	/*transform = transform * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));
	head -> SetTransform(head -> GetTransform() *Matrix4::Rotation(-msec / 10.0f, Vector3(0, 1, 0)));
	leftArm -> SetTransform(leftArm -> GetTransform() *Matrix4::Rotation(-msec / 10.0f, Vector3(1, 0, 0)));
	rightArm -> SetTransform(rightArm -> GetTransform() *Matrix4::Rotation(msec / 10.0f, Vector3(1, 0, 0)));*/
	SceneNode::Update(msec);
}