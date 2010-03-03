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
 * $Id: LabelScreen.cpp,v 1.3 2010-03-03 13:47:34 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */

#include "MAUIex.h"
#include "LabelScreen.h"
#include "Util.h"
#include "ScreenTransition.h"

LabelScreen::LabelScreen(Screen *previous) : previous(previous) {
	mainLayout = createMainLayout("", "Tillbaka");
	listBox = (ListBox*) mainLayout->getChildren()[0];

	createTextField("Vikter skrivs direkt i kilogram!");

	createTextField
	(
		"Programmet st�der:\n"
		"Enkelaxlade fordon (2 axlar - en fram "
		"och en bak),\n"
		"Boggieaxlade fordon (3 axlar - en fram "
		"och en bak) samt\n"
		"Trippelaxlade fordon (4 axlar - en fram "
		"och tre bak)."
	);

	createTextField
	(
		"Programmet st�djer ocks� en kombination av detta (enkel axlat, "
		"boggie eller trippelaxel fram och ditto bak) etc i alla "
		"m�jliga variationer. Som max sex axlar (trippelaxel fram "
		"och trippelaxel bak)."
	);

	createTextField("Axelavst�nd skrivs i millimeter (fr�n registreringsbeviset).");

	createTextField
	(
		"Enkelaxel\n"
		"Avst�ndet mellan framaxel och bakaxel.\n"
		"Exempel: 5900"
	);

	createTextField
	(
		"Boggieaxel\n"
		"Avst�ndet mellan framaxel och f�rsta bakaxeln + "
		"avst�ndet mellan f�rsta och sista bakaxeln.\n"
		"Exempel: 4900+1350"
	);

	createTextField
	(
		"Trippelaxel\n"
		"Avst�ndet mellan framaxel och f�rsta bakaxeln "
		"+ avst�ndet mellan f�rsta och andra bakaxeln + "
		"avst�ndet mellan andra och sista (tredje) bakaxeln.\n"
		"Exempel: 3750+1360+1310"
	);

	createTextField
	(
		"Trippel fram och Trippel bak\n"
		"Exempel: 1530+1530+4185+1360+1310"
	);

	createTextField
	(
		"Programmet r�knar automatiskt ut dom avst�nd det "
		"beh�ver fr�n dessa v�rden..."
	);

	createTextField
	(
		"Kopplingsavst�ndet �r avst�ndet fr�n (f�r lastbilen) "
		"f�rsta axeln och kopplingspunkten. F�r sl�pfordon �r "
		"det avst�ndet fr�n kopplingspunkten och sista axeln."
	);

	createTextField
	(
		"V�gv�nlig fj�dring �r en axel som har tvillingmontage och "
		"luftfj�dring."
	);

	createTextField
	(
		"LastBer�kning tar h�nsyn till 'Allm�nna begr�nsningar f�r "
		"fordon' (maximal fordonsvikt beroende p� antal axlar och "
		"deras inb�rdes avst�nd)."
	);

	createTextField
	(
		"Detta program f�rst�r ocks� maximala avst�ndet f�r "
		"att en 'axel' ska r�knas som boggie (max 2 meter) "
		"eller trippel (max 5 meter mellan f�rsta och sista "
		"axeln)."
	);

	createTextField
	(
		"OBSERVERA!\n"
		"N�r du valt Axeltyp, s� m�ste du g� �t H�GER (f�r att g� ner�t) "
		"eller V�NSTER (f�r att g� upp�t) f�r att f� valet att stanna kvar. "
		"G�r du upp eller ner, s� �ndras valet ist�llet..\n"
		"Detta �r lite omst�ndigt, men det finns inte mycket annat s�tt "
		"att g�ra det p�."
	);

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

void LabelScreen::createTextField(const char *str)
{
	Label *label;

	label = createLabel( str, 32 );
	label->setAutoSizeY();
	label->setSkin(gSkin);
	label->setMultiLine(true);
	listBox->add(label);
}
