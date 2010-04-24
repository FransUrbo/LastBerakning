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
 * $Id: EditBoxScreen.cpp,v 1.13 2010-04-24 09:23:10 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */

#include "MAHeaders.h"
#include "Util.h"
#include "EditBoxScreen.h"
#include "screen.h"

EditBoxScreen::EditBoxScreen(MainScreen *previous) : previous(previous) {
	Label *label;
	int i, radio_buttons = 3;

	/* Create the main work/text area */
	mainLayout = createMainLayout("Radera", "Tillbaka");
	listBox = (ListBox*) mainLayout->getChildren()[FIRSTCHILD];

	/* Create the different label/input fields */

	/* ---------------------------------- */
	label = createLabel("Tjänstevikt", (FONTHEIGHT*2)-(PADDING*2));
	editBox.add(new EditBox(0, FONTHEIGHT-(PADDING*2), label->getWidth()-PADDING*2, (FONTHEIGHT*2)-FONTHEIGHT-(PADDING*2),
							label, "", 0, gFont, true, false, 8, EditBox::IM_NUMBERS));
	editBox[0]->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Max belastning, framaxel", (FONTHEIGHT*3)-(PADDING*3));
	editBox.add(new EditBox(0, (FONTHEIGHT*2)-(PADDING*4), label->getWidth()-PADDING*2, (FONTHEIGHT*2)-FONTHEIGHT-(PADDING*2),
							label, "", 0, gFont, true, false, 8, EditBox::IM_NUMBERS));
	editBox[1]->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	/* ---------------------------------- */
	if(previous->trailer_screen)
		radio_buttons = 4;

	label = createLabel("Axeltyp, framaxel", (FONTHEIGHT+(radio_buttons*(RADIOHEIGHT+PADDING))));
	listBox_select1 = new TouchListBox(	0, FONTHEIGHT, label->getWidth()-PADDING*2, label->getHeight(), label);
	listBox_select1->setEnabled(true);
	group1 = new RadioButtonGroup();

	for(i = 0; i < radio_buttons; i++) {
		select1.add(new RadioButton(PADDING, (PADDING / 2) + ((i+1)*RADIOHEIGHT), label->getWidth()-PADDING*2,
									RADIOHEIGHT, listBox_select1, RES_RADIOBUTTON_SELECTED, RES_RADIOBUTTON_UNSELECTED));
		group1->addRadioButton(select1[i]);
	}

	i = 0;
	if(previous->trailer_screen) {
		select1[i]->setCaption("Trailer");		select1[i]->setSkin(false);	i++;
	}
	select1[i]->setCaption("Enkelaxel");		select1[i]->setSkin(false);	i++;
	select1[i]->setCaption("Boggieaxel");		select1[i]->setSkin(false);	i++;
	select1[i]->setCaption("Trippelaxel");		select1[i]->setSkin(false);	i++;

	if(previous->trailer_screen)
		group1->setSelectedButton(1);
	else
		group1->setSelectedButton(0);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Max belastning, bakaxel", (FONTHEIGHT*3)-(PADDING*3));
	editBox.add(new EditBox(0, (FONTHEIGHT*2)-(PADDING*4), label->getWidth()-PADDING*2, (FONTHEIGHT*2)-FONTHEIGHT-(PADDING*2),
							label, "", 0, gFont, true, false, 8, EditBox::IM_NUMBERS));
	editBox[2]->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Axeltyp, bakaxel", (FONTHEIGHT+(3*(RADIOHEIGHT+PADDING))));
	listBox_select2 = new TouchListBox(	0, FONTHEIGHT, label->getWidth()-PADDING*2, label->getHeight(), label);
	listBox_select2->setEnabled(true);
	group2 = new RadioButtonGroup();

	for(i = 0; i < 3; i++) {
		select2.add(new RadioButton(PADDING, (PADDING / 2) + ((i+1)*RADIOHEIGHT), label->getWidth()-PADDING*2,
									RADIOHEIGHT, listBox_select2, RES_RADIOBUTTON_SELECTED, RES_RADIOBUTTON_UNSELECTED));
		group2->addRadioButton(select2[i]);
	}

	select2[0]->setCaption("Enkelaxel");		select2[0]->setSkin(false);
	select2[1]->setCaption("Boggieaxel");		select2[1]->setSkin(false);
	select2[2]->setCaption("Trippelaxel");		select2[2]->setSkin(false);

	group2->setSelectedButton(0);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Vägvänlig fjädring", FONTHEIGHT);
	checkBox.add(new CheckBox(scrWidth - 50, 2, 16, 16, label));
	checkBox[0]->setResources(RES_CHECKBOX_UNCHECKED, RES_CHECKBOX_CHECKED);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Axelavstånd", (FONTHEIGHT*2)-(PADDING*2));
	editBox.add(new EditBox(0, FONTHEIGHT-(PADDING*2), label->getWidth()-PADDING*2, (FONTHEIGHT*2)-FONTHEIGHT-(PADDING*2),
							label, "", 0, gFont, true, false, (FONTHEIGHT*2), EditBox::IM_NUMBERS));
	editBox[3]->setDrawBackground(false);
	label->addWidgetListener(this);
	listBox->add(label);

	/* ---------------------------------- */
	label = createLabel("Kopplingsavstånd", (FONTHEIGHT*2)-(PADDING*2));
	editBox.add(new EditBox(0, FONTHEIGHT-(PADDING*2), label->getWidth()-PADDING*2, (FONTHEIGHT*2)-FONTHEIGHT-(PADDING*2),
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

void EditBoxScreen::keyPressEvent(int keyCode, int nativeCode) {
#if DEBUG >= 2
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
#if DEBUG >= 2
				lprintfln("Flipping checkbox...");
#endif
				CheckBox * cb = (CheckBox *)((Label*)listBox->getChildren()[listBox->getSelectedIndex()])->getChildren()[0];
				cb->flip();
#if DEBUG >= 2
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
#if DEBUG >= 2
			lprintfln("Showing previous screen...");
#endif
			ScreenTransition::makeTransition(this, previous, -1, 400);
			break;

		case MAK_UP:
#if DEBUG >= 2
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
#if DEBUG >= 2
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
#if DEBUG >= 2
	lprintfln("keyPressEvent() done...");
#endif
}

void EditBoxScreen::pointerPressEvent(MAPoint2d point) {
	Point p;
	p.set(point.x, point.y);

	for(int i = 0; i < listBox->getChildren().size(); i++) {
		if(listBox->getChildren()[i]->contains(p)) {
			listBox->setSelectedIndex(i);

			if (listBox->getSelectedIndex() == 5) {
#if DEBUG >= 2
				lprintfln("Flipping checkbox (touch)...");
#endif
				CheckBox * cb = (CheckBox *)((Label*)listBox->getChildren()[listBox->getSelectedIndex()])->getChildren()[0];
				cb->flip();
#if DEBUG >= 2
				lprintfln("flipped (touch)...");
#endif
			}

			break;
		}
	}
}
