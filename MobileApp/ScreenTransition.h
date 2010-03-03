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

#ifndef SCREENTRANSITION_H_
#define SCREENTRANSITION_H_

#include <MAUI/Widget.h>
#include <MAUI/Screen.h>
#include <MAUtil/Environment.h>

class ScreenTransition : public MAUI::Widget, public MAUtil::TimerListener
{
    public:
       ~ScreenTransition()
       {}
       static void makeTransition(MAUI::Screen* fromScreen, MAUI::Screen* toScreen, int direction, int duration);

    private:
       ScreenTransition()
       : Widget(0, 0, 0, 0, NULL)
       {}

    protected:
       static MAUI::Screen* gScreen;
       static ScreenTransition* gWidget;
       void drawWidget();

       double square_sigmoid(double t);
        void updateAlpha(int now);
       void runTimerEvent();

       MAUI::Screen* toScreen;
       MAUI::Widget* fromMainWidget;
       MAUI::Widget* toMainWidget;
       int duration;
       int direction;
       int start_time;
       double alpha; //0.0 = fromScreen, 1.0 = toScreen
};

#endif /* SCREENTRANSITION_H_ */

