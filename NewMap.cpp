#include "NewMap.h"
#include <algorithm>
#include <cmath>
#include "mapFile.h"

#define LAST_ROW	(active.horizontal ? ROWS : ROWS - 1)
#define LAST_COL	(active.horizontal ? COLS - 1 : COLS)

NewMap::NewMap()
{
	target.x = UINT16_MAX;
	memset(horizontal, false, sizeof(horizontal));
	memset(vertical, false, sizeof(vertical));

	active.column = active.row = 0;
	active.horizontal = true; //empiezo en un lugar arbitrario del mapa
}
NewMap::~NewMap()
{
}

//std::vector<uivector_t> * NewMap::getCurrWalls()
//{
//	return &walls;
//}

wallPos_t NewMap::getActiveWall()
{
	return active;
}

void NewMap::rotateActiveWall()
{
	active.horizontal = !active.horizontal;	//cambio el sentido que tenia
	
	//me aseguro de que no se me haya salido del mapa
	while (active.column > LAST_COL) {
		active.column--;
	}
	while (active.row > LAST_ROW) {
		active.row--;
	}
}

void NewMap::moveActiveWall(direction_t dir)
{
	switch (dir) {
	case LEFT: {
		if (active.column == 0) {	//esta a la izq de todo: va para la otra punta
			active.column = LAST_COL;
		}
		else {
			active.column--;
		}
	} break;

	case RIGHT: {
		if (active.column == LAST_COL) {
			active.column = 0;
		}
		else {
			active.column++;
		}
	} break;

	case UP: {
		if (active.row == 0) {
			active.row = LAST_ROW;
		}
		else {
			active.row--;
		}
	} break;

	case DOWN: {
		if (active.row == LAST_ROW) {
			active.row = 0;
		}
		else {
			active.row++;
		}
	} break;
	}
}

bool NewMap::toggleActiveWall()
{
	bool valid;
	bool * wall = activeWall();
	if (wall != nullptr) {
		valid = true;
		*wall = !(*wall);
	}
	return valid;
}

//bool NewMap::newWall()
//{
//	bool valid = false;
//
//	bool * wall = activeWall();
//	if (wall != nullptr && *wall == false) {
//		valid = *wall = true;
//	}
//
//	return valid;
//}
//
//bool NewMap::removeWall()
//{
//	bool valid = false;
//	bool * wall = activeWall();
//
//	if (wall != nullptr && *wall == true) {
//		*wall = false;
//		valid = true;
//	}
//
//	return valid;
//}
//
//bool NewMap::removeWall(uipoint_t p)
//{
//	std::vector<uivector_t>::iterator it;
//	for (it = walls.begin(); it != walls.end(); ++it)
//	{
//		if (p.x <= std::max(it->end.x, it->start.x) && p.y <= std::max(it->end.y, it->start.y) &&
//			p.x >= std::min(it->end.x, it->start.x) && p.y <= std::max(it->end.y, it->start.y))
//		{
//			double distance = abs((it->end.y - it->start.y)*p.x - (it->end.x - it->start.x)*p.y + it->end.x * it->start.y - it->end.y * it->start.x);
//			distance /= sqrt((it->end.y - it->start.y)*(it->end.y - it->start.y) + (it->end.x - it->start.x)*(it->end.x - it->start.x));
//
//			if (distance <= round(WALL_THICKNESS / 2.0)) {
//				walls.erase(it);
//				return true;
//			}
//		}
//	}
//
//	return false;
//}
//
//bool NewMap::newWall(uivector_t wall)
//{
//	double lenght = sqrt((wall.end.x - wall.start.x)*(wall.end.x - wall.start.x)
//		+ (wall.end.y - wall.start.y)*(wall.end.y - wall.start.y));
//
//	if (lenght <= 3*WALL_THICKNESS)	{
//		return false;
//	}
//
//
//	this->walls.push_back(wall);
//	return true;
//}

bool NewMap::makeMapFile(char * fileName)
{
	if (target.x == UINT16_MAX || fileName == NULL)
		return false;	//puedo hacer un mapa sin paredes, pero no sin target!

	map_t map;	map.target = target;
	std::vector<dvector_t> walls;
	
	dvector_t w;

	for (unsigned int i = 0; i <= ROWS; i++) { 	//paredes horizontales!
		unsigned int j = 0;
		while (j < COLS) {
			if (horizontal[i][j]) {
				w.start.y = w.end.y = i * PANEL_LEN_Y;
				w.start.x = j * PANEL_LEN_X;

				while (j+1 < COLS && horizontal[i][j + 1]) {
					j++;
				}

				w.end.x = (j + 1) * PANEL_LEN_X;
				walls.push_back(w);
			}
			j++;
		}
	}

	for (unsigned int j = 0; j <= COLS; j++) {
		unsigned int i = 0;
		while (i < ROWS) {
			if (vertical[i][j]) {
				w.start.x = w.end.x = j * PANEL_LEN_X;
				w.start.y = i * PANEL_LEN_Y;

				while (i + 1 < ROWS && vertical[i + 1][j]) {
					i++;
				}

				w.end.y = (i + 1) * PANEL_LEN_Y;
				walls.push_back(w);
			}
			i++;
		}
	}

	map.nWalls = walls.size() * 4;
	map.walls = new dvector_t[map.nWalls];

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

bool * NewMap::activeWall()
{
	bool * ans = nullptr;
	if (active.row <= LAST_ROW && active.column <= LAST_COL) {
		if (active.horizontal) {
			ans = &horizontal[active.row][active.column];
		}
		else {
			ans = &vertical[active.row][active.column];
		}
	}

	return ans;
}
