/*
 * screen.h
 *
 * $Id: screen.h,v 1.2 2010-03-05 20:44:05 turbo Exp $
 * $Revision: 1.2 $
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

using namespace MAUI;
using namespace MAUtil;

class MyScreen : public Screen {
	public:
		MyScreen();
		~MyScreen();
		void keyPressEvent(int keyCode);

		void MyScreen::getTruckWeight();

		Vector<Screen*> screens;

	private:
		Layout* layout;
		ListBox* listBox;
};

#endif /* SCREEN_H_ */
