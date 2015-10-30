#include <s3e.h>
#include <Rocket/Core.h>
#include "s3eKeyboard.h"
#include "MarmaladeEvent.h"


MarmaladeEvent::MarmaladeEvent(const Rocket::Core::String& value) : _value(value)
{

}

MarmaladeEvent::~MarmaladeEvent()
{


}

void MarmaladeEvent::ProcessEvent(Rocket::Core::Event &event)
{
	Rocket::Core::StringList list;
	Rocket::Core::StringUtilities::ExpandString(list, _value, ';');

	for (size_t i = 0; i < list.size(); ++i) {
		Rocket::Core::StringList command;
		Rocket::Core::StringUtilities::ExpandString(command, list[i], ' ');

		if (command.empty()) return;

		if (command[0] == "showKeys")
		{
			s3eKeyboardSetInt(S3E_KEYBOARD_GET_CHAR, 1);
		} else 
		{
			s3eKeyboardSetInt(S3E_KEYBOARD_GET_CHAR, 0);
		}
	}


}

void MarmaladeEvent::OnDetach(Rocket::Core::Element* ROCKET_UNUSED(element))
{
	delete this;
}

