/* Copyright (C) 2010 Turbo Fredriksson <turbo@bayour.com>
 *
 * $Id: SaveScreen.h,v 1.1 2010-04-24 09:29:25 turbo Exp $
*/

#ifndef _SAVESCREEN_H_
#define _SAVESCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUI/ListBox.h>
#include <MAUI/Layout.h>

#include <conprint.h> /* lprintfln() */

#include "MAHeaders.h"
#include "ScreenTransition.h"
#include "TouchListBox.h"
#include "CheckBox.h"
#include "Util.h"

using namespace MAUI;

class SaveScreen : public Screen {
	public:
		SaveScreen(Screen *previous);
		~SaveScreen();

		void keyPressEvent(int keyCode, int nativeCode);
		void pointerPressEvent(MAPoint2d point);

	private:
		Screen *previous;

		Layout *mainLayout;
		TouchListBox *listBox;
		Vector<CheckBox*> checkBox;
};

#endif	//_SAVESCREEN_H_
