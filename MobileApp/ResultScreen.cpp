/*
 * ResultScreen.cpp
 *
 * Code to do the actuall calculations!
 *
 * $Id: ResultScreen.cpp,v 1.4 2010-03-06 00:17:43 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */
#include <MAUtil/String.h>

#include "screen.h"
#include "EditBoxScreen.h"
#include "ResultScreen.h"
#include "ScreenTransition.h"
#include "Util.h"

ResultScreen::ResultScreen(MyScreen *previous) : previous(previous) {
	Label *label;
	ListBox *field;

	/* Create the main work/text area */
	mainLayout = createMainLayout("Radera", "Tillbaka");
	listBox = (ListBox*) mainLayout->getChildren()[0];

	/* ---------------------------------- */
	label = createLabel("Max bruttovikt", (32+(3*20)));
	field = new ListBox(	0, 20, label->getWidth()-PADDING*2, label->getHeight(),
							label, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false);
	createTextField("BK1", previous->weight_bk1, field);
	createTextField("BK2", previous->weight_bk2, field);
	createTextField("BK3", previous->weight_bk3, field);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Max Last", (32+(3*20)));
	field = new ListBox(	0, 20, label->getWidth()-PADDING*2, label->getHeight(),
							label, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false);
	createTextField("BK1", previous->load_bk1, field);
	createTextField("BK2", previous->load_bk2, field);
	createTextField("BK3", previous->load_bk3, field);
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

void ResultScreen::createTextField(const char *leader, float value, Widget *parent)
{
	Label *label;
	String string;
	char str[40];

	/* NOTE: There's probably a much better way to do this, but... */
	sprintf(str, "%02.02f", value);
	string += leader;
	string += ": ";
	string += str;

	lprintfln("Value: '%s' (%f)", str, value);

	label = new Label(0, 0, scrWidth-PADDING*2, 20, parent, string, 0, gFont);
}
