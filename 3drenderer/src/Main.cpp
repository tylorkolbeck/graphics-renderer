#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <iostream>
#include "renderer.h"
#include "vector.h"
#include "color.h"
#include "matrix.h"
#include <filesystem>
#include "light.h"
#include "Mesh.h"
#include "ImGuiManager.h"
#include "w_HelloWindow.h"
#include "w_FPSCounter.h"
#include "w_Transform.h"
#include "Window.h"
#include "GameEngine.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"


GameEngine *gameEngine;

int main(int argc, char *args[])
{
	gameEngine = new GameEngine("3D Engine", 1080, 720, true);
	gameEngine->Init();

	gameEngine->startGameLoop();

	delete gameEngine;

	return 0;
}