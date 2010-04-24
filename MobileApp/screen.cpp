/*
 * screen.cpp
 *
 * $Id: screen.cpp,v 1.19 2010-04-24 11:54:57 turbo Exp $
 * $Revision: 1.19 $
 *
 * Copyright Turbo Fredriksson <turbo@bayour.com>
 */

#include "screen.h"
#include "LabelScreen.h"
#include "EditBoxScreen.h"
#include "ResultScreen.h"
#include "InfoScreen.h"
#include "ScreenTransition.h"
#include "SaveScreen.h"
#include "Language.h"
#include "Util.h"

//This is the Screen class. This is what you'll see displayed on your phone. It inherits from
//MAUI::Screen base class. To put content on the screen, you have to add widgets to it.
MainScreen::MainScreen(void) {
	/* ---------------------------------- */
	// Open data tables
	main_screen_loaded = 0;
	TABLE_DATA.resize(5);

	TABLE_DATA[0] = openTable("table-bk1.txt");
	main_screen_loaded++;

	TABLE_DATA[1] = openTable("table-bk2.txt");
	main_screen_loaded++;

	TABLE_DATA[2] = openTable("table-bk3.txt");
	main_screen_loaded++;

	TABLE_DATA[3] = openTable("table-boggie.txt");
	main_screen_loaded++;

	TABLE_DATA[4] = openTable("table-tripple.txt");
	main_screen_loaded++;

	/* ---------------------------------- */
	// Create menu layout
	layout = createMainLayout(LANG_CHOOSE, LANG_QUIT);
	listBox = (TouchListBox*)layout->getChildren()[FIRSTCHILD];

	/* ---------------------------------- */
	// Create screens
	listBox->add(createLabel(LANG_INFO));
	screens.add(new InfoScreen(this));

	listBox->add(createLabel(LANG_HELP));
	screens.add(new LabelScreen(this));

	listBox->add(createLabel(LANG_INFO_TRUCK));
	trailer_screen = FALSE;
	editBoxScreens.add(new EditBoxScreen(this));
	screens.add(editBoxScreens[0]);

	listBox->add(createLabel(LANG_INFO_TRAILER));
	trailer_screen = TRUE;
	editBoxScreens.add(new EditBoxScreen(this));
	screens.add(editBoxScreens[1]);

	listBox->add(createLabel(LANG_CALCULATE, FONTHEIGHT*2));
	screens.add(NULL);

//	listBox->add(createLabel(LANG_INFO_SAVE));
	screens.add(new SaveScreen(this));

	/* ---------------------------------- */
	listBox->setWrapping(WRAPPING);

	//Set this widget as the main widget to be shown on this screen
	this->setMain(layout);

	main_screen_loaded = 10;
}

MainScreen::~MainScreen(void) {
	delete layout;
	for(int i = 0; i < screens.size(); i++)
		delete screens[i];
}

void MainScreen::keyPressEvent(int keyCode) {
#if DEBUG >= 2
		lprintfln("Keycode='%x'", keyCode);
#endif

	//A full list of the key constants is available at http://www.mosync.com/docs/2.0b1/html/maapi_8h.html
	switch(keyCode) {
		case MAK_SOFTRIGHT:
		case MAK_HASH:
			// Hash (#) key - ask the moblet to close the application
			maExit(0);
			break;

		case MAK_2:
		case MAK_UP:
			listBox->selectPreviousItem(true); //Select the previous item on the menu
			break;

		case MAK_5:
		case MAK_SOFTLEFT:
		case MAK_RIGHT:
		case MAK_FIRE:
			int index = listBox->getSelectedIndex();

			if(index == 4) {
				/* ---------------------------------- */
				doCalculations();

				/* ---------------------------------- */
				screens[4] = new ResultScreen(this);
			}

			/* ---------------------------------- */
#if DEBUG >= 2
			lprintfln("Showing screen %d", index);
#endif
			ScreenTransition::makeTransition(this, screens[index], 1, 400);
			break;

		case MAK_8:
		case MAK_DOWN:
			listBox->selectNextItem(true); //Select the next item on the menu
			break;
	}
}

void MainScreen::pointerPressEvent(MAPoint2d point) {
	Point p;
	p.set(point.x, point.y);

	for(int i = 0; i < listBox->getChildren().size(); i++) {
		if(listBox->getChildren()[i]->contains(p)) {
			listBox->setSelectedIndex(i);

			if(listBox->getSelectedIndex() == 4) {
				/* ---------------------------------- */
				doCalculations();

				/* ---------------------------------- */
				screens[4] = new ResultScreen(this);
			}

			/* ---------------------------------- */
#if DEBUG >= 2
			lprintfln("Showing screen (touch) %d", listBox->getSelectedIndex());
#endif
			ScreenTransition::makeTransition(this, screens[listBox->getSelectedIndex()], 1, 400);

			break;
		}
	}
}
