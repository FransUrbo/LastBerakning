/* Copyright (C) 2010 Turbo Fredriksson <turbo@bayour.com>
 *
 * This screen is the program info screen.
 * $Id: InfoScreen.cpp,v 1.2 2010-04-18 10:24:14 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */

#include "InfoScreen.h"
#include "Util.h"
#include "ScreenTransition.h"
#include "Version.h"

InfoScreen::InfoScreen(Screen *previous) : LabelScreen(previous) {
	mainLayout = createMainLayout("", "Tillbaka");
	listBox = (ListBox*) mainLayout->getChildren()[FIRSTCHILD];

	createTextField(listBox, "LastBeräkning av Turbo Fredriksson!");

	createTextField(listBox, "Copyright Turbo Fredriksson <turbo@bayour.com>");

	String str("Version: ");
	str.append(VERSION, strlen(VERSION));
	createTextField(listBox, (const char *)str.c_str());

	createTextField(listBox, "Se http://last.bayour.com/ för online version.");

	listBox->setWrapping(WRAPPING);

	this->setMain(mainLayout);
}
