/*
 * screen.cpp
 *
 * $Id: screen.cpp,v 1.15 2010-03-16 13:31:20 turbo Exp $
 * $Revision: 1.15 $
 *
 * Copyright Turbo Fredriksson <turbo@bayour.com>
 */

#include "screen.h"
#include "LabelScreen.h"
#include "EditBoxScreen.h"
#include "ResultScreen.h"
#include "InfoScreen.h"
#include "ScreenTransition.h"
#include "Util.h"

//This is the Screen class. This is what you'll see displayed on your phone. It inherits from
//MAUI::Screen base class. To put content on the screen, you have to add widgets to it.
MainScreen::MainScreen(void) {
	/* ---------------------------------- */
	// Open data tables
	main_screen_loaded = FALSE;
	TABLE_DATA.resize(5);
	TABLE_DATA[0] = openTable("table-bk1.txt");
	TABLE_DATA[1] = openTable("table-bk2.txt");
	TABLE_DATA[2] = openTable("table-bk3.txt");
	TABLE_DATA[3] = openTable("table-boggie.txt");
	TABLE_DATA[4] = openTable("table-tripple.txt");

	/* ---------------------------------- */
	screens.add(new InfoScreen(this));

	screens.add(new LabelScreen(this));

	editBoxScreens.add(new EditBoxScreen(this));
	screens.add(editBoxScreens[0]);

	editBoxScreens.add(new EditBoxScreen(this));
	screens.add(editBoxScreens[1]);

	/* ---------------------------------- */
	layout = createMainLayout("Välj", "Avsluta");
	listBox = (ListBox*) layout->getChildren()[0];

	/* ---------------------------------- */
	listBox->add(createLabel("Program information"));
	listBox->add(createLabel("Hjälptexter"));
	listBox->add(createLabel("Information, Bil"));
	listBox->add(createLabel("Information, Släp"));
	listBox->add(createLabel("Gör bruttovikts beräkning"));

	/* ---------------------------------- */
	listBox->setWrapping(WRAPPING);
	main_screen_loaded = TRUE;

	//Set this widget as the main widget to be shown on this screen
	this->setMain(layout);
}

MainScreen::~MainScreen(void) {
	delete layout;
	for(int i = 0; i < screens.size(); i++)
		delete screens[i];
}

void MainScreen::keyPressEvent(int keyCode) {
#ifdef DEBUG2
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
				if(listBox->getSelectedIndex() == 4) {
					/* ---------------------------------- */
					doCalculations();

					/* ---------------------------------- */
					screens[4] = new ResultScreen(this);
				}

				/* ---------------------------------- */
				lprintfln("Showing screen %d", listBox->getSelectedIndex());
				ScreenTransition::makeTransition(this, screens[listBox->getSelectedIndex()], 1, 400);
				break;

			case MAK_8:
			case MAK_DOWN:
				listBox->selectNextItem(true); //Select the next item on the menu
				break;
		}
}
