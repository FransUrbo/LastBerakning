/*
 * ResultScreen.cpp
 *
 * Code to do the actuall calculations!
 *
 * $Id: ResultScreen.cpp,v 1.14 2010-04-24 11:54:57 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */
#include <MAUtil/String.h>
#include <MAUtil/util.h>

#include "screen.h"
#include "EditBoxScreen.h"
#include "ResultScreen.h"
#include "ScreenTransition.h"
#include "Language.h"
#include "Util.h"

ResultScreen::ResultScreen(MainScreen *previous) : previous(previous) {
	Label *label;
	ListBox *field;

	/* Create the main work/text area */
	mainLayout = createMainLayout(LANG_ERASE, LANG_BACK);
	listBox = (ListBox*) mainLayout->getChildren()[FIRSTCHILD];

	label = createLabel(LANG_RESULT_HEADER, (FONTHEIGHT*2));
	listBox->add(label);
	listBox->setEnabled(false);

	/* ---------------------------------- */
	label = createLabel(LANG_RESULT_MAXGROSS, (FONTHEIGHT*7)-(PADDING*8));
	field = new ListBox(	0, (FONTHEIGHT*2)-(PADDING*4), label->getWidth()-PADDING*2, label->getHeight(),
							label, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false);
	createTextFields(previous->result_weight, field);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel(LANG_RESULT_MAXLOAD, (FONTHEIGHT*7)-(PADDING*9));
	field = new ListBox(	0, FONTHEIGHT-PADDING, label->getWidth()-PADDING*2, label->getHeight(),
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
#if DEBUG >= 2
	lprintfln("Index: %d", listBox->getSelectedIndex());
#endif

	switch(keyCode) {
		case MAK_HASH:
			// Hash (#) key - ask the moblet to close the application
			maExit(0);
			break;

		case MAK_LEFT:
		case MAK_SOFTRIGHT:
#if DEBUG >= 2
			lprintfln("Showing previous screen...");
#endif
			ScreenTransition::makeTransition(this, previous, -1, 400);
			break;

		case MAK_UP:
#if DEBUG >= 2
			lprintfln("selectPreviousItem()");
#endif
			listBox->selectPreviousItem();
			break;

		case MAK_DOWN:
#if DEBUG >= 1
			lprintfln("selectNextItem()");
#endif
			listBox->selectNextItem();
			break;
	}
#if DEBUG >= 2
	lprintfln("keyPressEvent() done...");
#endif
}

void ResultScreen::createTextFields(double value[3][3], Widget *parent) {
	String prefix;
	char valstr[64];

	for(int bk = 0; bk < 3; bk++) {
		prefix  = LANG_LOAD_CLASS;
		prefix += integerToString(bk+1);

#if DEBUG >= 0
		lprintfln("%s%d => %02.02Lf + %02.02Lf = %02.02Lf",
				  prefix.c_str(), bk+1, value[TRUCK][bk], value[TRAILER][bk], value[TRAIN][bk]);
#endif

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

#if DEBUG >= 1
	lprintfln("createTextField(): string='%s'", string.c_str());
#endif

	label = new Label(0, 0, scrWidth-PADDING*2, RADIOHEIGHT*2, parent, string, 0, gFont);
	label->setMultiLine(true);
	label->setSkin(false);
}
