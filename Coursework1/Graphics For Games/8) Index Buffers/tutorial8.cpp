#pragma comment(lib, "nclgl.lib")

#include "../../NCLGL/window.h"
#include "Renderer.h"
#include <time.h>

int main() {
	Light * light;
	srand(time(NULL));
	Window w("Per Pixel Lighting!", 800, 600, true);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);//This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if (!renderer.HasInitialised()) {
		return -1;
	}
	
	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);
	float rotation = 1.0f;
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		light = renderer.getLight(0);
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
			renderer.setRoot(1);
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
			renderer.setRoot(2);
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_3)) {
			renderer.setRoot(3);
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_4)) {
			renderer.setRoot(4);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
			light->SetAmbient(light->GetAmbient() + 0.001f);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_O)) {
			light->SetAmbient(light->GetAmbient() - 0.001f);
		}
	
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_L)) {
			light->SetRadius(light->GetRadius() + 30.0f);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
			light->SetRadius(light->GetRadius() - 30.0f);

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_E)) {
			if (renderer.getCanEnterPlanet()) {
				renderer.setPlanetEnter(true);
			}
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Q)) {
			renderer.setGoToSpace(true);
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F)) {
			renderer.toggleFog();
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_G)) {
			renderer.toggleRain();
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_R)) {
			renderer.rotateSun();
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_PAUSE)) {
			renderer.toggleSceneChange();
		}

		float sinceLastTime = w.GetTimer()->GetTimedMS();
		renderer.setSinceLastTime(sinceLastTime);
		renderer.UpdateScene(sinceLastTime);
		renderer.RenderScene();
	}

	return 0;
}