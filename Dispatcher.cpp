#include "Dispatcher.h"
#include "World.h"

Dispatcher::Dispatcher(Graphic& g) : g(g)
{
	m = new NewMap;
	g.setMap(m);
	g.redrawGrid();
	g.updateActive();

	dpoint_t t = { 4,5 };
	m->setTarget(t);
}

Dispatcher::~Dispatcher()
{
	delete m;
}

void Dispatcher::dispatch(Event * ev)
{
	switch (ev->name) {
	case EV_DISCARD: {
		delete m;
		m = new NewMap;
		g.setMap(m);
		g.redrawGrid();
		g.updateActive();
	} break;

	case EV_SELECT: {
		m->toggleActiveWall();
		g.updateGrid();
	} break;

	case EV_ROTATE: {
		m->rotateActiveWall();
		g.updateActive();
	} break;

	case EV_EXIT: {
		m->makeMapFile("mapa");
	} break;

	default: {
		if (ev->name >= EV_LEFT && ev->name <= EV_DOWN) {
			m->moveActiveWall(direction_t(ev->name - EV_LEFT));
			g.updateActive();
		}
	} break;

	}
}
