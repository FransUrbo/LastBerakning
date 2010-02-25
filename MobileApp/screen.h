/*
 * screen.h
 *
 * $Id: screen.h,v 1.1 2010-02-25 13:00:05 turbo Exp $
 * $Revision: 1.1 $
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

	private:
		Vector<Screen*> screens;
		ListBox* listBox;
		Layout* layout;
};

#endif /* SCREEN_H_ */
