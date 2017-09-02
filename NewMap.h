#pragma once
#include <vector>
#include <list>
#include "vector.h"

typedef enum dirs { LEFT, RIGHT, UP, DOWN } direction_t;

#define WALL_THICKNESS	.05
#define PANEL_LEN_X		.40
#define PANEL_LEN_Y		.40
#define MAX_WIDTH		(7.2 * PANEL_LEN_X)
#define MAX_HEIGHT		(6.2 * PANEL_LEN_Y)

#define ROWS (unsigned int(MAX_HEIGHT/PANEL_LEN_X))
#define COLS (unsigned int(MAX_WIDTH/PANEL_LEN_Y))

typedef struct {
	bool horizontal;
	unsigned int row;
	unsigned int column;
} wallPos_t;

class NewMap
{
public:
	friend class Graphic;

	NewMap();
	~NewMap();

//	std::vector<uivector_t> * getCurrWalls(); // para mostrar
	
	wallPos_t getActiveWall();
	void rotateActiveWall();
	void moveActiveWall(direction_t dir);
	bool toggleActiveWall();

//	bool newWall();
//	bool removeWall();

//	bool removeWall(uipoint_t p);
	//borrar pared que contenga el punto p; devuelve si la pudo borrar o no


//	bool newWall(uivector_t wall);
	//crear una nueva pared entre p1 y p2; devuelve si la pudo crear o no
	
	bool makeMapFile(char * fileName);

	bool setTarget(dpoint_t target);
private:
	wallPos_t active;

	bool vertical[ROWS][COLS + 1];
	bool horizontal[ROWS + 1][COLS];

	bool * activeWall();
//	std::vector<uivector_t> walls;
	//las paredes que vad recibiendo, y las que van quedando duplicadas considerando la thickness
	dpoint_t target;
};