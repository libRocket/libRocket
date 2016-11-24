/*
 * Copyright (c) 2006 - 2008
 * Wandering Monster Studios Limited
 *
 * Any use of this program is governed by the terms of Wandering Monster
 * Studios Limited's Licence Agreement included with this program, a copy
 * of which can be obtained by contacting Wandering Monster Studios
 * Limited at info@wanderingmonster.co.nz.
 *
 */

#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include <Input.h>
#include <Shell.h>
#include "Inventory.h"

Rocket::Core::Context* context = NULL;

ShellRenderInterfaceExtensions *shell_renderer;

void GameLoop()
{
	context->Update();

	shell_renderer->PrepareRenderBuffer();
	context->Render();
	shell_renderer->PresentRenderBuffer();
}

#if defined ROCKET_PLATFORM_WIN32
#include <windows.h>
int APIENTRY WinMain(HINSTANCE ROCKET_UNUSED_PARAMETER(instance_handle), HINSTANCE ROCKET_UNUSED_PARAMETER(previous_instance_handle), char* ROCKET_UNUSED_PARAMETER(command_line), int ROCKET_UNUSED_PARAMETER(command_show))
#else
int main(int ROCKET_UNUSED_PARAMETER(argc), char** ROCKET_UNUSED_PARAMETER(argv))
#endif
{
#ifdef ROCKET_PLATFORM_WIN32
	ROCKET_UNUSED(instance_handle);
	ROCKET_UNUSED(previous_instance_handle);
	ROCKET_UNUSED(command_line);
	ROCKET_UNUSED(command_show);
#else
	ROCKET_UNUSED(argc);
	ROCKET_UNUSED(argv);
#endif

#ifdef ROCKET_PLATFORM_LINUX
#define APP_PATH "../Samples/tutorial/tutorial_drag/"
#else
#define APP_PATH "../../Samples/tutorial/tutorial_drag/"
#endif

#ifdef ROCKET_PLATFORM_WIN32
        AllocConsole();
#endif

	int window_width = 1024;
	int window_height = 768;

	ShellRenderInterfaceOpenGL opengl_renderer;
	shell_renderer = &opengl_renderer;

	// Generic OS initialisation, creates a window and attaches OpenGL.
	if (!Shell::Initialise(APP_PATH) ||
		!Shell::OpenWindow("Drag Tutorial", shell_renderer, window_width, window_height, true))
	{
		Shell::Shutdown();
		return -1;
	}

	// Rocket initialisation.
	Rocket::Core::SetRenderInterface(&opengl_renderer);
	opengl_renderer.SetViewport(window_width, window_height);

	ShellSystemInterface system_interface;
	Rocket::Core::SetSystemInterface(&system_interface);

	Rocket::Core::Initialise();

	// Create the main Rocket context and set it on the shell's input layer.
	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(window_width, window_height));
	if (context == NULL)
	{
		Rocket::Core::Shutdown();
		Shell::Shutdown();
		return -1;
	}

	Rocket::Debugger::Initialise(context);
	Input::SetContext(context);
	shell_renderer->SetContext(context);

	Shell::LoadFonts("../../assets/");

	// Load and show the inventory document.
	Inventory* inventory_1 = new Inventory("Inventory 1", Rocket::Core::Vector2f(50, 200), context);
	Inventory* inventory_2 = new Inventory("Inventory 2", Rocket::Core::Vector2f(540, 240), context);

	// Add items into the inventory.
	inventory_1->AddItem("Mk III L.A.S.E.R.");
	inventory_1->AddItem("Gravity Descender");
	inventory_1->AddItem("Closed-Loop Ion Beam");
	inventory_1->AddItem("5kT Mega-Bomb");

	Shell::EventLoop(GameLoop);

	delete inventory_1;
	delete inventory_2;

	// Shutdown Rocket.
	context->RemoveReference();
	Rocket::Core::Shutdown();

	Shell::CloseWindow();
	Shell::Shutdown();

	return 0;
}
