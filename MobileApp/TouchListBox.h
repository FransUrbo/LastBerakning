/* Copyright rival
 * http://www.mosync.com/forum/viewtopic.php?f=12&t=594&p=1214&hilit=touch+screen#p1214
 */

#ifndef _TOUCHLISTBOX_H_
#define _TOUCHLISTBOX_H_

#include <ma.h>
#include <maapi.h>

#include <MAUI/Widget.h>
#include <MAUI/ListBox.h>
#include <MAUtil/Environment.h>

using namespace MAUI;

class ITouchListBoxListener {
	public:
		virtual void touchListBoxItemSelected(int selectedIndex) = 0;
};

class TouchListBox : public ListBox, public PointerListener {
	public:   
		TouchListBox(int x, int y, int width, int height, Widget* parent = NULL);
		~TouchListBox();
		void pointerPressEvent(MAPoint2d point);
		void pointerMoveEvent(MAPoint2d point);
		void pointerReleaseEvent(MAPoint2d point);
		void setListener(ITouchListBoxListener* listener);
		void setEnabled(bool e);

	private:
		ITouchListBoxListener* l;
		void informListener();
		void locateItem(MAPoint2d point);
		bool enabled;
};

#endif //_TOUCHLISTBOX_H_
