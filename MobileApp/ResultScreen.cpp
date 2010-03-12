/*
 * ResultScreen.cpp
 *
 * Code to do the actuall calculations!
 *
 * $Id: ResultScreen.cpp,v 1.6 2010-03-12 11:53:06 turbo Exp $
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
	label = createLabel("Max bruttovikt, TÅG (ton)", (32+(3*20)));
	field = new ListBox(	0, 20, label->getWidth()-PADDING*2, label->getHeight(),
							label, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false);
	createTextField("BK1", previous->result_weight[BK1], field);
	createTextField("BK2", previous->result_weight[BK2], field);
	createTextField("BK3", previous->result_weight[BK3], field);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Max Last, TÅG (ton)", (32+(3*20)));
	field = new ListBox(	0, 20, label->getWidth()-PADDING*2, label->getHeight(),
							label, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false);
	createTextField("BK1", previous->result_load[TRAIN][BK1], field);
	createTextField("BK2", previous->result_load[TRAIN][BK2], field);
	createTextField("BK3", previous->result_load[TRAIN][BK3], field);
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
#ifdef DEBUG1
	lprintfln("Index: %d", listBox->getSelectedIndex());
#endif

	switch(keyCode) {
		case MAK_HASH:
			// Hash (#) key - ask the moblet to close the application
			maExit(0);
			break;

		case MAK_LEFT:
		case MAK_SOFTRIGHT:
#ifdef DEBUG1
			lprintfln("Showing previous screen...");
#endif
			ScreenTransition::makeTransition(this, previous, -1, 400);
			break;

		case MAK_UP:
#ifdef DEBUG1
			lprintfln("selectPreviousItem()");
#endif
			listBox->selectPreviousItem();
			break;

		case MAK_DOWN:
#ifdef DEBUG1
			lprintfln("selectNextItem()");
#endif
			listBox->selectNextItem();
			break;
	}
#ifdef DEBUG1
	lprintfln("keyPressEvent() done...");
#endif
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

#ifdef DEBUG1
	lprintfln("Value: '%s' (%f)", str, value);
#endif

	label = new Label(0, 0, scrWidth-PADDING*2, 20, parent, string, 0, gFont);
}
