/*
 * LoadingScreen.cpp
 *
 * $Id: LoadScreen.cpp,v 1.5 2010-03-16 13:55:45 turbo Exp $
 */

#include <MAUtil/Moblet.h>

#include <conprint.h> /* lprintfln() */

#include "MAHeaders.h"
#include "LoadScreen.h"
#include "Util.h"
#include "Version.h"

LoadScreen::LoadScreen(void) {
	/* ---------------------------------- */
	maSetDrawTarget(HANDLE_SCREEN);

	MAExtent screenSize = maGetScrSize();
	int scrHeight = EXTENT_Y(screenSize);

	/* ---------------------------------- */
	drawText(40, "Var god vänta några sekunder...", 0xffffff);
	drawText(60, "Laddar bruttoviktstabellerna.", 0xffffff);

	/* ---------------------------------- */
	String str("Version: ");
	str.append(VERSION, strlen(VERSION));

	/* ---------------------------------- */
	drawText(scrHeight-16, (const char*)str.c_str(), 0xff7700);
}

LoadScreen::~LoadScreen(void) {
	delete mainLayout;
}

void LoadScreen::keyPressEvent(int keyCode, int nativeCode) { }

void LoadScreen::drawText(int y, const char *string, int rgb)
{
	maSetColor(rgb);
	maDrawText(PADDING*2, y, string);
	maUpdateScreen();
}
