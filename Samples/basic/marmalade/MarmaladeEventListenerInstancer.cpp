#include "MarmaladeEventListenerInstancer.h"
#include "MarmaladeEvent.h"


Rocket::Core::EventListener* MarmaladeEventListenerInstancer::InstanceEventListener(const Rocket::Core::String& value, Rocket::Core::Element* element)
{
	return new MarmaladeEvent(value);

}

void MarmaladeEventListenerInstancer::Release()
{
	delete this;
}
