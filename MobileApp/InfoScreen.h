/* Copyright (C) 2010 Turbo Fredriksson <turbo@bayour.com>
 *
 * $Id: InfoScreen.h,v 1.1 2010-03-16 13:11:59 turbo Exp $
*/

#ifndef _INFOSCREEN_H_
#define _INFOSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUI/ListBox.h>

#include "LabelScreen.h"

using namespace MAUI;

class InfoScreen : public LabelScreen {
	public:
		InfoScreen(Screen *previous);

	private:
		Screen *previous;
		ListBox *listBox;
		Layout *mainLayout;
};

#endif	//_INFOSCREEN_H_
