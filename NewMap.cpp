#include "NewMap.h"
#include <algorithm>
#include <cmath>
#include "mapFile.h"


//static bool onPolygon(uipoint_t p, uipoint_t * pol, uint16_t sides)
//{
//	if (pol == nullptr || sides <= 1)
//		return false;
//
//	for (unsigned int i = 1; i < sides; i++)
//	{
//		int16_t dy = pol[i].y - pol[i-1].y;
//		int16_t dx = pol[i].x - pol[i-1].x;
//
//		if ((-dy)*p.x + dx*p.y + dy*pol[i-1].x - dx*pol[i-1].y >= 0)
//			return false; //si queda a la derecha, esta cuenta da <0
//	}
//	
//	uint16_t dy = pol[0].y - pol[sides-1].y;
//	uint16_t dx = pol[0].x - pol[sides-1].x;
//
//	if ((-dy)*p.x + dx*p.y + dy*pol[sides - 1].x - dx*pol[sides - 1].y >= 0)
//		return false;
//	else
//		return true;//si todo me quedaba a la izquierda true
//}

NewMap::NewMap()
{
	target.x = UINT16_MAX;
}
NewMap::~NewMap()
{
}

std::vector<uivector_t> * NewMap::getCurrWalls()
{
	return &walls;
}


bool NewMap::removeWall(uipoint_t p)
{
	std::vector<uivector_t>::iterator it;
	for (it = walls.begin(); it != walls.end(); ++it)
	{
		if (p.x <= std::max(it->end.x, it->start.x) && p.y <= std::max(it->end.y, it->start.y) &&
			p.x >= std::min(it->end.x, it->start.x) && p.y <= std::max(it->end.y, it->start.y))
		{
			double distance = abs((it->end.y - it->start.y)*p.x - (it->end.x - it->start.x)*p.y + it->end.x * it->start.y - it->end.y * it->start.x);
			distance /= sqrt((it->end.y - it->start.y)*(it->end.y - it->start.y) + (it->end.x - it->start.x)*(it->end.x - it->start.x));

			if (distance <= round(WALL_THICKNESS / 2.0)) {
				walls.erase(it);
				return true;
			}
		}
	}

	return false;
}

bool NewMap::newWall(uivector_t wall)
{
	double lenght = sqrt((wall.end.x - wall.start.x)*(wall.end.x - wall.start.x)
		+ (wall.end.y - wall.start.y)*(wall.end.y - wall.start.y));

	if (lenght <= 3*WALL_THICKNESS)	{
		return false;
	}


	this->walls.push_back(wall);
	return true;
}

bool NewMap::makeMapFile(char * fileName)
{
	if (target.x == UINT16_MAX || fileName == NULL)
		return false;	//puedo hacer un mapa sin paredes, pero no sin target!

	map_t map;
	map.nWalls = (uint16_t)walls.size() * 4; //por cada pared que dibuja el usuario hago 4
	map.walls = new dvector_t[map.nWalls];
	map.target = target;

	for (unsigned int i = 0; i < walls.size(); i++) {
	//genero dos paredes paralelas a la que me dieron, separadas entre si por WALL_THICKNESS
		dvector_t w1 = { (double)walls[i].start.x, (double)walls[i].start.y,
							(double)walls[i].end.x, (double)walls[i].end.y };
		dvector_t w2 = w1; //w1 y w2 paralelas a la que dibuje, separadas por THICKNESS
		dvector_t ends;	//aca se cierran los cantos

		double lenght = sqrt((w1.end.x - w1.start.x)*(w1.end.x - w1.start.x)
			+ (w1.end.y - w1.start.y)*(w1.end.y - w1.start.y));
		double offsetX = double(w1.end.y - w1.start.y) * (WALL_THICKNESS / 2) / lenght;
		double offsetY = double(w1.start.x - w1.end.x) * (WALL_THICKNESS / 2) / lenght;

		w1.start.x += offsetX;	w1.end.x += offsetX;	//muevo la pared que dibujo el usuario THICKNESS/2
		w1.start.y += offsetY;	w1.end.y += offsetY;	//para un lado para que quede centrada donde la dibujo
		map.walls[4*i] = w1;
		
		w2.start.x -= offsetX;	w2.end.x -= offsetX;	//las dos paredes quedan separadas en total por 
		w2.start.y -= offsetY;	w2.end.y -= offsetY;	//THICKNESS
		map.walls[4*i+1] = w2;

		ends.start = w1.start;	ends.end = w2.start;		//junto el origen de las dos paredes
		map.walls[4 * i + 2] = ends;

		ends.start = w1.end;	ends.end = w2.end;			//junto la punta de las dos paredes
		map.walls[4 * i + 3] = ends;
	}
	bool valid = makeMap(fileName, map);
	delete[] map.walls;

	return valid;
}

bool NewMap::setTarget(dpoint_t target)
{
	this->target = target;
	return true;
}
