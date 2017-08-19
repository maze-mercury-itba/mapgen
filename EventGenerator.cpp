#include "EventGenerator.h"
#include <cstdio>


EventGenerator::EventGenerator(Graphic * g)
{
	last = { { UINT16_MAX, UINT16_MAX } , { UINT16_MAX, UINT16_MAX } };
	this->g = NULL;
	evQ = NULL;

	if (al_install_keyboard()) {
		if (al_install_mouse()) {
			if ((evQ = al_create_event_queue()) != NULL) {
				if (g != NULL) {
					al_register_event_source(evQ, al_get_keyboard_event_source());
					al_register_event_source(evQ, al_get_mouse_event_source());
					al_register_event_source(evQ, al_get_display_event_source(g->getDisplay()));
					this->g = g;
				}
				else {
					fprintf(stderr, "graphic era null!\n");
					al_uninstall_keyboard();
					al_uninstall_mouse();
					al_destroy_event_queue(evQ);
					evQ = NULL;
				}
			}
			else {
				fprintf(stderr, "Unable to create event queue\n");
				al_uninstall_keyboard();
				al_uninstall_mouse();
			}
		}
		else {
			fprintf(stderr, "Unable to install mouse\n");
		}
	}
	else {
		fprintf(stderr, "Unable to install keyboard\n");
	}
}


EventGenerator::~EventGenerator()
{
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_destroy_event_queue(evQ);
	evQ = NULL;
}

uint16_t EventGenerator::getNextEvent()
{
	uint16_t evCode = NO_EVENT;
	ALLEGRO_EVENT ev;

	if (al_get_next_event(evQ, &ev)) {  //faltan los eventos de display!!
		if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				evCode = EXIT;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			if (last.start.x == UINT16_MAX) // si no tenia nada seleccionado ya
			{
				last.start.x = ev.mouse.x; 
				last.start.y = ev.mouse.y;
				if (g->isWhite(ev.mouse.x, ev.mouse.y) == false) {
					evCode = DELETE_WALL;	//si aprete en una pared que habia, generar evento delete
				}
			}
			else
			{
				last.end.x = ev.mouse.x;
				last.end.y = ev.mouse.y;
				evCode = NEW_WALL;			
			}
		}
	}

	return evCode;
}

uivector_t EventGenerator::getLast()
{
	uivector_t temp = last;
	last = { {UINT16_MAX, UINT16_MAX}, { UINT16_MAX, UINT16_MAX } };
	return temp;
}


