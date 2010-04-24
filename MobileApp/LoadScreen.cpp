/*
 * LoadingScreen.cpp
 *
 * $Id: LoadScreen.cpp,v 1.8 2010-04-24 11:54:57 turbo Exp $
 */

#include <MAUtil/Moblet.h>

#include <conprint.h> /* lprintfln() */

#include "MAHeaders.h"
#include "LoadScreen.h"
#include "Version.h"
#include "Language.h"
#include "Util.h"

LoadScreen::LoadScreen(void) {
	/* ---------------------------------- */
	maSetDrawTarget(HANDLE_SCREEN);

	MAExtent screenSize = maGetScrSize();
	int scrHeight = EXTENT_Y(screenSize);

	/* ---------------------------------- */
	drawText(40, LANG_PLEASE_WAIT, 0xffffff);
	drawText(60, LANG_LOADING_TABLES, 0xffffff);

	/* ---------------------------------- */
	String str(LANG_VERSION);
	str.append(VERSION, strlen(VERSION));

	/* ---------------------------------- */
	drawText(scrHeight-16, (const char*)str.c_str(), 0xff7700);
}

LoadScreen::~LoadScreen(void) {
}

void LoadScreen::keyPressEvent(int keyCode, int nativeCode) {

}

void LoadScreen::drawText(int y, const char *string, int rgb)
{
	maSetColor(rgb);
	maDrawText(PADDING*2, y, string);
	maUpdateScreen();
}
