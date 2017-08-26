#pragma once
#include "EventGenerator.h"
#include "Graphic.h"
#include "NewMap.h"

class Dispatcher {
public:

	Dispatcher(Graphic& g);
	~Dispatcher();
	void dispatch(Event * ev);

private:
	NewMap * m;
	Graphic& g;
};

