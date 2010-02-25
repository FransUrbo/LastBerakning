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
 * This screen is the help text screen.
 * $Id: LabelScreen.cpp,v 1.2 2010-02-25 16:41:34 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */

#include "MAUIex.h"
#include "LabelScreen.h"
#include "Util.h"

LabelScreen::LabelScreen(Screen *previous) : previous(previous) {
	mainLayout = createMainLayout("", "Tillbaka");
	listBox = (ListBox*) mainLayout->getChildren()[0];
	Label *label;

	label = createLabel
	(
		"Vikter skrivs direkt i ton, inte kilogram!",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel
	(
		"Programmet stöder:\n"
		"Enkelaxlade fordon (2 axlar - en fram "
		"och en bak),\n"
		"Boggieaxlade fordon (3 axlar - en fram "
		"och en bak) samt\n"
		"Trippelaxlade fordon (4 axlar - en fram "
		"och tre bak).",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel
	(
		"Programmet stödjer också en kombination av detta (enkel axlat, "
		"boggie eller trippelaxel fram och ditto bak) etc i alla "
		"möjliga variationer. Som max sex axlar (trippelaxel fram "
		"och trippelaxel bak).",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel
	(
		"Axelavstånd skrivs i millimeter (från registreringsbeviset).",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel
	(
		"Enkelaxel\n"
		"Avståndet mellan framaxel och bakaxel.\n"
		"Exempel: 5900",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel
	(
		"Boggieaxel\n"
		"Avståndet mellan framaxel och första bakaxeln + "
		"avståndet mellan första och sista bakaxeln.\n"
		"Exempel: 4900+1350",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel
	(
		"Trippelaxel\n"
		"Avståndet mellan framaxel och första bakaxeln "
		"+ avståndet mellan första och andra bakaxeln + "
		"avståndet mellan andra och sista (tredje) bakaxeln.\n"
		"Exempel: 3750+1360+1310",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel
	(
		"Trippel fram och Trippel bak\n"
		"Exempel: 1530+1530+4185+1360+1310",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel
	(
		"Programmet räknar automatiskt ut dom avstånd det "
		"behöver från dessa värden...",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel
	(
		"Kopplingsavståndet är avståndet från (för lastbilen) "
		"första axeln och kopplingspunkten. För släpfordon är "
		"det avståndet från kopplingspunkten och sista axeln.",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel
	(
		"Vägvänlig fjädring är en axel som har tvillingmontage och "
		"luftfjädring.",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	label = createLabel
	(
		"LastBeräkning tar hänsyn till Allmänna begränsningar för "
		"fordon samt max boggie/trippelaxel avstånd (två respektive "
		"fem meter).",
		32
	);
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);

	// BUG/Counter-intuitive!!
	// createMainLayout creates a listbox with wrapping=true.
	// This don't work!! Works if I change to 'false' though!!
	listBox->setWrapping(false);

	this->setMain(mainLayout);
}

LabelScreen::~LabelScreen() {
	delete mainLayout;
}

void LabelScreen::keyPressEvent(int keyCode, int nativeCode) {
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
