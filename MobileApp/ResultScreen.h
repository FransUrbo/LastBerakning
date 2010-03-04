/*
 * ResultScreen.h
 *
 * $Id: ResultScreen.h,v 1.2 2010-03-04 19:05:38 turbo Exp $
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
		void ResultScreen::createTextField(const char *leader, float value, Widget *parent);
		void ResultScreen::doCalculations();

		Screen *previous;
		Layout* mainLayout;
		ListBox* listBox;

		float weight_bk1, weight_bk2, weight_bk3;
		float load_bk1, load_bk2, load_bk3;
};

#endif /* RESULTSCREEN_H_ */
