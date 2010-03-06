/*
 * screen.cpp
 *
 * $Id: screen.cpp,v 1.8 2010-03-06 00:22:45 turbo Exp $
 * $Revision: 1.8 $
 *
 * Copyright Turbo Fredriksson <turbo@bayour.com>
 */

#include <madmath.h>

#include "screen.h"
#include "LabelScreen.h"
#include "EditBoxScreen.h"
#include "ResultScreen.h"
#include "ScreenTransition.h"
#include "Util.h"

//This is the Screen class. This is what you'll see displayed on your phone. It inherits from
//MAUI::Screen base class. To put content on the screen, you have to add widgets to it.
MyScreen::MyScreen(void) {
	/* ---------------------------------- */
	screens.add(new LabelScreen(this));

	editBoxScreens.add(new EditBoxScreen(this));
	screens.add(editBoxScreens[0]);

	editBoxScreens.add(new EditBoxScreen(this));
	screens.add(editBoxScreens[1]);

	/* ---------------------------------- */
	layout = createMainLayout("V�lj", "Avsluta");
	listBox = (ListBox*) layout->getChildren()[0];

	/* ---------------------------------- */
	listBox->add(createLabel("Hj�lptexter"));
	listBox->add(createLabel("Information, Bil"));
	listBox->add(createLabel("Information, Sl�p"));
	listBox->add(createLabel("G�r bruttovikts ber�kning"));

	/* ---------------------------------- */
	listBox->setWrapping(WRAPPING);

	//Set this widget as the main widget to be shown on this screen
	this->setMain(layout);
}

MyScreen::~MyScreen(void) {
	delete layout;
	for(int i = 0; i < screens.size(); i++)
		delete screens[i];
}

void MyScreen::keyPressEvent(int keyCode) {
		lprintfln("Keycode='%x'", keyCode);

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
				/* ---------------------------------- */
				doCalculations();

				/* ---------------------------------- */
				screens[3] = new ResultScreen(this);

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

void MyScreen::doCalculations() {
	/* TODO: Do calculations !! */
	lprintfln("------------------");
	lprintfln("Doing calculations...");

	/* ---------------------------------- */
	weight_bk1 = 18.0; weight_bk2 = 16.7; weight_bk3 = 14.7;
	load_bk1   =  8.3; load_bk2   =  7.0; load_bk3 =  5.0;

	/* ---------------------------------- */
	weight_bk1 = getTruckWeight();

	lprintfln("Tj�nstevikt, bil: %f", weight_bk1);
	lprintfln("------------------");
}

float MyScreen::getTruckWeight() {
	float value = (float)atof((const char *)editBoxScreens[0]->editBox[0]->getText().c_str());
	return(value);
}
