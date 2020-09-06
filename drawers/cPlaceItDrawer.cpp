/*
 * cPlaceItDrawer.cpp
 *
 *  Created on: 12-aug-2010
 *      Author: Stefan
 */

#include "../include/d2tmh.h"

cPlaceItDrawer::cPlaceItDrawer() {
	cellCalculator = new cCellCalculator(&map);
}

cPlaceItDrawer::~cPlaceItDrawer() {
	delete cellCalculator;
}

void cPlaceItDrawer::draw(cBuildingListItem *itemToPlace) {
	assert(itemToPlace);
	assert(itemToPlace->getBuildType() == STRUCTURE);

	// this is only done when bPlaceIt=true
	if (player[HUMAN].getSideBar() == NULL) {
		return;
	}

	int iMouseCell = player[HUMAN].getGameControlsContext()->getMouseCell();

	if (iMouseCell < 0) {
		return;
	}

	drawStructureIdAtCell(itemToPlace, iMouseCell);
	drawStatusOfStructureAtCell(itemToPlace, iMouseCell);
}

void cPlaceItDrawer::drawStatusOfStructureAtCell(cBuildingListItem *itemToPlace, int mouseCell) {
	assert(itemToPlace);
	if (mouseCell < 0) return;

	cStructureUtils structureUtils;
	int structureId = itemToPlace->getBuildId();
	assert(structureId > -1);

	bool bWithinBuildDistance=false;
	bool bMayPlace=true;

	int iTile = PLACE_ROCK;	// rocky placement = ok, but bad for power

    int width = structures[structureId].bmp_width;
    int height = structures[structureId].bmp_height;
    int scaledWidth = mapCamera->factorZoomLevel(width);
    int scaledHeight = mapCamera->factorZoomLevel(height);
    int cellWidth = structureUtils.getWidthOfStructureTypeInCells(structureId);
    int cellHeight = structureUtils.getHeightOfStructureTypeInCells(structureId);


    //
	int iTotalBlocks = cellWidth * cellHeight;

	int iTotalRocks=0.0;

#define SCANWIDTH	1

	int iCellX = cellCalculator->getX(mouseCell);
	int iCellY = cellCalculator->getY(mouseCell);

	// check
	int iStartX = iCellX-SCANWIDTH;
	int iStartY = iCellY-SCANWIDTH;

	int iEndX = iCellX + SCANWIDTH + cellWidth;
	int iEndY = iCellY + SCANWIDTH + cellHeight;

	// Fix up the boundaries
	FIX_POS(iStartX, iStartY);
	FIX_POS(iEndX, iEndY);

    BITMAP *temp;
    temp = create_bitmap(scaledWidth+1, scaledHeight+1);
    clear_bitmap(temp);

	// Determine if structure to be placed is within build distance
	for (int iX=iStartX; iX < iEndX; iX++) {
		for (int iY=iStartY; iY < iEndY; iY++) {
            int iCll = iCellMakeWhichCanReturnMinusOne(iX, iY);

			if (iCll > -1) {
                int idOfStructureAtCell = map.getCellIdStructuresLayer(iCll);
                if (idOfStructureAtCell > -1) {
                    int iID = idOfStructureAtCell;

                    if (structure[iID]->getOwner() == HUMAN) {
                        bWithinBuildDistance = true; // connection!
                    }
                }

                if (map.getCellType(iCll) == TERRAIN_WALL ||
                    map.getCellType(iCll) == TERRAIN_SLAB) {
                    bWithinBuildDistance=true;
                    // TODO: here we should actually find out if the slab is ours or not??
                }
            }
		}
	}

	if (!bWithinBuildDistance) {
		bMayPlace=false;
	}

	int iDrawX = map.mouse_draw_x();
	int iDrawY = map.mouse_draw_y();

	// Draw over it the mask for good/bad placing (decorates temp bitmap)
	for (int iX=0; iX < cellWidth; iX++) {
		for (int iY=0; iY < cellHeight; iY++) {
			iTile = PLACE_ROCK;

            int cellX = iCellX + iX;
            int cellY = iCellY + iY;

            if (cellX < 1 || cellY < 1 || cellX > (game.map_width - 2) || cellX > (game.map_height - 2)) {
                // out of bounds
                bWithinBuildDistance=false;
                bMayPlace=false;
                break;
            }

            int iCll = iCellMake(cellX, cellY);

			if (!map.isCellPassable(iCll))
				iTile = PLACE_BAD;

			if (map.getCellType(iCll) != TERRAIN_ROCK)
				iTile = PLACE_BAD;

			if (map.getCellType(iCll) == TERRAIN_SLAB)
				iTile = PLACE_GOOD;

			// occupied by units or structures
            int idOfStructureAtCell = map.getCellIdStructuresLayer(iCll);
            if (idOfStructureAtCell > -1)
				iTile = PLACE_BAD;

			int unitIdOnMap = map.getCellIdUnitLayer(iCll);
			if (unitIdOnMap > -1) {
				if (!unit[unitIdOnMap].bPickedUp) // only when not picked up, take this in (fixes carryall carrying this unit bug)
					iTile = PLACE_BAD;
			}

			// DRAWING & RULER
			if (iTile == PLACE_BAD && structureId != SLAB4)
				bMayPlace=false;

			// Count this as GOOD stuff
			if (iTile == PLACE_GOOD)
				iTotalRocks++;

			// Draw bad gfx on spot
            allegroDrawer->stretchBlitFromGfxData(iTile, temp, 0, 0, 32, 32,
                iX*mapCamera->getZoomedTileWidth(),
                iY*mapCamera->getZoomedTileHeight(),
                mapCamera->getZoomedTileWidth(),
                mapCamera->getZoomedTileHeight());
		}

		if (!bWithinBuildDistance) {
			clear_to_color(temp, makecol(160,0,0));
		}
	}

	// draw temp bitmap
	set_trans_blender(0, 0, 0, 64);

	draw_trans_sprite(bmp_screen, temp, iDrawX, iDrawY);

	// reset to normal
	set_trans_blender(0, 0, 0, 128);

	destroy_bitmap(temp);

	// clicked mouse button
	if (cMouse::isLeftButtonClicked()) {
		if (bMayPlace && bWithinBuildDistance)	{
			int iHealthPercent = 50; // the minimum is 50% (with no slabs)

			if (iTotalRocks > 0) {
				iHealthPercent += health_bar(50, iTotalRocks, iTotalBlocks);
			}

            play_sound_id(SOUND_PLACE);

			player[HUMAN].getStructurePlacer()->placeStructure(mouseCell, structureId, iHealthPercent);

			game.bPlaceIt=false;

			itemToPlace->decreaseTimesToBuild();
			itemToPlace->setPlaceIt(false);
			itemToPlace->setIsBuilding(false);
			itemToPlace->setProgress(0);
			if (itemToPlace->getTimesToBuild() < 1) {
				player[HUMAN].getItemBuilder()->removeItemFromList(itemToPlace);
			}
		}
	}
}

void cPlaceItDrawer::drawStructureIdAtCell(cBuildingListItem *itemToPlace, int cell) {
	assert(itemToPlace);
	assert(cell >= 0);

	int structureId = itemToPlace->getBuildId();

	int iDrawX = map.mouse_draw_x();
	int iDrawY = map.mouse_draw_y();

    BITMAP *temp;
    int width = structures[structureId].bmp_width;
    int height = structures[structureId].bmp_height;

    int scaledWidth = mapCamera->factorZoomLevel(width);
    int scaledHeight = mapCamera->factorZoomLevel(height);

    temp = create_bitmap(scaledWidth+1, scaledHeight+1);
    clear_to_color(temp, makecol(255, 0, 255));

    BITMAP *bmp = nullptr;
    if (structureId == SLAB1) {
		bmp = (BITMAP *)gfxdata[PLACE_SLAB1].dat;
	} else if (structureId == SLAB4) {
        bmp = (BITMAP *)gfxdata[PLACE_SLAB4].dat;
	} else if (structureId == WALL) {
        bmp = (BITMAP *)gfxdata[PLACE_WALL].dat;
	} else {
        bmp = player->getStructureBitmap(structureId);
	}

    allegroDrawer->stretchBlit(bmp, temp, 0, 0, width, height, 0, 0, scaledWidth, scaledHeight);

    draw_trans_sprite(bmp_screen, temp, iDrawX, iDrawY);

    destroy_bitmap(temp);
}
