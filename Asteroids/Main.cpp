#include "Defines.h"
#include "Application.h"

#include "MenuItems.h"


int main(void)
{
	Application app(SCRN_WIDTH, SCRN_HEIGHT, "Asteroids!");

	app.run();

	return 0;
}