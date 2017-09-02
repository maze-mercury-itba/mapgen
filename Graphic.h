#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "vector.h"
#include <vector>
#include "NewMap.h"

#define DISP_WIDTH		800
#define	DISP_HEIGHT		600	


class Graphic
{
public:
	Graphic(const char * backgroundPath = NULL);
	bool setMap(NewMap * m);
	~Graphic();

	ALLEGRO_DISPLAY * getDisplay();

	void redrawGrid();
	void updateGrid(bool currPos = true);
	void updateActive();
	void showChanges();

//	bool isWhite(unsigned int x, unsigned int y);

private:
	bool valid;

	void setUpDisplay(unsigned int width, unsigned int height);
	void drawWall(wallPos_t w, unsigned int thickness, ALLEGRO_COLOR c);	

	void rescaleMap();
	fvector_t scaleVector(fvector_t wall);
	fpoint_t scalePoint(fpoint_t p);

	const NewMap * m;
	fpoint_t map0;	//en pixeles
	fpoint_t mapSize;
	wallPos_t last;

	ALLEGRO_BITMAP * background;
	ALLEGRO_DISPLAY * display;
};