/*
 * LoadScreen.h
 *
 * $Id: LoadScreen.h,v 1.1 2010-03-11 20:43:56 turbo Exp $
 */

#ifndef LOADSCREEN_H_
#define LOADSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/ListBox.h>

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
