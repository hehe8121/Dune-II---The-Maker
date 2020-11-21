/*
 * cGameControlsContext.cpp
 *
 *  Created on: 31-okt-2010
 *      Author: Stefan
 */

#include "../include/d2tmh.h"

#define MOUSECELL_MINIMAP -2

cGameControlsContext::cGameControlsContext(cPlayer * thePlayer) {
	assert(thePlayer);
	player = thePlayer;
	mouseCell = -99;
	cellCalculator = new cCellCalculator(&map);
	drawToolTip = false;
}

cGameControlsContext::~cGameControlsContext() {
	player = NULL;
	delete cellCalculator;
}


void cGameControlsContext::updateState() {
	determineMouseCell();
	determineToolTip();
	determineHoveringOverStructureId();
	determineHoveringOverUnitId();
}

void cGameControlsContext::determineMouseCell() {
    int heightTopBar = 42;
    int screenY = cMouse::getY();
    int screenX = cMouse::getX();

    if (screenY < heightTopBar) {
		mouseCell = -1; // at the top bar or higher, so no mouse cell id.
		return;
	}

    if (drawManager->getMiniMapDrawer()->isMouseOver()) {
        mouseCell = MOUSECELL_MINIMAP ; // on minimap
        return;
    }

	if (screenX > (game.screen_x - 160)) {
		mouseCell = -3 ; // on sidebar
		return;
	}

    int result = getMouseCellFromScreen(heightTopBar, screenY, screenX);

    mouseCell = result;
}

int cGameControlsContext::getMouseCellFromScreen(int heightTopBar, int screenY, int screenX) const {
    int iMouseX = mapCamera->divideByZoomLevel(screenX);
    int iMouseY = mapCamera->divideByZoomLevel(screenY - heightTopBar);

    int viewportMouseX = iMouseX + mapCamera->getViewportStartX();
    int viewportMouseY = iMouseY + mapCamera->getViewportStartY();

    int mapX = viewportMouseX / TILESIZE_WIDTH_PIXELS;
    int mapY = viewportMouseY / TILESIZE_HEIGHT_PIXELS;

    return cellCalculator->getCellWithMapDimensions(mapX, mapY, game.map_width, game.map_height);
}

void cGameControlsContext::determineToolTip() {
	drawToolTip = false;
	if (key[KEY_T] && isMouseOnBattleField()) {
		drawToolTip = true;
	}
}

void cGameControlsContext::determineHoveringOverStructureId() {
	cStructureUtils structureUtils;

	mouseHoveringOverStructureId = -1;

	for (int i=0; i < MAX_STRUCTURES; i++) {
		cAbstractStructure * theStructure = structure[i];

		if (theStructure) {
			if (structureUtils.isStructureVisibleOnScreen(theStructure)) {
				if (structureUtils.isMouseOverStructure(theStructure, cMouse::getX(), cMouse::getY())) {
					mouseHoveringOverStructureId = i;
				}
			}
		}
	}
}

void cGameControlsContext::determineHoveringOverUnitId() {
	mouseHoveringOverUnitId=-1;
}

cAbstractStructure * cGameControlsContext::getStructurePointerWhereMouseHovers() {
	if (mouseHoveringOverStructureId < 0) {
		return NULL;
	}
	return structure[mouseHoveringOverStructureId];
}

int cGameControlsContext::getMouseCellFromMiniMap() {
	return drawManager->getMiniMapDrawer()->getMouseCell(mouse_x, mouse_y);
}
