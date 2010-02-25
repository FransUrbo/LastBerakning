/*
 * screen.cpp
 *
 * $Id: screen.cpp,v 1.2 2010-02-25 16:44:12 turbo Exp $
 * $Revision: 1.2 $
 *
 * Copyright Turbo Fredriksson <turbo@bayour.com>
 */

#include "screen.h"
#include "LabelScreen.h"
#include "EditBoxScreen.h"
#include "Util.h"

//This is the Screen class. This is what you'll see displayed on your phone. It inherits from
//MAUI::Screen base class. To put content on the screen, you have to add widgets to it.
MyScreen::MyScreen(void) {
	screens.add(new LabelScreen(this));
	screens.add(new EditBoxScreen(this));

	layout = createMainLayout("Välj", "Avsluta");
	listBox = (ListBox*) layout->getChildren()[0];

	listBox->add(createLabel("Hjälptexter"));
	listBox->add(createLabel("Beräkna bruttovikt"));

	// BUG/Counter-intuitive!!
	// createMainLayout creates a listbox with wrapping=true.
	// This don't work!! Works if I change to 'false' though!!
	listBox->setWrapping(false);

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
				//Decide on the action you want to perform when an option is selected.
				//You can get the selected option with
				//listBox->getSelectedIndex();
				lprintfln("Showing screen %d", listBox->getSelectedIndex());
				screens[listBox->getSelectedIndex()]->show();
				break;

			case MAK_8:
			case MAK_DOWN:
				listBox->selectNextItem(true); //Select the next item on the menu
				break;
		}
}
