#pragma once

#include <cstdint>
#include <allegro5\allegro.h>
#include <allegro5\events.h>
#include "vector.h"
#include "Graphic.h"
#include "Event.h"

class EventGenerator
{
public:
	EventGenerator(ALLEGRO_DISPLAY * display = NULL);
	~EventGenerator();
	Event * getNextEvent();
	//uivector_t getLast();

private:
	ALLEGRO_EVENT_QUEUE * evQ;
	//uivector_t last;
	//std::vector<Button>& b;
};