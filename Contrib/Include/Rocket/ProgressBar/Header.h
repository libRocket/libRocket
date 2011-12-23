#ifndef ROCKETPROGRESSBARHEADER_H
#define ROCKETPROGRESSBARHEADER_H

#include <Rocket/Core/Platform.h>

#if !defined STATIC_LIB
	#ifdef ROCKET_PLATFORM_WIN32
		#ifdef RocketProgressBar_EXPORTS
			#define ROCKETPROGRESSBAR_API __declspec(dllexport)
		#else
			#define ROCKETPROGRESSBAR_API __declspec(dllimport)
		#endif
	#else
		#define ROCKETPROGRESSBAR_API __attribute__((visibility("default")))
	#endif
#else
	#define ROCKETPROGRESSBAR_API
#endif

#endif
