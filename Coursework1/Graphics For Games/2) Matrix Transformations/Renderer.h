#pragma once

#include "../../nclgl/OGLRenderer.h"

class Renderer : public OGLRenderer {
public:
 Renderer(Window & parent);
 virtual ~Renderer(void);

 virtual void RenderScene();
 virtual void UpdateScene(float msec);
 void SwitchToPerspective();
 void SwitchToOrthographic();

 inline void SetScale(float s) { scale = s; }
 inline void SetRotation(float r) { rotation = r; }
 inline void SetPosition(Vector3 p) { position = p; }
 void SetRotationAxis(char r);

 float x = 0.0f, y = 1.0f, z = 0.0f;
 protected:
 Mesh * triangle;

 float scale;
 float rotation;
 Vector3 position;

 Camera *camera;

};