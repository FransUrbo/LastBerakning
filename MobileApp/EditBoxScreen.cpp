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

/* Modified by Turbo Fredriksson <turbo@bayour.com>
 *
 * This screen is the main input data screen.
 * $Id: EditBoxScreen.cpp,v 1.9 2010-03-17 10:12:28 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */

#include "MAHeaders.h"
#include "Util.h"
#include "EditBoxScreen.h"
#include "RadioButton.h"
#include "RadioButtonGroup.h"
#include "ScreenTransition.h"
#include "screen.h"

EditBoxScreen::EditBoxScreen(Screen *previous) : previous(previous) {
	Label *label;

	/* Create the main work/text area */
	mainLayout = createMainLayout("Radera", "Tillbaka");
	listBox = (ListBox*) mainLayout->getChildren()[0];

	/* Create the different label/input fields */

	/* ---------------------------------- */
	label = createLabel("Tjänstevikt", 64);
	editBox.add(new EditBox(0, 24, label->getWidth()-PADDING*2, 64-24-PADDING*2,
							label, "", 0, gFont, true, false, 8, EditBox::IM_NUMBERS));
	editBox[0]->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Max belastning, framaxel", 64);
	editBox.add(new EditBox(0, 24, label->getWidth()-PADDING*2, 64-24-PADDING*2,
							label, "", 0, gFont, true, false, 8, EditBox::IM_NUMBERS));
	editBox[1]->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Axeltyp, framaxel", (32+(3*(20+PADDING))));
	listBox_select1 = new ListBox(	0, 32, label->getWidth()-PADDING*2, label->getHeight(),
									label, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false);
	group1 = new RadioButtonGroup();

	for(int i = 0; i < 3; i++) {
		select1.add(new RadioButton(PADDING, (PADDING / 2) + ((i+1)*20), label->getWidth()-PADDING*2,
									20, listBox_select1, RES_RADIOBUTTON_SELECTED, RES_RADIOBUTTON_UNSELECTED));
		group1->addRadioButton(select1[i]);
	}

	select1[0]->setCaption("Enkelaxel");		select1[0]->setSkin(false);
	select1[1]->setCaption("Boggieaxel");		select1[1]->setSkin(false);
	select1[2]->setCaption("Trippelaxel");		select1[2]->setSkin(false);

	group1->setSelectedButton(0);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Max belastning, bakaxel", 64);
	editBox.add(new EditBox(0, 24, label->getWidth()-PADDING*2, 64-24-PADDING*2,
							label, "", 0, gFont, true, false, 8, EditBox::IM_NUMBERS));
	editBox[2]->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Axeltyp, bakaxel", (32+(3*(20+PADDING))));
	listBox_select2 = new ListBox(	0, 32, label->getWidth()-PADDING*2, label->getHeight(),
									label, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false);
	group2 = new RadioButtonGroup();

	for(int i = 0; i < 3; i++) {
		select2.add(new RadioButton(PADDING, (PADDING / 2) + ((i+1)*20), label->getWidth()-PADDING*2,
									20, listBox_select2, RES_RADIOBUTTON_SELECTED, RES_RADIOBUTTON_UNSELECTED));
		group2->addRadioButton(select2[i]);
	}

	select2[0]->setCaption("Enkelaxel");		select2[0]->setSkin(false);
	select2[1]->setCaption("Boggieaxel");		select2[1]->setSkin(false);
	select2[2]->setCaption("Trippelaxel");		select2[2]->setSkin(false);

	group2->setSelectedButton(0);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Vägvänlig fjädring", 32);
	checkBox.add(new CheckBox(scrWidth - 50, 2, 16, 16, label));
	checkBox[0]->setResources(RES_CHECKBOX_UNCHECKED, RES_CHECKBOX_CHECKED);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Axelavstånd", 64);
	editBox.add(new EditBox(0, 24, label->getWidth()-PADDING*2, 64-24-PADDING*2,
							label, "", 0, gFont, true, false, 64, EditBox::IM_NUMBERS));
	editBox[3]->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Kopplingsavstånd", 64);
	editBox.add(new EditBox(0, 24, label->getWidth()-PADDING*2, 64-24-PADDING*2,
							label, "", 0, gFont, true, false, 8, EditBox::IM_NUMBERS));
	editBox[4]->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	/* ---------------------------------- */
	listBox->setWrapping(WRAPPING);

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
#ifdef DEBUG1
	lprintfln("Index: %d (%d / %d)", listBox->getSelectedIndex(), keyCode, nativeCode);
#endif

	switch(keyCode) {
		case MAK_HASH:
			// Hash (#) key - ask the moblet to close the application
			maExit(0);
			break;

		case MAK_LEFT:
			if((listBox->getSelectedIndex() == 2) || (listBox->getSelectedIndex() == 4))
				listBox->selectPreviousItem();
			break;

		case MAK_RIGHT:
		case MAK_FIRE:
			if((listBox->getSelectedIndex() == 2) || (listBox->getSelectedIndex() == 4))
				listBox->selectNextItem();
			else if (listBox->getSelectedIndex() == 5) {
#ifdef DEBUG1
				lprintfln("Flipping checkbox...");
#endif
				CheckBox * cb = (CheckBox *)((Label*)listBox->getChildren()[listBox->getSelectedIndex()])->getChildren()[0];
				cb->flip();
#ifdef DEBUG1
				lprintfln("flipped...");
#endif
			}
			break;

		case MAK_SOFTLEFT:
			// Remove index for Axle types and road nice checkbox...
			int index = listBox->getSelectedIndex();
			if(index >= 3) index -= 1;
			if(index >= 5) index -= 2;

			editBox[index]->deletePreviousCharacter();
			break;

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
			if((listBox->getSelectedIndex() == 2) && (listBox_select1->getSelectedIndex() > 0))
				listBox_select1->selectPreviousItem();
			else if((listBox->getSelectedIndex() == 4) && (listBox_select2->getSelectedIndex() > 0))
				listBox_select2->selectPreviousItem();
			else
				listBox->selectPreviousItem();
			break;

		case MAK_DOWN:
#ifdef DEBUG1
			lprintfln("selectNextItem()");
#endif
			if((listBox->getSelectedIndex() == 2) && (listBox_select1->getSelectedIndex() < 2))
				listBox_select1->selectNextItem();
			else if((listBox->getSelectedIndex() == 4) && (listBox_select2->getSelectedIndex() < 2))
				listBox_select2->selectNextItem();
			else
				listBox->selectNextItem();
			break;
	}
#ifdef DEBUG1
	lprintfln("keyPressEvent() done...");
#endif
}
