#include "SDL/SDL.h"
#include "CSurface.h"
#include "map.h"
#include "random.h"

Map::Map() {
  max_width = MAP_MAX_WIDTH;
  max_height = MAP_MAX_HEIGHT;

  for (int i = 0; i < MAP_MAX_SIZE; i++) {
    cells[i].tile = (flipCoin() ? 0 : 64);
  }
}

void Map::setBoundaries(int max_width, int max_height) {
  this->max_width = max_width;
  this->max_height = max_height;
}

//=============================================================================
//
MapCamera::MapCamera(int x, int y, SDL_Surface* screen, Map* map) {
  this->x = x; // pixel size, relative to map (starts at 0,0)
  this->y = y;
  this->max_cells_width_on_screen = screen->w / 32;
  this->max_cells_height_on_screen = screen->h / 32;
  this->map_y_boundary = map->getMaxHeight();
  this->map_x_boundary = map->getMaxWidth();
}

void MapCamera::draw(Map* map, SDL_Surface* tileset, SDL_Surface* screen) {
  // determine x and y from map data.
  int startX = (this->x / 32);
  int startY = (this->y / 32);

  int offsetX = (this->x % 32);
  int offsetY = (this->y % 32);

  int endX = startX + (getWidth() + 1);
  int endY = startY + (getHeight() + 1);

  for (int dx = startX; dx < endX; dx++) {
    for (int dy = startY; dy < endY; dy++) {
      Cell c = map->getCell(dx, dy);

      int mouse_x, mouse_y;
      SDL_GetMouseState(&mouse_x, &mouse_y);

      // weird: have to compensate for the coordinates above. Drawing should be done separately 
      // from coordinates of map.
      int drawX = (dx - startX) * 32;
      int drawY = (dy - startY) * 32;

      drawX -= offsetX;
      drawY -= offsetY;

      CSurface::drawTile(tileset, screen, c.tile, drawX, drawY);

      if ((mouse_x >= drawX && mouse_x <= (drawX + 32)) && (mouse_y >= drawY && mouse_y <= (drawY + 32))) {
	      SDL_Rect rect;
	      rect.w = 32;
	      rect.h = 32;
	      rect.x = drawX;
	      rect.y = drawY;
	      SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format , 0 , 0 , 20 ) );
      }
    }
  }
}

