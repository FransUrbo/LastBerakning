/*
 * screen.cpp
 *
 * $Id: screen.cpp,v 1.9 2010-03-06 13:36:40 turbo Exp $
 * $Revision: 1.9 $
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
	layout = createMainLayout("Välj", "Avsluta");
	listBox = (ListBox*) layout->getChildren()[0];

	/* ---------------------------------- */
	listBox->add(createLabel("Hjälptexter"));
	listBox->add(createLabel("Information, Bil"));
	listBox->add(createLabel("Information, Släp"));
	listBox->add(createLabel("Gör bruttovikts beräkning"));

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
				if(listBox->getSelectedIndex() == 3) {
					/* ---------------------------------- */
					doCalculations();

					/* ---------------------------------- */
					screens[3] = new ResultScreen(this);
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

void MyScreen::doCalculations() {
	float truck_weight, truck_load_front, truck_axles_front, truck_load_back, truck_axles_back, truck_road_nice, truck_axle, truck_link;
	float trailer_weight, trailer_load_front, trailer_axles_front, trailer_load_back, trailer_axles_back, trailer_road_nice, trailer_axle, trailer_link;

	lprintfln("------------------");
	lprintfln("Doing calculations...");

	/* ---------------------------------- */
	// Get values we need - truck
	truck_weight      = (float)atof((const char *)editBoxScreens[0]->editBox[0]->getText().c_str());
	truck_load_front  = (float)atof((const char *)editBoxScreens[0]->editBox[1]->getText().c_str());
	truck_axles_front = (float)editBoxScreens[0]->group1->getSelectedButton();
	truck_load_back   = (float)atof((const char *)editBoxScreens[0]->editBox[2]->getText().c_str());
	truck_axles_back  = (float)editBoxScreens[0]->group2->getSelectedButton();
	if(editBoxScreens[0]->checkBox[0]->isChecked())
		truck_road_nice   = (float)1;
	else
		truck_road_nice   = (float)0;
	truck_axle        = (float)atof((const char *)editBoxScreens[0]->editBox[3]->getText().c_str());
	truck_link        = (float)atof((const char *)editBoxScreens[0]->editBox[4]->getText().c_str());

	/* ---------------------------------- */
	// Get values we need - trailer
	trailer_weight      = (float)atof((const char *)editBoxScreens[1]->editBox[0]->getText().c_str());
	trailer_load_front  = (float)atof((const char *)editBoxScreens[1]->editBox[1]->getText().c_str());
	trailer_axles_front = (float)editBoxScreens[1]->group1->getSelectedButton();
	trailer_load_back   = (float)atof((const char *)editBoxScreens[1]->editBox[2]->getText().c_str());
	trailer_axles_back  = (float)editBoxScreens[1]->group2->getSelectedButton();
	if(editBoxScreens[1]->checkBox[0]->isChecked())
		trailer_road_nice   = (float)1;
	else
		trailer_road_nice   = (float)0;
	trailer_axle        = (float)atof((const char *)editBoxScreens[1]->editBox[3]->getText().c_str());
	trailer_link        = (float)atof((const char *)editBoxScreens[1]->editBox[4]->getText().c_str());

	/* ---------------------------------- */
	// Default values for debugging purposes
	weight_bk1 = 18.0; weight_bk2 = 16.7; weight_bk3 = 14.7;
	load_bk1   =  8.3; load_bk2   =  7.0; load_bk3 =  5.0;

	/* ---------------------------------- */
	weight_bk1 = truck_axles_back;

	/* TODO: Do calculations !! */

	lprintfln("Tjänstevikt, bil: %f", weight_bk1);
	lprintfln("------------------");
}
