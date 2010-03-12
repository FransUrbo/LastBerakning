/*
 * LoadingScreen.cpp
 *
 * $Id: LoadScreen.cpp,v 1.2 2010-03-12 10:49:56 turbo Exp $
 */

#include <MAUtil/Moblet.h>

#include <conprint.h> /* lprintfln() */

#include "MAHeaders.h"
#include "MAUIex.h"
#include "LoadScreen.h"
#include "Util.h"

LoadScreen::LoadScreen(void) {
	maSetDrawTarget(HANDLE_SCREEN);

	maSetColor(0xffffff);
	maDrawText(PADDING*2, 40, "Var god vänta några sekunder...");
	maDrawText(PADDING*2, 60, "Laddar bruttoviktstabellerna.");

	maUpdateScreen();
}

LoadScreen::~LoadScreen(void) {
	delete mainLayout;
}

void LoadScreen::keyPressEvent(int keyCode, int nativeCode) { }
