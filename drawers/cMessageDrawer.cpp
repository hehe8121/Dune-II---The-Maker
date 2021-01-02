/*
 * cMessageDrawer.cpp
 *
 *  Created on: 11-aug-2010
 *      Author: Stefan
 */

#include "../include/d2tmh.h"

/*
 * The cMessageDrawer draws messages IN-GAME. This is NOT used for region texts.
 * 
 */
cMessageDrawer::cMessageDrawer() {
	init();
}

cMessageDrawer::~cMessageDrawer() {
	init();
}

void cMessageDrawer::init() {
	iMessageAlpha = -1;
	memset(cMessage, 0, sizeof(cMessage));
	TIMER_message = 0;
	initCombatPosition();
}

void cMessageDrawer::think() {
	int iLimit=250;

	if (game.isState(GAME_REGION)) {
		iLimit=600;
	}

	if (cMessage[0] != '\0')
	{
		TIMER_message++;

		// and clear message after shown
		if (TIMER_message > iLimit) {
			memset(cMessage, 0, sizeof(cMessage));
		}

		iMessageAlpha+=3;
		// fade in
		if (iMessageAlpha > 254) {
			iMessageAlpha = 255;
		}
	}
	else
	{
		iMessageAlpha-=6;
		if (iMessageAlpha < 0) {
			iMessageAlpha=-1;
		}

		TIMER_message=0;
	}
}

/**
 * This sets a message on the message bar, which will fade out
 * after a specific amount of time and then get cleared again.
 *
 * @param msg
 */
void cMessageDrawer::setMessage(const char msg[255]) {
	TIMER_message=0;
	memset(cMessage, 0, sizeof(cMessage));
	sprintf(cMessage, "%s", msg);
}

void cMessageDrawer::draw() {
	if (iMessageAlpha > -1) {
		set_trans_blender(0,0,0,iMessageAlpha);
		int width = 480; // this is fixed, see BMP_MESSAGEBAR (it is one fixed image, so need to chop it up)
		
		BITMAP *temp = create_bitmap(width,30);
		clear_bitmap(temp);
		rectfill(temp, 0,0,width,40, makecol(255,0,255));
		draw_sprite(temp, (BITMAP *)gfxinter[BMP_MESSAGEBAR].dat, 0,0);

		// draw message
		alfont_textprintf(temp, game_font, 13,21, makecol(0,0,0), cMessage);

		// draw temp
		draw_trans_sprite(bmp_screen, temp, x, y);

		destroy_bitmap(temp);
	}
}

void cMessageDrawer::initRegionPosition(int offsetX, int offsetY) {
    // default positions region mode
    x = offsetX + 73;
    y = offsetY + 358;
}

void cMessageDrawer::initCombatPosition() {
    // default positions in-game (battle mode)
    x = 1;
    y = 42;
}