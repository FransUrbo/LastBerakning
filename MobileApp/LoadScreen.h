/*
 * LoadScreen.h
 *
 * $Id: LoadScreen.h,v 1.2 2010-03-13 23:32:13 turbo Exp $
 */

#ifndef LOADSCREEN_H_
#define LOADSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/ListBox.h>
#include <MAUI/Screen.h>
#include <MAUI/Label.h>
#include <MAUI/ListBox.h>

using namespace MAUI;

class LoadScreen : public Screen {
	public:
		LoadScreen();
		~LoadScreen();
		void keyPressEvent(int keyCode, int nativeCode);

	private:
		Screen *previous;
		ListBox *listBox;
		Layout *mainLayout;
};

#endif /* LOADSCREEN_H_ */
