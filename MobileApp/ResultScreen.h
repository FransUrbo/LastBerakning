/*
 * ResultScreen.h
 *
 * $Id: ResultScreen.h,v 1.5 2010-03-12 14:00:17 turbo Exp $
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
		/* Constructor */
		ResultScreen(MyScreen *previous);

		/* Destructor */
		~ResultScreen();

		/*  Recieves key presses and performs appropriate interaction */
		void keyPressEvent(int keyCode, int nativeCode);

		/* Overload of MAUI::Screen::show(). */
		void show();

	private:
		void hide();
		void ResultScreen::createTextFields(double value[3][3], Widget *parent);
		void ResultScreen::createTextField(const char *leader, const char *value, Widget *parent);

		Screen *previous;
		Layout* mainLayout;
		ListBox* listBox;
};

#endif /* RESULTSCREEN_H_ */
