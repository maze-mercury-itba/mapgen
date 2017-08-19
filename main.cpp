#include "EventGenerator.h"
#include "Graphic.h"
#include <allegro5\allegro.h>
#include "NewMap.h"


int main(void)
{
	if (!al_init())
		return EXIT_FAILURE;
	
	NewMap map;
	Graphic g(NULL);
	g.drawBackground(map.getCurrWalls());
	g.showChanges();

	EventGenerator e(&g);
	uint16_t ev = NO_EVENT;


	//Orden de updates: mundo, sensores y actuadores, inteligencia. Es importante.
	do {
		ev = e.getNextEvent();

		if (ev != NO_EVENT) {
			switch (ev) {
			case NEW_WALL:
				map.newWall(e.getLast());

				break;

			case DELETE_WALL:
				map.removeWall(e.getLast().start);
				break;
			}

			g.drawBackground(map.getCurrWalls());
			g.showChanges();
		}

	} while (ev != EXIT);

	map.setTarget({ 400, 300 });
	map.makeMapFile("mapa");

	return EXIT_SUCCESS;
}