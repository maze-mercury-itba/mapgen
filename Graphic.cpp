#include "Graphic.h"
#include <cstdio>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_native_dialog.h>

Graphic::Graphic(const char * backgroundPath)
{
	background = NULL;
	display = NULL;
	isValid = false;

	if ((display = al_create_display(DISP_WIDTH, DISP_HEIGHT)) != NULL) {
		if (al_init_image_addon()) {
			if (al_init_primitives_addon()) {
				if ((backgroundPath != NULL && (background = al_load_bitmap(backgroundPath)) != NULL)
					|| backgroundPath == NULL) {
					isValid = true;
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

void Graphic::drawBackground(std::vector<uivector_t> * wall)
{
	if (background == NULL)
		al_clear_to_color(al_map_rgb(255, 255, 255));
	else
		al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
			0, 0, DISP_WIDTH, DISP_HEIGHT, 0);

	for (std::vector<uivector_t>::iterator it = wall->begin(); it != wall->end(); it++) {
		al_draw_line(it->start.x, it->start.y, it->end.x, it->end.y,
			al_map_rgb(0, 0, 0), WALL_THICKNESS);
	}
}

void Graphic::showChanges()
{
	al_flip_display();
}

bool Graphic::isWhite(unsigned int x, unsigned int y)
{
	bool isWhite = false;

	if (x < (unsigned int)al_get_display_width(display) && y < (unsigned int)al_get_display_height(display)) {
		unsigned char r = 0, g = 0, b = 0;
		al_unmap_rgb(al_get_pixel(al_get_backbuffer(display), x, y), &r, &g, &b);

		if (r == 255 && g == 255 && b == 255)
			isWhite = true;
	}
	
	return isWhite;
}

ALLEGRO_DISPLAY * Graphic::getDisplay()
{
	return display;
}
