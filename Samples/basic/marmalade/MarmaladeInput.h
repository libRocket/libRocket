#ifndef ROCKETMARMALADEINPUT_H
#define ROCKETMARMALADEINPUT_H

#include <s3e.h>
#include <Rocket/Core/Input.h>
#include <Rocket/Core/Types.h>

namespace Rocket {
namespace Core {

class Context;

}
}

class MarmaladeInput 
{
public:
	static void SetContext(Rocket::Core::Context* context);

	static Rocket::Core::word GetCharacterCode(Rocket::Core::Input::KeyIdentifier key_identifier, int key_modifier_state);

	static Rocket::Core::Context* context;


};



#endif