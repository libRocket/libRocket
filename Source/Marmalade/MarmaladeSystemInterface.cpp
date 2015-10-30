#include "MarmaladeSystemInterface.h"
#include <s3eTimer.h>

// Get the number of seconds elapsed since the start of the application
float MarmaladeSystemInterface::GetElapsedTime()
{
	return s3eTimerGetMs() / 1000.f;
}
