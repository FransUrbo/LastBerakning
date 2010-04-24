/*
 * screen.h
 *
 * $Id: screen.h,v 1.14 2010-04-24 09:28:56 turbo Exp $
 * $Revision: 1.14 $
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
#include <MAUtil/String.h>

#include <conprint.h> /* lprintfln() */

#include "EditBoxScreen.h"
#include "TouchListBox.h"

#define AXLE_SINGLE		0
#define AXLE_BOGGIE		1
#define AXLE_TRIPPLE	2

#define FRONT			0
#define BACK			1

#define TRUCK			0
#define TRAILER			1
#define TRAIN			2

#define BK1				0
#define BK2				1
#define BK3				2

// NOTE: Be carefull to set to >= 2. It will EAT cycles!!
#define DEBUG			1

using namespace MAUI;
using namespace MAUtil;

class MainScreen : public Screen {
	public:
		MainScreen();
		~MainScreen();
		void keyPressEvent(int keyCode);
		void pointerPressEvent(MAPoint2d point);

		double result_weight[3][3], result_load[3][3];
		int main_screen_loaded;
		bool trailer_screen;

	private:
		void doCalculations();
		void fetchValues();
		void checkAxleTypeTruck();
		void checkAxleTypeTrailer();
		void checkAxleType(Vector<double> &axle_dists, int &axle_type, bool front);
		Vector<String> openTable(const char *name);
		double parseTable(Vector<String> data, double dist, int road);
		Vector<double> split(const char *needles, char *heystack);
		int getIntValue(EditBox *e);
		char *getCharValue(EditBox *e);

		Vector<EditBoxScreen*> editBoxScreens;
		Vector<Screen*> screens;

		Layout* layout;
		TouchListBox* listBox;

		// FILE[0] : BK1
		// FILE[1] : BK2
		// FILE[2] : BK3
		// FILE[3] : BOGGIE
		// FILE[4] : TRIPPLE
		Vector<Vector<String> > TABLE_DATA;

		// INPUT variables
		int truck_weight, trailer_weight;
		int weicle_load[2][2];				// [TRUCK|TRAILER][FRONT|BACK] axle load
		int axle_type[2][2];				// [TRUCK|TRAILER][FRONT|BACK] axle type
		int weicle_link[2];					// [TRUCK|TRAILER] connection link distance
		bool road_nice[2];					// [TRUCK|TRAILER] axle is 'road nice'
		char *truck_axle, *trailer_axle;	// [TRUCK|TRAILER] connection

		Vector<double> weicle_dists[2];
};

#endif /* SCREEN_H_ */
