/*
 * cMessageDrawer.h
 *
 *  Created on: 11-aug-2010
 *      Author: Stefan
 */

#ifndef CMESSAGEDRAWER_H_
#define CMESSAGEDRAWER_H_

class cMessageDrawer {
	public:
		cMessageDrawer();
		~cMessageDrawer();

		void draw();

		void think(); // timer based thinking to fade in/out message

		void init();

		void initRegionPosition(int offsetX, int offsetY);

		void initCombatPosition();

		void setMessage(const char msg[255]); // set a new message
		char * getMessage() { return &cMessage[0]; }
		bool hasMessage() { return cMessage[0] != '\0'; }

	protected:

	private:
		// MESSAGING / HELPER
		char cMessage[255];
		int iMessageAlpha;
		int TIMER_message;
		int x, y;	// draw positions
};

#endif /* CMESSAGEDRAWER_H_ */
