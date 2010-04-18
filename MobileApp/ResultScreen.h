/*
 * ResultScreen.h
 *
 * $Id: ResultScreen.h,v 1.7 2010-04-18 19:38:29 turbo Exp $
 */

#ifndef RESULTSCREEN_H_
#define RESULTSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/EditBox.h>
#include <MAUI/ListBox.h>
#include <MAUI/Layout.h>

#include "screen.h"

using namespace MAUI;

class ResultScreen : public Screen, WidgetListener {
	public:
		ResultScreen(MainScreen *previous);
		~ResultScreen();
		void keyPressEvent(int keyCode, int nativeCode);

		void show();

	private:
		void hide();
		void createTextFields(double value[3][3], Widget *parent);
		void createTextField(const char *leader, const char *value, Widget *parent);

		Screen *previous;

		Layout* mainLayout;
		ListBox* listBox;
};

#endif /* RESULTSCREEN_H_ */
