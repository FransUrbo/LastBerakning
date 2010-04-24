/* Copyright (C) 2010 Turbo Fredriksson <turbo@bayour.com>
 *
 * This screen is the save info screen.
 *
 * $Id: SaveScreen.cpp,v 1.1 2010-04-24 09:29:25 turbo Exp $
 */

#include "SaveScreen.h"

SaveScreen::SaveScreen(Screen *previous) : previous(previous) {
	Label *label;

	/* Create the main work/text area */
	mainLayout = createMainLayout("", "Tillbaka");
	listBox = (TouchListBox*) mainLayout->getChildren()[FIRSTCHILD];

	/* Create the different label/input fields */

	/* ---------------------------------- */
	label = createLabel("Spara bil", FONTHEIGHT);
	checkBox.add(new CheckBox(scrWidth - 50, 2, 16, 16, label));
	checkBox[0]->setResources(RES_CHECKBOX_UNCHECKED, RES_CHECKBOX_CHECKED);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Spara släp", FONTHEIGHT);
	checkBox.add(new CheckBox(scrWidth - 50, 2, 16, 16, label));
	checkBox[1]->setResources(RES_CHECKBOX_UNCHECKED, RES_CHECKBOX_CHECKED);
	listBox->add(label);

	/* ---------------------------------- */
	listBox->setWrapping(WRAPPING);

	this->setMain(mainLayout);
}

SaveScreen::~SaveScreen() {
}

void SaveScreen::keyPressEvent(int keyCode, int nativeCode) {
#if DEBUG >= 1
	lprintfln("Index: %d (%d / %d)", listBox->getSelectedIndex(), keyCode, nativeCode);
#endif

	switch(keyCode) {
		case MAK_HASH:
			// Hash (#) key - ask the moblet to close the application
			maExit(0);
			break;

		case MAK_LEFT:
		case MAK_SOFTRIGHT:
			ScreenTransition::makeTransition(this, previous, -1, 400);
			break;

		case MAK_RIGHT:
		case MAK_FIRE:
			CheckBox * cb = (CheckBox *)((Label*)listBox->getChildren()[listBox->getSelectedIndex()])->getChildren()[0];
			cb->flip();
			break;

		case MAK_DOWN:
			listBox->selectNextItem();
			break;

		case MAK_UP:
			listBox->selectPreviousItem();
			break;
	}
}

void SaveScreen::pointerPressEvent(MAPoint2d point) {
	Point p;
	p.set(point.x, point.y);

	for(int i = 0; i < listBox->getChildren().size(); i++) {
		if(listBox->getChildren()[i]->contains(p)) {
			listBox->setSelectedIndex(i);

			CheckBox * cb = (CheckBox *)((Label*)listBox->getChildren()[listBox->getSelectedIndex()])->getChildren()[0];
			cb->flip();
			break;
		}
	}
}
