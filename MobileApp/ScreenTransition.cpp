/* Copyright (C) 2010 Oscar Lindberg

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License, version 2, as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.

   http://www.mosync.com/forum/viewtopic.php?f=19&t=234
 */

#import "ScreenTransition.h"
#import <MAUtil/Graphics.h>

MAUI::Screen* ScreenTransition::gScreen = 0;
ScreenTransition* ScreenTransition::gWidget = 0;

void ScreenTransition::makeTransition(MAUI::Screen* fromScreen, MAUI::Screen* toScreen, int direction, int duration)
{
	if (gScreen != 0)
	{
		return;
	}
	gScreen = new MAUI::Screen();
	gWidget = new ScreenTransition();
	gScreen->setMain(gWidget);
	gWidget->fromMainWidget = fromScreen->getMain();
	gWidget->toScreen = toScreen;
	gWidget->toMainWidget = toScreen->getMain();
	gWidget->direction = direction;
	gWidget->duration = duration;
	gWidget->start_time = maGetMilliSecondCount();
	gWidget->updateAlpha(gWidget->start_time);

	// This will tell the widget to fix stuff before being drawn
	gWidget->toMainWidget->update();

	gScreen->show();
	MAUtil::Environment::getEnvironment().addTimer(gWidget, 20, -1);
}

void ScreenTransition::drawWidget()
{
	int scrW = EXTENT_X(maGetScrSize());
	int scrH = EXTENT_Y(maGetScrSize());

	Gfx_translate((int)(-direction*alpha*scrW), 0);
	fromMainWidget->setEnabled(true);
	fromMainWidget->draw();
	fromMainWidget->setEnabled(false);

	Gfx_translate(direction*scrW, 0);
	toMainWidget->setEnabled(true);
	toMainWidget->draw();
	toMainWidget->setEnabled(false);
}

double ScreenTransition::square_sigmoid(double t)
{
	if (t < 0.0) {
		return 0.0;
	} else if (t < 0.5) {
		return 2*t*t;
	} else if (t < 1.0) {
		float tm1 = (t-1.0);
		return 1.0-2*tm1*tm1;
	} else {
		return 1.0;
	}
}

void ScreenTransition::updateAlpha(int now)
{
	alpha = double(now - this->start_time) / double(duration);
	alpha = square_sigmoid(alpha);
}

void ScreenTransition::runTimerEvent()
{
	updateAlpha(maGetMilliSecondCount());
	if (alpha >= 1.0)
	{
		MAUtil::Environment::getEnvironment().removeTimer(this);
		this->toScreen->show();
		delete gScreen;
		gScreen = 0;
		delete gWidget; // I.e. this
		gWidget = 0;
	}
	else
	{
		this->requestRepaint();
	}
}

