#ifndef ROCKETMARMALADEEVENTINSTANCER_H
#define ROCKETMARMALADEEVENTINSTANCER_H

#include "Rocket/Core/EventListenerInstancer.h"


class MarmaladeEventListenerInstancer : public Rocket::Core::EventListenerInstancer
{
public:
	MarmaladeEventListenerInstancer() { }
	~MarmaladeEventListenerInstancer() { }


	virtual Rocket::Core::EventListener* InstanceEventListener(const Rocket::Core::String& value, Rocket::Core::Element* element);

	virtual void Release();

};

#endif