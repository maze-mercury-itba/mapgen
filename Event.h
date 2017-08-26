#pragma once
#include <cstdint>

typedef enum events { NO_EVENT, EV_SELECT, EV_ROTATE, EV_DISCARD, EV_EXIT,
					EV_LEFT, EV_RIGHT, EV_UP, EV_DOWN } event_t; //RESPETAR EL ORDEN DE LAS DIRECCIONES!!!
	


class Event {
public:
	Event(event_t name, uint16_t x = 0, uint16_t y = 0) : name(name),x(x), y(y) { ; };
	const event_t name;
	const uint16_t x, y;
};