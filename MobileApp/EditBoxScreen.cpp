/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include <conprint.h> /* lprintfln() */

#include "EditBoxScreen.h"
#include "Util.h"
#include <MAUI/EditBox.h>

EditBoxScreen::EditBoxScreen(Screen *previous) : previous(previous) {
	mainLayout = createMainLayout("", "back");
	listBox = (ListBox*) mainLayout->getChildren()[0];

	Label *label;
	EditBox *editBox;

	label = createLabel("single line", 64);
	editBox = new EditBox(0, 24, label->getWidth()-PADDING*2, 64-24-PADDING*2, label, "", 0, gFont, true, false);
	editBox->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	label = createLabel("multi line input", 128);
	editBox = new EditBox(0, 24, label->getWidth()-PADDING*2, 128-24-PADDING*2, label, "", 0, gFont, true, true, 512);
	editBox->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	label = createLabel("number input", 64);
	editBox = new EditBox(0, 24, label->getWidth()-PADDING*2, 64-24-PADDING*2, label, "", 0, gFont, true, false, 64, EditBox::IM_NUMBERS);
	editBox->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	// BUG/Counter-intuitive!!
	// createMainLayout creates a listbox with wrapping=true.
	// This don't work!! Works if I change to 'false' though!!
	listBox->setWrapping(false);

	this->setMain(mainLayout);
}

EditBoxScreen::~EditBoxScreen() {
}

void EditBoxScreen::selectionChanged(Widget *widget, bool selected) {
	if(selected) {
		widget->getChildren()[0]->setSelected(true);
	} else {
		widget->getChildren()[0]->setSelected(false);
	}
}

void EditBoxScreen::show() {
	listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(true);
	Screen::show();
}

void EditBoxScreen::hide() {
	listBox->getChildren()[listBox->getSelectedIndex()]->setSelected(false);
	Screen::hide();
}

void EditBoxScreen::keyPressEvent(int keyCode, int nativeCode) {
	switch(keyCode) {
		case MAK_HASH:
			// Hash (#) key - ask the moblet to close the application
			maExit(0);
			break;

		case MAK_SOFTRIGHT:
		case MAK_LEFT:
			lprintfln("Showing previous screen...");
			previous->show();
			break;

		case MAK_UP:
			listBox->selectPreviousItem();
			break;

		case MAK_DOWN:
			listBox->selectNextItem();
			break;
	}
}
