/*
 * LoadScreen.h
 *
 * $Id: LoadScreen.h,v 1.4 2010-04-18 19:35:59 turbo Exp $
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
		void drawText(int y, const char *string, int rgb);

	private:
		Screen *previous;

		Layout *mainLayout;
		ListBox *listBox;
};

#endif /* LOADSCREEN_H_ */
