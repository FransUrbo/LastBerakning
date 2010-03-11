/*
 * LoadingScreen.cpp
 *
 * $Id: LoadScreen.cpp,v 1.1 2010-03-11 20:43:56 turbo Exp $
 */

#include <MAUtil/Moblet.h>

#include <conprint.h> /* lprintfln() */

#include "MAUIex.h"
#include "LoadScreen.h"
#include "Util.h"

LoadScreen::LoadScreen(void) {
	Label *label;

    MAExtent screenSize = maGetScrSize();
    int offset = EXTENT_Y(screenSize) / 2;

	/* Create the main work/text area */
	mainLayout = createMainLayout("", "", 0, 160);
	listBox = (ListBox*) mainLayout->getChildren()[0];

	/* Create the different label/input fields */

	/* ---------------------------------- */
	label = createLabel("Laddar. Var god vänta...", 32);
	listBox->add(label);

	listBox->setWrapping(WRAPPING);

	this->setMain(mainLayout);
}

LoadScreen::~LoadScreen(void) {
	delete mainLayout;
}

void LoadScreen::keyPressEvent(int keyCode, int nativeCode) { }
