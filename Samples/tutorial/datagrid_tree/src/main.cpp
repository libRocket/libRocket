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
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include <Input.h>
#include <Shell.h>
#include "DecoratorInstancerDefender.h"
#include "HighScores.h"
#include "HighScoresShipFormatter.h"

Rocket::Core::Context* context = NULL;

void GameLoop()
{
	glClear(GL_COLOR_BUFFER_BIT);

	context->Update();
	context->Render();

	Shell::FlipBuffers();
}

#if defined ROCKET_PLATFORM_WIN32
#include <windows.h>
int APIENTRY WinMain(HINSTANCE ROCKET_UNUSED(instance_handle), HINSTANCE ROCKET_UNUSED(previous_instance_handle), char* ROCKET_UNUSED(command_line), int ROCKET_UNUSED(command_show))
#else
int main(int ROCKET_UNUSED(argc), char** ROCKET_UNUSED(argv))
#endif
{
	// Generic OS initialisation, creates a window and attaches OpenGL.
	if (!Shell::Initialise("../Samples/tutorial/datagrid_tree/") ||
		!Shell::OpenWindow("Datagrid Tree Tutorial", true))
	{
		Shell::Shutdown();
		return -1;
	}

	// Rocket initialisation.
	ShellRenderInterfaceOpenGL opengl_renderer;
	Rocket::Core::SetRenderInterface(&opengl_renderer);
    opengl_renderer.SetViewport(1024, 768);

	ShellSystemInterface system_interface;
	Rocket::Core::SetSystemInterface(&system_interface);

	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();

	// Create the main Rocket context and set it on the shell's input layer.
	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(1024, 768));
	if (context == NULL)
	{
		Rocket::Core::Shutdown();
		Shell::Shutdown();
		return -1;
	}

	Rocket::Debugger::Initialise(context);
	Input::SetContext(context);

	Shell::LoadFonts("../../assets/");

	// Load the defender decorator.
	Rocket::Core::DecoratorInstancer* decorator_instancer = Rocket::Core::Factory::RegisterDecoratorInstancer("defender", new DecoratorInstancerDefender());
	if (decorator_instancer != NULL)
		decorator_instancer->RemoveReference();

	// Add the ship formatter.
	HighScoresShipFormatter ship_formatter;

	// Construct the high scores.
	HighScores::Initialise();

	// Load and show the tutorial document.
	Rocket::Core::ElementDocument* document = context->LoadDocument("data/tutorial.rml");
	document->GetElementById("title")->SetInnerRML(document->GetTitle());
	if (document != NULL)
	{
		document->Show();
		document->RemoveReference();
	}

	Shell::EventLoop(GameLoop);

	// Shut down the high scores.
	HighScores::Shutdown();

	// Shutdown Rocket.
	context->RemoveReference();
	Rocket::Core::Shutdown();

	Shell::CloseWindow();
	Shell::Shutdown();

	return 0;
}
