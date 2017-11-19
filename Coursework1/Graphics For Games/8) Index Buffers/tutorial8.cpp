#pragma comment(lib, "nclgl.lib")

#include "../../NCLGL/window.h"
#include "Renderer.h"

int main() {
	Light * light;
	Window w("Per Pixel Lighting!", 800, 600, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);//This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if (!renderer.HasInitialised()) {
		return -1;
	}
	
	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);
	float rotation = 2.0f;
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		light = renderer.getLight(0);
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
			renderer.setRoot(1);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_2)) {
			renderer.setRoot(2);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_3)) {
			renderer.setRoot(3);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
			light->SetAmbient(light->GetAmbient() + 0.005f);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_O))
			light->SetAmbient(light->GetAmbient() - 0.005f);
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_L)) {
			light->SetRadius(light->GetRadius() + 25.0f);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
			light->SetRadius(light->GetRadius() - 25.0f);
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_C)) {
			/*Vector4 colour = light->GetColour();
			light->SetColour(Vector4(colour.x*1.1f, colour.y*1.0f, colour.z*1.0f, colour.w*1.0f));*/
			renderer.jump();
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_R)) {
			//cout << renderer.getLight()->GetPosition().x << " " << renderer.getLight()->GetPosition().y << " " << renderer.getLight()->GetPosition().z << endl;
			cout << renderer.getCamera()->GetPosition().x << " " << renderer.getCamera()->GetPosition().y << " " << renderer.getCamera()->GetPosition().z << endl;
			renderer.getLight(0)->SetPosition(Matrix4::Translation(Vector3(4000.0f, 0.0f, 4200.0f)) * Matrix4::Rotation(rotation, Vector3(0.0f, 0.0f, 1.0f))  * Matrix4::Translation(-Vector3(4000.0f, 0.0f, 4200.0f))  * renderer.getLight(0)->GetPosition());
		//	renderer.getCamera()->SetPosition(Matrix4::Translation(Vector3(800.0f, 50.0f, 800.0f)) * Matrix4::Rotation(rotation, Vector3(0.0f, 1.0f, 0.0f))  * Matrix4::Translation(-Vector3(800.0f, 50.0f, 800.0f))  * renderer.getCamera()->GetPosition()) ;
			//renderer.getCamera()->SetYaw(renderer.getCamera()->GetYaw() + 2.0f);
		}
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	return 0;
}