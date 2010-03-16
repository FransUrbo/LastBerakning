/* Copyright (C) 2010 Turbo Fredriksson <turbo@bayour.com>
 *
 * $Id: LabelScreen.h,v 1.4 2010-03-16 13:12:00 turbo Exp $
*/

#ifndef _LABELSCREEN_H_
#define _LABELSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUI/ListBox.h>

using namespace MAUI;

class LabelScreen : public Screen {
	public:
		LabelScreen(Screen *previous);
		~LabelScreen();
		void keyPressEvent(int keyCode, int nativeCode);
		void createTextField(ListBox *&listBox, const char *str);

	private:
		Screen *previous;
		ListBox *listBox;
		Layout *mainLayout;
};

#endif	//_LABELSCREEN_H_
