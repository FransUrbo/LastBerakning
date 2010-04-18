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

#include "Util.h"
#include "screen.h"

Font *gFont;
WidgetSkin *gSkin;
int scrWidth;
int scrHeight;

void setLabelPadding(Widget *w) {
	w->setPaddingLeft(PADDING);
	w->setPaddingBottom(PADDING);
	w->setPaddingRight(PADDING);
	w->setPaddingTop(PADDING);
}

Label* createLabel(const char *str, int height) {
	Label *label;

	label = new Label(0, 0, scrWidth-PADDING*2, height, NULL, str, 0, gFont);
	label->setSkin(gSkin);
	label->setMultiLine(true);
	setLabelPadding(label);

	return label;
}

Widget* createSoftKeyBar(int height, const char *left, const char *right) {
	Layout *mainLayout;
	Label *label;
	TouchListBox *listBoxL, *listBoxR;
#ifdef DEBUG2
	lprintfln("Screen Width=%d, Height=%d, Left=%s, Right=%s", scrWidth, height, left, right);
#endif

	/* ---------------------------------- */
	mainLayout = new Layout(0, 0, scrWidth, height, NULL, 2, 1);

	/* ---------------------------------- */
	label = new Label(PADDING, 0, (scrWidth/2)-10, height, NULL, left, 0, gFont);
	label->setSkin(false);
	label->setHorizontalAlignment(Label::HA_LEFT);
	setLabelPadding(label);
	mainLayout->add(label);

	/* ---------------------------------- */
	label = new Label(0, 0, scrWidth/2, height, NULL, right, 0, gFont);
	label->setSkin(false);
	label->setHorizontalAlignment(Label::HA_RIGHT);
	setLabelPadding(label);
	mainLayout->add(label);

	return mainLayout;
}

// first and second child is the SoftKeys
// third child is listbox
Layout* createMainLayout(const char *left, const char *right, int offset_x, int offset_y) {
	Layout *mainLayout = new Layout(offset_x, offset_y, scrWidth-offset_x, scrHeight-offset_y, NULL, 1, 2);

	Widget *softKeys = createSoftKeyBar(FONTHEIGHT+5, left, right);

	TouchListBox* listBox = new TouchListBox(0, 0, scrWidth, scrHeight-(softKeys->getHeight()), mainLayout);
	listBox->setSkin(gSkin);
	listBox->setPaddingLeft(5);
	listBox->setPaddingRight(5);
	listBox->setPaddingTop(15);
	listBox->setPaddingBottom(15);

	mainLayout->add(softKeys);

	return mainLayout;
}
