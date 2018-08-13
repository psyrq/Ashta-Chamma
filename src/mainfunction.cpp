#include "header.h"

#include <ctime>

#include "templates.h"

// Needs one of the following #includes, to include the class definition
#include "MyEngine.h"

#include "PlayRules.h"

#define BASE_SCREEN_WIDTH 800
#define BASE_SCREEN_HEIGHT 600

int main(int argc, char *argv[])
{
	int iResult;

	// Send random number generator with current time
	MyEngine oMain;

	char buf[1024];
	sprintf( buf, "My Demonstration Program : Size %d x %d", BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT );
	iResult = oMain.Initialise( buf, BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, "consola.ttf", 24 );
	iResult = oMain.MainLoop();
	oMain.Deinitialise();

	return iResult;
}
