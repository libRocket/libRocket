#ifndef ROCKETMARMALADEEVENT_H
#define ROCKETMARMALADEEVENT_H

#include "Rocket/Core/EventListener.h"

class MarmaladeEvent : public Rocket::Core::EventListener
{
public:
	MarmaladeEvent(const Rocket::Core::String& value);
    virtual ~MarmaladeEvent() override;

	virtual void ProcessEvent(Rocket::Core::Event& event) override;
    /// Destroys the event.
    virtual void OnDetach(Rocket::Core::Element* element);
private:
	Rocket::Core::String _value;

};



#endif