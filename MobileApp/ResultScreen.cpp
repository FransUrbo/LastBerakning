/*
 * ResultScreen.cpp
 *
 * Code to do the actuall calculations!
 *
 * $Id: ResultScreen.cpp,v 1.8 2010-03-12 14:23:26 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */
#include <MAUtil/String.h>
#include <MAUtil/util.h>

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

	label = createLabel("Väg:  Bil  +  Släp = Tåg", 32);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Max bruttovikt, TÅG (ton)", (32+(3*20)));
	field = new ListBox(	0, 20, label->getWidth()-PADDING*2, label->getHeight(),
							label, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false);
	createTextFields(previous->result_weight, field);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Max Last, TÅG (ton)", (32+(3*20)));
	field = new ListBox(	0, 20, label->getWidth()-PADDING*2, label->getHeight(),
							label, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false);
	createTextFields(previous->result_load, field);
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

void ResultScreen::createTextFields(double value[3][3], Widget *parent) {
	String prefix;
	char *valstr;

	for(int bk = 0; bk < 3; bk++) {
		prefix  = "BK";
		prefix += integerToString(bk+1);

		sprintf(valstr, "%02.02Lf + %02.02Lf = %02.02Lf",
				value[TRUCK][bk], value[TRAILER][bk], value[TRAIN][bk]);

		createTextField(prefix.c_str(), valstr, parent);
	}
}

void ResultScreen::createTextField(const char *leader, const char *value, Widget *parent)
{
	Label *label;
	String string;

	string += leader;
	string += ": ";
	string += value;

#ifdef DEBUG1
	lprintfln("Value: '%s'", string.c_str());
#endif

	label = new Label(0, 0, scrWidth-PADDING*2, 20, parent, string, 0, gFont);
}
