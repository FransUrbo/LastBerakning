/*
 * main.cpp
 *
 * $Id: main.cpp,v 1.10 2010-04-24 11:54:57 turbo Exp $
 * $Revision: 1.10 $
 *
 * Program to do BKx road calculations in Sweden (EU?)
 * Copyright Turbo Fredriksson <turbo@bayour.com>
 *
 * Example and help from
 * http://www.mosync.com/documentation/tutorials/introduction-maui
 */

#include <MAUtil/Moblet.h>

#include "MAHeaders.h"
#include "LoadScreen.h"
#include "ScreenTransition.h"
#include "Language.h"
#include "screen.h"
#include "Util.h"

//This is the Moblet class. This manages the events your application will need, like getting
//key presses and screen touches.
//It also creates an instance of the MAUI::Screen class 'MainScreen', and shows it on screen.
class MAUIMoblet : public Moblet {
public:
	MAUIMoblet() {
		// initialize
		gFont = new MAUI::Font(RES_FONT);
		gSkin = new WidgetSkin(RES_TILE_SELECTED, RES_TILE_UNSELECTED, 16, 32, 16, 32, true, true);

		Engine& engine = Engine::getSingleton();

		engine.setDefaultFont(gFont);
		engine.setDefaultSkin(gSkin);

		MAExtent screenSize = maGetScrSize();
		scrWidth = EXTENT_X(screenSize);
		scrHeight = EXTENT_Y(screenSize);

		LoadScreen* ls = new LoadScreen();
		ls->show();

		screen = new MainScreen();

		/* ------------- */
		ls->drawText(80, LANG_LOADING_PROGRAM, 0xffffff);

		while(screen->main_screen_loaded != 10);
		screen->show();
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		// todo: handle key presses
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
		// todo: handle key releases
	}

	MainScreen* screen;

	~MAUIMoblet() {
		delete screen;
	}

};

//This is where your application starts. It creates a new instance of MAUIMoblet (see above)
//and runs it.
extern "C" int MAMain() {
	Moblet::run(new MAUIMoblet());
	return 0;
};
