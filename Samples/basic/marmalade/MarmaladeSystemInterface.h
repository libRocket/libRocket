#ifndef ROCKETMARMALADESYSTEMINTERFACE_H
#define ROCKETMARMALADESYSTEMINTERFACE_H

#include <Rocket/Core/SystemInterface.h>

/**
	A custom system interface for Rocket. This provides timing information.
 */

class MarmaladeSystemInterface : public Rocket::Core::SystemInterface
{
public:
	/// Get the number of seconds elapsed since the start of the application
	/// @returns Seconds elapsed
	virtual float GetElapsedTime();
};

#endif
