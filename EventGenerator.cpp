#include "EventGenerator.h"
#include <cstdio>


EventGenerator::EventGenerator(ALLEGRO_DISPLAY * disp)
{
	//last = { { UINT16_MAX, UINT16_MAX } , { UINT16_MAX, UINT16_MAX } };
	evQ = NULL;

	if (al_install_keyboard()) {
		if (al_install_mouse()) {
			if ((evQ = al_create_event_queue()) != NULL) {
				al_register_event_source(evQ, al_get_keyboard_event_source());
				al_register_event_source(evQ, al_get_mouse_event_source());
				
				if (disp != NULL) {
					al_register_event_source(evQ, al_get_display_event_source(disp));
				}
			}
			else	{
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

Event * EventGenerator::getNextEvent()
{
	Event * e = nullptr;
	ALLEGRO_EVENT ev;

	if (al_get_next_event(evQ, &ev)) {  
		if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:	e = new Event(EV_EXIT);		break;
			case ALLEGRO_KEY_ENTER:		e = new Event(EV_SELECT);	break;
			case ALLEGRO_KEY_X:			e = new Event(EV_DISCARD);	break;
			case ALLEGRO_KEY_SPACE:		e = new Event(EV_ROTATE);	break;
			case ALLEGRO_KEY_RIGHT:		e = new Event(EV_RIGHT);	break;
			case ALLEGRO_KEY_LEFT:		e = new Event(EV_LEFT);		break;
			case ALLEGRO_KEY_UP:		e = new Event(EV_UP);		break;
			case ALLEGRO_KEY_DOWN:		e = new Event(EV_DOWN);		break;
			}
		}
		//else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
		//	for (unsigned int i = 0; i < b.size(); i++) {
		//		if (b[i].wasPressed(ev.mouse.x, ev.mouse.y)) {
		//			switch (b[i].getName()) {
		//				case 
		//			}
		//		}
		//	}
		//	if (last.start.x == UINT16_MAX) // si no tenia nada seleccionado ya
		//	{
		//		last.start.x = ev.mouse.x; 
		//		last.start.y = ev.mouse.y;
		//		if (g->isWhite(ev.mouse.x, ev.mouse.y) == false) {
		//			e = new Event(DELETE_WALL);	//si aprete en una pared que habia, generar evento delete
		//		}
		//	}
		//	else
		//	{
		//		last.end.x = ev.mouse.x;
		//		last.end.y = ev.mouse.y;
		//		e = new Event(NEW_WALL);			
		//	}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			e = new Event(EV_EXIT);
		}
	}

	if (e == nullptr) {
		e = new Event(NO_EVENT);
	}

	return e;
}

//uivector_t EventGenerator::getLast()
//{
//	uivector_t temp = last;
//	last = { {UINT16_MAX, UINT16_MAX}, { UINT16_MAX, UINT16_MAX } };
//	return temp;
//}


