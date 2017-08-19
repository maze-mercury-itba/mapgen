#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "vector.h"
#include <vector>

#define DISP_WIDTH		800
#define	DISP_HEIGHT		600	

#define WALL_THICKNESS	5

class Graphic
{
public:
	Graphic(const char * backgroundPath = NULL);
	~Graphic();

	ALLEGRO_DISPLAY * getDisplay();

	void drawBackground(std::vector<uivector_t> * walls);
	void showChanges();

	bool isWhite(unsigned int x, unsigned int y);

private:
	bool isValid;

	ALLEGRO_BITMAP * background;
	ALLEGRO_DISPLAY * display;
};