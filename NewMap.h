#pragma once
#include <vector>
#include <list>
#include "vector.h"

#define WALL_THICKNESS	5	

class NewMap
{
public:
	NewMap();
	~NewMap();

	std::vector<uivector_t> * getCurrWalls(); // para mostrar
	
	bool removeWall(uipoint_t p);
	//borrar pared que contenga el punto p; devuelve si la pudo borrar o no

	bool newWall(uivector_t wall);
	//crear una nueva pared entre p1 y p2; devuelve si la pudo crear o no
	
	bool makeMapFile(char * fileName);

	bool setTarget(dpoint_t target);
private:
	std::vector<uivector_t> walls;
	//las paredes que va recibiendo, y las que van quedando duplicadas considerando la thickness
	dpoint_t target;
};