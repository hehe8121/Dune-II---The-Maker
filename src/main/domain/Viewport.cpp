#include "Viewport.h"

void Viewport::draw(Screen * screen, int x, int y) {
	
	// determine what to draw on screen

	// z-ordering (z is already known in entity)

	// draw it on bitmap (buffer)
	
	// draw bitmap (buffer) on screen
	this->bitmap->draw(screen->getBitmap(), x, y);
}