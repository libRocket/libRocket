#include <s3e.h>
#include "s3eKeyboard.h"
#include "s3eOSReadString.h"
#include <IwDebug.h>
#include <IwGx.h>
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include "MarmaladeRenderInterfaceOpenGL.h"
#include "MarmaladeSystemInterface.h"
#include "MarmaladeInput.h"
#include "MarmaladeEventListenerInstancer.h"

MarmaladeRenderInterfaceOpenGL *opengl_renderer;
MarmaladeSystemInterface *system_interface;
MarmaladeEventListenerInstancer *event_instancer;

Rocket::Core::Context* context = NULL;


static const int KEYMAP_SIZE = 256;
bool keysAvailable;
static Rocket::Core::Input::KeyIdentifier key_identifier_map[KEYMAP_SIZE];
#define NUM_CHARS 5
static s3eWChar g_Chars[NUM_CHARS];
static int g_NumChars = 0;

void initKeymap()
{
	memset(key_identifier_map, 0, sizeof(key_identifier_map));
	
	key_identifier_map[s3eKeyA] = Rocket::Core::Input::KI_A;
	key_identifier_map[s3eKeyB] = Rocket::Core::Input::KI_B;
	key_identifier_map[s3eKeyC] = Rocket::Core::Input::KI_C;
	key_identifier_map[s3eKeyD] = Rocket::Core::Input::KI_D;
	key_identifier_map[s3eKeyE] = Rocket::Core::Input::KI_E;
	key_identifier_map[s3eKeyF] = Rocket::Core::Input::KI_F;
	key_identifier_map[s3eKeyG] = Rocket::Core::Input::KI_G;
	key_identifier_map[s3eKeyH] = Rocket::Core::Input::KI_H;
	key_identifier_map[s3eKeyI] = Rocket::Core::Input::KI_I;
	key_identifier_map[s3eKeyJ] = Rocket::Core::Input::KI_J;
	key_identifier_map[s3eKeyK] = Rocket::Core::Input::KI_K;
	key_identifier_map[s3eKeyL] = Rocket::Core::Input::KI_L;
	key_identifier_map[s3eKeyM] = Rocket::Core::Input::KI_M;
	key_identifier_map[s3eKeyN] = Rocket::Core::Input::KI_N;
	key_identifier_map[s3eKeyO] = Rocket::Core::Input::KI_O;
	key_identifier_map[s3eKeyP] = Rocket::Core::Input::KI_P;
	key_identifier_map[s3eKeyQ] = Rocket::Core::Input::KI_Q;
	key_identifier_map[s3eKeyR] = Rocket::Core::Input::KI_R;
	key_identifier_map[s3eKeyS] = Rocket::Core::Input::KI_S;
	key_identifier_map[s3eKeyT] = Rocket::Core::Input::KI_T;
	key_identifier_map[s3eKeyU] = Rocket::Core::Input::KI_U;	
	key_identifier_map[s3eKeyV] = Rocket::Core::Input::KI_V;
	key_identifier_map[s3eKeyW] = Rocket::Core::Input::KI_W;
	key_identifier_map[s3eKeyX] = Rocket::Core::Input::KI_X;
	key_identifier_map[s3eKeyY] = Rocket::Core::Input::KI_Y;
	key_identifier_map[s3eKeyZ] = Rocket::Core::Input::KI_Z;

	key_identifier_map[s3eKey0] = Rocket::Core::Input::KI_0;
	key_identifier_map[s3eKey1] = Rocket::Core::Input::KI_1;
	key_identifier_map[s3eKey2] = Rocket::Core::Input::KI_2;
	key_identifier_map[s3eKey3] = Rocket::Core::Input::KI_3;
	key_identifier_map[s3eKey4] = Rocket::Core::Input::KI_4;
	key_identifier_map[s3eKey5] = Rocket::Core::Input::KI_5;
	key_identifier_map[s3eKey6] = Rocket::Core::Input::KI_6;
	key_identifier_map[s3eKey7] = Rocket::Core::Input::KI_7;
	key_identifier_map[s3eKey8] = Rocket::Core::Input::KI_8;
	key_identifier_map[s3eKey9] = Rocket::Core::Input::KI_9;


	key_identifier_map[s3eKeyNumPad0] = Rocket::Core::Input::KI_0;
	key_identifier_map[s3eKeyNumPad1] = Rocket::Core::Input::KI_1;
	key_identifier_map[s3eKeyNumPad2] = Rocket::Core::Input::KI_2;
	key_identifier_map[s3eKeyNumPad3] = Rocket::Core::Input::KI_3;
	key_identifier_map[s3eKeyNumPad4] = Rocket::Core::Input::KI_4;
	key_identifier_map[s3eKeyNumPad5] = Rocket::Core::Input::KI_5;
	key_identifier_map[s3eKeyNumPad6] = Rocket::Core::Input::KI_6;
	key_identifier_map[s3eKeyNumPad7] = Rocket::Core::Input::KI_7;
	key_identifier_map[s3eKeyNumPad8] = Rocket::Core::Input::KI_8;
	key_identifier_map[s3eKeyNumPad9] = Rocket::Core::Input::KI_9;



	key_identifier_map[s3eKeyF1] = Rocket::Core::Input::KI_F1;
	key_identifier_map[s3eKeyF2] = Rocket::Core::Input::KI_F2;
	key_identifier_map[s3eKeyF3] = Rocket::Core::Input::KI_F3;
	key_identifier_map[s3eKeyF4] = Rocket::Core::Input::KI_F4;
	key_identifier_map[s3eKeyF5] = Rocket::Core::Input::KI_F5;
	key_identifier_map[s3eKeyF6] = Rocket::Core::Input::KI_F6;
	key_identifier_map[s3eKeyF7] = Rocket::Core::Input::KI_F7;
	key_identifier_map[s3eKeyF8] = Rocket::Core::Input::KI_F8;
	key_identifier_map[s3eKeyF9] = Rocket::Core::Input::KI_F9;
	key_identifier_map[s3eKeyF10] = Rocket::Core::Input::KI_F10;
	key_identifier_map[s3eKeyF11] = Rocket::Core::Input::KI_F11;
	key_identifier_map[s3eKeyF12] = Rocket::Core::Input::KI_F12;


	key_identifier_map[s3eKeyLeft] = Rocket::Core::Input::KI_LEFT;
	key_identifier_map[s3eKeyRight] = Rocket::Core::Input::KI_RIGHT;
	key_identifier_map[s3eKeyUp] = Rocket::Core::Input::KI_UP;
	key_identifier_map[s3eKeyDown] = Rocket::Core::Input::KI_DOWN;

	key_identifier_map[s3eKeyBackspace] = Rocket::Core::Input::KI_BACK;
	key_identifier_map[s3eKeyTab] = Rocket::Core::Input::KI_TAB;
	key_identifier_map[s3eKeyEsc] = Rocket::Core::Input::KI_ESCAPE;
	key_identifier_map[s3eKeySpace] = Rocket::Core::Input::KI_SPACE;
	key_identifier_map[s3eKeyEnd] = Rocket::Core::Input::KI_END;
	key_identifier_map[s3eKeyHome] = Rocket::Core::Input::KI_HOME;
	key_identifier_map[s3eKeyInsert] = Rocket::Core::Input::KI_INSERT;
	key_identifier_map[s3eKeyDelete] = Rocket::Core::Input::KI_DELETE;

	key_identifier_map[s3eKeyPageUp] = Rocket::Core::Input::KI_PRIOR;
	key_identifier_map[s3eKeyPageDown] = Rocket::Core::Input::KI_NEXT;

	key_identifier_map[s3eKeyLeftShift] = Rocket::Core::Input::KI_LSHIFT;
	key_identifier_map[s3eKeyRightShift] = Rocket::Core::Input::KI_RSHIFT;
	key_identifier_map[s3eKeyLeftControl] = Rocket::Core::Input::KI_LCONTROL;
	key_identifier_map[s3eKeyRightControl] = Rocket::Core::Input::KI_RCONTROL;



	key_identifier_map[s3eKeyNumLock] = Rocket::Core::Input::KI_NUMLOCK;
	//key_identifier_map[s3eKeySc] = Rocket::Core::Input::KI_SCROLL;






}
 



void multiTouchButtonCB(s3ePointerTouchEvent* event) {
  if (event->m_Pressed)
  {
    context->ProcessMouseMove(event->m_x, event->m_y, 0);
    context->ProcessMouseButtonDown(0, 0);
  }
  else
  {
    context->ProcessMouseMove(event->m_x, event->m_y, 0);
    context->ProcessMouseButtonUp(0, 0);
  }
}

void multiTouchMotionCB(s3ePointerTouchMotionEvent* event) {
  context->ProcessMouseMove(event->m_x, event->m_y, 0);
}

void singleTouchButtonCB(s3ePointerEvent* event) {
  if (event->m_Pressed)
  {
    context->ProcessMouseMove(event->m_x, event->m_y, 0);
    context->ProcessMouseButtonDown(event->m_Button, 0);
  }
  else
  {
    context->ProcessMouseMove(event->m_x, event->m_y, 0);
    context->ProcessMouseButtonUp(event->m_Button, 0);
  }
}

void singleTouchMotionCB(s3ePointerMotionEvent* event) {
  context->ProcessMouseMove(event->m_x, event->m_y, 0);
}


static int32 keyboardHandler(void* sys, void*)
{
    s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;
	Rocket::Core::Input::KeyIdentifier key_identifier;

	if (event->m_Pressed) 
    {
	//	key_identifier = key_identifier_map[event->m_Key];
	//	context->ProcessKeyDown(key_identifier, 32);
    }
    else
    {
		key_identifier = key_identifier_map[event->m_Key];
		if (key_identifier == Rocket::Core::Input::KI_UNKNOWN) 
			return 0;

		int key_modifier = 32;

		context->ProcessKeyDown(key_identifier, key_modifier);


    }
    return 0;
}


void init() {
	IwGxInit();
	
	opengl_renderer = new MarmaladeRenderInterfaceOpenGL;
    system_interface = new MarmaladeSystemInterface;
	event_instancer = new MarmaladeEventListenerInstancer();



	// Rocket initialisation.
	Rocket::Core::SetRenderInterface(opengl_renderer);
	Rocket::Core::SetSystemInterface(system_interface);

	Rocket::Core::Initialise();
    Rocket::Controls::Initialise();

    //FIXME: Crashes when using events
    Rocket::Core::Factory::RegisterEventListenerInstancer(event_instancer);




	// Create the main Rocket context and set it on the shell's input layer.
	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(IwGxGetScreenWidth(), IwGxGetScreenHeight()));
	if (context == NULL)
	{
		Rocket::Core::Shutdown();
		return;
	}

	Rocket::Debugger::Initialise(context);

	MarmaladeInput::SetContext(context);

	Rocket::Core::String font_names[4];
	font_names[0] = "Delicious-Roman.otf";
	font_names[1] = "Delicious-Italic.otf";
	font_names[2] = "Delicious-Bold.otf";
	font_names[3] = "Delicious-BoldItalic.otf";
    //font_names[4] = "Arial.fnt";

	for (uint i = 0; i < sizeof(font_names) / sizeof(Rocket::Core::String); i++)
	{
		Rocket::Core::FontDatabase::LoadFontFace(font_names[i]);
	}

	// Load and show the demo document.
	Rocket::Core::ElementDocument* document = context->LoadDocument("demo.rml");

	if (document != NULL)
	{
		document->Show();
		document->RemoveReference();
	}

  if (s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE)) {
    s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)multiTouchButtonCB, NULL);
    s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)multiTouchMotionCB, NULL);
  } else {
    s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)singleTouchButtonCB, NULL);
    s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)singleTouchMotionCB, NULL);
  }

//  if ((keysAvailable = s3eKeyboardGetInt(S3E_KEYBOARD_HAS_KEYPAD) || s3eKeyboardGetInt(S3E_KEYBOARD_HAS_ALPHA)))
 // {
 // }
   initKeymap();

   s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, keyboardHandler, NULL);

   

}

void release() {
	// Shutdown Rocket.
	context->RemoveReference();
	event_instancer->RemoveReference();
	Rocket::Core::Shutdown();

    delete opengl_renderer;
	delete system_interface;

	IwGxTerminate();
}

bool update() {
	s3eWChar ch;
	
	int charState = s3eKeyboardGetInt(S3E_KEYBOARD_GET_CHAR);

	context->Update();

	s3eKeyboardUpdate();

	if (charState)
	{
        while ((ch = s3eKeyboardGetChar()) != S3E_WEOF)
        {
            if (g_NumChars < NUM_CHARS)
                g_NumChars++;
            memmove(g_Chars+1, g_Chars, (NUM_CHARS - 1) * sizeof(s3eWChar));
            g_Chars[0] = ch;


			// TODO: prepsat keyboardHandler 
			if (ch == 0x0008) { // Backspace
				context->ProcessKeyDown(Rocket::Core::Input::KI_BACK, 32);
			}

			if (ch != 0xffff && ch >= 32) 
			{
				context->ProcessTextInput((Rocket::Core::word) ch);
			}
        }

	}


	return true;
}

void draw() {
	IwGxSetColClear(0, 0, 0, 0);
	// Clear the screen
	IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
	// Draw screen space in the back so 3D sprites are visible
	IwGxSetScreenSpaceSlot(-1);


	////////////
	context->Render();

	//Iw2DFinishDrawing();

	// Flush IwGx
	IwGxFlush();

	// Display the rendered frame
	IwGxSwapBuffers();

	// Show the surface
	//	Iw2DSurfaceShow();
}

S3E_MAIN_DECL void IwMain() {

	init();

	// Main Game Loop
	while (!s3eDeviceCheckQuitRequest())
	{
		// Yield to the operating system
		s3eDeviceYield(0);

		// Prevent back-light to off state
		s3eDeviceBacklightOn();

		// Update the game
		if (!update())
			break;

		// Draw the scene
		draw();
	}

	release();

}