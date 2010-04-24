/* Copyright (C) 2010 Turbo Fredriksson <turbo@bayour.com>
 *
 * This screen is the program info screen.
 * $Id: InfoScreen.cpp,v 1.3 2010-04-24 11:54:57 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */

#include "InfoScreen.h"
#include "ScreenTransition.h"
#include "Version.h"
#include "Language.h"
#include "Util.h"

InfoScreen::InfoScreen(Screen *previous) : LabelScreen(previous) {
	mainLayout = createMainLayout("", LANG_BACK);
	listBox = (ListBox*) mainLayout->getChildren()[FIRSTCHILD];

	createTextField(listBox, LANG_MADE_BY);

	createTextField(listBox, LANG_COPYRIGHT);

	String str(LANG_VERSION);
	str.append(VERSION, strlen(VERSION));
	createTextField(listBox, (const char *)str.c_str());

	createTextField(listBox, LANG_SEE_ALSO);

	listBox->setWrapping(WRAPPING);

	this->setMain(mainLayout);
}
