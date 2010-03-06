/*
 * screen.h
 *
 * $Id: screen.h,v 1.3 2010-03-06 00:17:43 turbo Exp $
 * $Revision: 1.3 $
 *
 * Copyright Turbo Fredriksson <turbo@bayour.com>
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Layout.h>
#include <MAUI/Image.h>
#include <MAUI/ListBox.h>
#include <MAUI/Label.h>
#include <MAUI/Widget.h>

#include <conprint.h> /* lprintfln() */

#include "screen.h"
#include "EditBoxScreen.h"

using namespace MAUI;
using namespace MAUtil;

class MyScreen : public Screen {
	public:
		MyScreen();
		~MyScreen();
		void keyPressEvent(int keyCode);

		float weight_bk1, weight_bk2, weight_bk3;
		float load_bk1, load_bk2, load_bk3;

	private:
		void MyScreen::doCalculations();
		void MyScreen::getTruckWeight();

		Vector<EditBoxScreen*> editBoxScreens;
		Vector<Screen*> screens;

		Layout* layout;
		ListBox* listBox;
};

#endif /* SCREEN_H_ */
