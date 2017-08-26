#include "Graphic.h"
#include <cstdio>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_native_dialog.h>
#include <algorithm>


#define G_MARGIN	(std::min(width, height) * 0.01)

#define T_GRID		1
#define T_WALL		3
#define T_ACTIVE	5

#define C_GRID			al_map_rgb(0,0,0)		//negro
#define C_WALL			al_map_rgb(0,0,0)		//negro
#define C_ACTIVE		al_map_rgb(255,0,0)		//rojo
#define C_BACKGROUND	al_map_rgb(255,255,255)	//blanco


Graphic::Graphic(const char * backgroundPath)
{
	m = NULL;
	background = NULL;
	display = NULL;
	isValid = false;

	if ((display = al_create_display(DISP_WIDTH, DISP_HEIGHT)) != NULL) {
		if (al_init_image_addon()) {
			if (al_init_primitives_addon()) {
				if ((backgroundPath != NULL && (background = al_load_bitmap(backgroundPath)) != NULL)
					|| backgroundPath == NULL) {
					isValid = true;
					setUpDisplay(DISP_WIDTH, DISP_HEIGHT);
				}
				else {
					fprintf(stderr, "Unable to load background\n");
					al_destroy_display(display);
					display = NULL;
				}
			}
			else {
				fprintf(stderr, "Unable to initialize primitives add-on\n");
				al_destroy_display(display);
				display = NULL;
			}
		}
		else {
			fprintf(stderr, "Unable to install image add-on\n");
			al_destroy_display(display);
			display = NULL;
		}
	}
	else {
		fprintf(stderr, "Unable to create display\n");
	}
}

bool Graphic::setMap(NewMap * m)
{
	this->m = m;
	if (m != nullptr) {
		last = m->active;
	}

	return (isValid && (m == NULL));
}

Graphic::~Graphic()
{
	if (this->isValid) {
		if (background != NULL) {
			al_destroy_bitmap(background);
			background = NULL;
		}

		al_destroy_display(display);
		display = NULL;
	}
}

//void Graphic::drawBackground(std::vector<uivector_t> * wall)
//{
//	if (background == NULL)
//		al_clear_to_color(al_map_rgb(255, 255, 255));
//	else
//		al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
//			0, 0, DISP_WIDTH, DISP_HEIGHT, 0);
//
//	for (std::vector<uivector_t>::iterator it = wall->begin(); it != wall->end(); it++) {
//		al_draw_line(it->start.x, it->start.y, it->end.x, it->end.y,
//			al_map_rgb(0, 0, 0), WALL_THICKNESS);
//	}
//}

void Graphic::showChanges()
{
	al_flip_display();
}

void Graphic::setUpDisplay(unsigned int width, unsigned int height)
{
	map0.x = map0.y = G_MARGIN;

	mapSize.x = width - G_MARGIN - map0.x;
	mapSize.y = height - G_MARGIN - map0.y;

	if (COLS * PANEL_LEN_X / (ROWS * PANEL_LEN_Y) > mapSize.x / mapSize.y ) { //tengo mas ancho del que quiero
		map0.y += mapSize.y / 2;
		mapSize.y = mapSize.x * (ROWS * PANEL_LEN_Y) / (COLS * PANEL_LEN_X);
		map0.y -= mapSize.y / 2;
	}
	else {
		map0.x += mapSize.x / 2;
		mapSize.x = mapSize.y * COLS * PANEL_LEN_X / (ROWS * PANEL_LEN_Y);
		map0.x -= mapSize.x / 2;
	}



}

void Graphic::drawWall(wallPos_t w, unsigned int thickness, ALLEGRO_COLOR c)
{
	fvector_t line;

	line.start.x = w.column * PANEL_LEN_X;
	line.start.y = w.row * PANEL_LEN_Y;
	line.end = line.start;

	if (w.horizontal) {
		line.end.x += PANEL_LEN_X;
	}
	else {
		line.end.y += PANEL_LEN_Y;
	}

	line = scaleVector(line);
	line.start.x += map0.x;		line.end.x += map0.x;
	line.start.y += map0.y;		line.end.y += map0.y;

	al_draw_line(line.start.x, line.start.y, line.end.x, line.end.y, c, thickness);
	al_flip_display();
}

void Graphic::redrawGrid()
{
	al_clear_to_color(C_BACKGROUND);
	wallPos_t w;	w.horizontal = true;
	
	for (unsigned int i = 0; i < ROWS + 1; i++) {
		w.row = i;
		for (unsigned int j = 0; j < COLS; j++) {
			w.column = j;
			drawWall(w, T_GRID, C_GRID);
		}
	}

	w.horizontal = false;
	for (unsigned int i = 0; i < ROWS; i++) {
		w.row = i;
		for (unsigned int j = 0; j < COLS + 1; j++) {
			w.column = j;
			drawWall(w, T_GRID, C_GRID);
		}
	}

	if (m != nullptr) {
		last = m->active;
	}

	al_flip_display();
}

void Graphic::updateGrid(bool currPos)
{
	wallPos_t w = currPos ? m->active : last;

	bool newWall = w.horizontal ? m->horizontal[w.row][w.column] : m->vertical[w.row][w.column];

	if (newWall) {
		drawWall(w, T_WALL, C_WALL);
	}
	else {
		drawWall(w, T_WALL, C_BACKGROUND); //borro la pared que estaba
		drawWall(w, T_GRID, C_GRID);
	}

	al_flip_display();
}

void Graphic::updateActive()
{
	drawWall(last, T_ACTIVE, C_BACKGROUND);
	updateGrid(false);
	drawWall(m->active, T_ACTIVE, C_ACTIVE);
	last = m->active;
	al_flip_display();
}

ALLEGRO_DISPLAY * Graphic::getDisplay()
{
	return display;
}


fvector_t Graphic::scaleVector(fvector_t v)
{
	fvector_t ans;
	ans.start = scalePoint(v.start);
	ans.end = scalePoint(v.end);
	return ans;
}

fpoint_t Graphic::scalePoint(fpoint_t p)
{
	fpoint_t ans;
	ans.x = p.x / MAX_WIDTH * mapSize.x + map0.x;
	ans.y = p.y / MAX_HEIGHT * mapSize.y + map0.y;

	ans.x = std::max(ans.x, map0.x); ans.x = std::min(ans.x, map0.x+mapSize.x);
	ans.y = std::max(ans.y, map0.y); ans.y = std::min(ans.y, map0.y+mapSize.y);

	return ans;
}