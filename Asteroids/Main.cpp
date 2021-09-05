#include "Defines.h"
#include "Application.h"


int main(void)
{
	srand((uint32_t)time(0));

	Application app(SCRN_WIDTH, SCRN_HEIGHT, "Asteroids!");

	app.run();

	return 0;
}