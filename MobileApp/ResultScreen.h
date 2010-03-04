/*
 * ResultScreen.h
 *
 * $Id: ResultScreen.h,v 1.1 2010-03-04 18:28:41 turbo Exp $
 */

#ifndef RESULTSCREEN_H_
#define RESULTSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/EditBox.h>
#include <MAUI/ListBox.h>
#include <MAUI/Layout.h>

using namespace MAUI;

class ResultScreen : public Screen, WidgetListener {
	public:
		/* Constructor */
		ResultScreen(Screen *previous);

		/* Destructor */
		~ResultScreen();

		/*  Recieves key presses and performs appropriate interaction */
		void keyPressEvent(int keyCode, int nativeCode);

		/* Overload of MAUI::Screen::show(). */
		void show();

	private:
		void hide();
		void ResultScreen::createTextField(const char *leader, const char *value, Widget *parent);

		Screen *previous;

		Layout* mainLayout;

		ListBox* listBox;
};

#endif /* RESULTSCREEN_H_ */
