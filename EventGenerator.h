#pragma once

#include <cstdint>
#include <allegro5\allegro.h>
#include <allegro5\events.h>
#include "vector.h"
#include "Graphic.h"

enum events { NO_EVENT, NEW_WALL, DELETE_WALL, EXIT };


class EventGenerator
{
public:
	EventGenerator(Graphic * g);
	~EventGenerator();
	uint16_t getNextEvent();
	uivector_t getLast();

private:
	ALLEGRO_EVENT_QUEUE * evQ;
	Graphic * g;
	uivector_t last;
};