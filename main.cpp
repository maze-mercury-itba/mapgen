#include "EventGenerator.h"
#include "Graphic.h"
#include <allegro5\allegro.h>
#include "NewMap.h"
#include "Dispatcher.h"
#include <Windows.h>

#define ALLEGRO_STATICLINK

int main(void)
{
	ShowWindow(GetConsoleWindow(), SW_HIDE); //oculto la consola.

	if (!al_init())
		return EXIT_FAILURE;
	
	NewMap map;
	Graphic g(NULL);

	EventGenerator e(g.getDisplay());
	Dispatcher d(g);
	Event * ev = new Event(NO_EVENT);


	//Orden de updates: mundo, sensores y actuadores, inteligencia. Es importante.
	do {
		delete ev;
		ev = e.getNextEvent();
		d.dispatch(ev);
	} while (ev->name != EV_EXIT);

//	map.setTarget({ 400, 300 });
	//map.makeMapFile("mapa");

	return EXIT_SUCCESS;
}