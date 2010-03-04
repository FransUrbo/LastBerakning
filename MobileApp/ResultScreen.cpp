/*
 * ResultScreen.cpp
 *
 * Code to do the actuall calculations!
 *
 * $Id: ResultScreen.cpp,v 1.1 2010-03-04 18:28:41 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */
#include <MAUtil/String.h>

#include "ResultScreen.h"
#include "ScreenTransition.h"
#include "Util.h"

ResultScreen::ResultScreen(Screen *previous) : previous(previous) {
	Label *label;
	ListBox *field;

	/* Create the main work/text area */
	mainLayout = createMainLayout("Radera", "Tillbaka");
	listBox = (ListBox*) mainLayout->getChildren()[0];

	/* ---------------------------------- */
	label = createLabel("Max bruttovikt", (32+(3*20)));
	field = new ListBox(	0, 20, label->getWidth()-PADDING*2, label->getHeight(),
							label, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false);
	createTextField("BK1", "18", field);
	createTextField("BK2", "16.7", field);
	createTextField("BK3", "14.7", field);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Max Last", (32+(3*20)));
	field = new ListBox(	0, 20, label->getWidth()-PADDING*2, label->getHeight(),
							label, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false);
	createTextField("BK1", "8.3", field);
	createTextField("BK2", "7", field);
	createTextField("BK3", "5", field);
	listBox->add(label);

	/* ---------------------------------- */
	listBox->setWrapping(WRAPPING);

	this->setMain(mainLayout);
}

ResultScreen::~ResultScreen() {
}

void ResultScreen::show() {
	listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(true);
	Screen::show();
}

void ResultScreen::hide() {
	listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(false);
	Screen::hide();
}

void ResultScreen::keyPressEvent(int keyCode, int nativeCode) {
	lprintfln("Index: %d", listBox->getSelectedIndex());

	switch(keyCode) {
		case MAK_HASH:
			// Hash (#) key - ask the moblet to close the application
			maExit(0);
			break;

		case MAK_LEFT:
		case MAK_SOFTRIGHT:
			lprintfln("Showing previous screen...");
			ScreenTransition::makeTransition(this, previous, -1, 400);
			break;

		case MAK_UP:
			lprintfln("selectPreviousItem()");
			listBox->selectPreviousItem();
			break;

		case MAK_DOWN:
			lprintfln("selectNextItem()");
			listBox->selectNextItem();
			break;
	}
	lprintfln("keyPressEvent() done...");
}

void ResultScreen::createTextField(const char *leader, const char *value, Widget *parent)
{
	Label *label;
	String str;

	str += leader;
	str += ": ";
	str += value;

	label = new Label(0, 0, scrWidth-PADDING*2, 20, parent, str, 0, gFont);
}
