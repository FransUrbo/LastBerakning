/*
 * LoadingScreen.cpp
 *
 * $Id: LoadScreen.cpp,v 1.4 2010-03-13 23:32:13 turbo Exp $
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
	maSetColor(0xffffff);
	maDrawText(PADDING*2, 40, "Var god vänta några sekunder...");
	maDrawText(PADDING*2, 60, "Laddar bruttoviktstabellerna.");

	/* ---------------------------------- */
	String str("Version: ");
	str.append(VERSION, strlen(VERSION));

	maSetColor(0xff7700);
	maDrawText(PADDING*2, scrHeight-16, (const char *)str.c_str());

	/* ---------------------------------- */
	maUpdateScreen();
}

LoadScreen::~LoadScreen(void) {
	delete mainLayout;
}

void LoadScreen::keyPressEvent(int keyCode, int nativeCode) { }
