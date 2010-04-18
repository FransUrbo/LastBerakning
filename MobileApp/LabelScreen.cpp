/* Copyright (C) 2010 Turbo Fredriksson <turbo@bayour.com>
 *
 * This screen is the help text screen.
 * $Id: LabelScreen.cpp,v 1.9 2010-04-18 19:42:04 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */

#include "LabelScreen.h"
#include "Util.h"
#include "ScreenTransition.h"

LabelScreen::LabelScreen(Screen *previous) : previous(previous) {
	mainLayout = createMainLayout("", "Tillbaka");
	listBox = (ListBox*) mainLayout->getChildren()[FIRSTCHILD];

	createTextField(listBox, "Vikter skrivs direkt i kilogram!");

	createTextField
	(	listBox,
		"Programmet stöder:\n"
		"Enkelaxlade fordon (2 axlar - en fram "
		"och en bak),\n"
		"Boggieaxlade fordon (3 axlar - en fram "
		"och en bak) samt\n"
		"Trippelaxlade fordon (4 axlar - en fram "
		"och tre bak)."
	);

	createTextField
	(	listBox,
		"Programmet stödjer också en kombination av detta (enkel axlat, "
		"boggie eller trippelaxel fram och ditto bak) etc i alla "
		"möjliga variationer. Som max sex axlar (trippelaxel fram "
		"och trippelaxel bak)."
	);

	createTextField(listBox,
					"Axelavstånd skrivs i millimeter (från registreringsbeviset).");

	createTextField
	(	listBox,
		"Enkelaxel\n"
		"Avståndet mellan framaxel och bakaxel.\n"
		"Exempel: 5900"
	);

	createTextField
	(	listBox,
		"Boggieaxel\n"
		"Avståndet mellan framaxel och första bakaxeln + "
		"avståndet mellan första och sista bakaxeln.\n"
		"Exempel: 4900+1350"
	);

	createTextField
	(	listBox,
		"Trippelaxel\n"
		"Avståndet mellan framaxel och första bakaxeln "
		"+ avståndet mellan första och andra bakaxeln + "
		"avståndet mellan andra och sista (tredje) bakaxeln.\n"
		"Exempel: 3750+1360+1310"
	);

	createTextField
	(	listBox,
		"Trippel fram och Trippel bak\n"
		"Exempel: 1530+1530+4185+1360+1310"
	);

	createTextField
	(	listBox,
		"Programmet räknar automatiskt ut dom avstånd det "
		"behöver från dessa värden..."
	);

	createTextField
	(	listBox,
		"Kopplingsavståndet är avståndet från (för lastbilen) "
		"första axeln och kopplingspunkten. För släpfordon är "
		"det avståndet från kopplingspunkten och sista axeln."
	);

	createTextField
	(	listBox,
		"Vägvänlig fjädring är en axel som har tvillingmontage och "
		"luftfjädring."
	);

	createTextField
	(	listBox,
		"LastBeräkning tar hänsyn till 'Allmänna begränsningar för "
		"fordon' (maximal fordonsvikt beroende på antal axlar och "
		"deras inbördes avstånd)."
	);

	createTextField
	(	listBox,
		"Detta program förstår också maximala avståndet för "
		"att en 'axel' ska räknas som boggie (max 2 meter) "
		"eller trippel (max 5 meter mellan första och sista "
		"axeln)."
	);

	createTextField
	(	listBox,
		"OBSERVERA!\n"
		"När du valt Axeltyp, så måste du gå åt HÖGER (för att gå neråt) "
		"eller VÄNSTER (för att gå uppåt) för att få valet att stanna kvar. "
		"Går du upp eller ner, så ändras valet istället..\n"
		"Detta är lite omständigt, men det finns inte mycket annat sätt "
		"att göra det på."
	);

	listBox->setWrapping(WRAPPING);

	this->setMain(mainLayout);
}

LabelScreen::~LabelScreen() {
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
			ScreenTransition::makeTransition(this, previous, -1, 400);
			break;

		case MAK_UP:
			listBox->selectPreviousItem();
			break;

		case MAK_DOWN:
			listBox->selectNextItem();
			break;
	}
}

void LabelScreen::createTextField(ListBox *&listBox, const char *str)
{
	Label *label;

	label = createLabel( str, 32 );
	label->setAutoSizeY();
	label->setSkin(false);
	label->setMultiLine(true);

	listBox->add(label);
}
