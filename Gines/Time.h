#ifndef TIME_H
#define TIME_H

#include "Error.hpp"
#include <SDL\SDL_stdinc.h>



namespace gines
{
	//Variables that should be visible outside
	extern float fps;
	extern float maxFPS;
	extern Uint32 deltaTime;
	extern bool showFps;

	bool initializeTime();
	void uninitializeTime();
	void beginFPS();
	void endFPS();
	void drawFPS();

}

#endif