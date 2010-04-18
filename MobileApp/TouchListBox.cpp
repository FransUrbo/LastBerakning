/* Copyright rival
 * http://www.mosync.com/forum/viewtopic.php?f=12&t=594&p=1214&hilit=touch+screen#p1214
 */

#include <conprint.h> /* lprintfln() */

#include "TouchListBox.h"

TouchListBox::TouchListBox(int x, int y, int width, int height, Widget* parent) : ListBox(x, y, width, height, parent, ListBox::LBO_VERTICAL, ListBox::LBA_LINEAR, true) {
	l = NULL;
	enabled = false;
}

TouchListBox::~TouchListBox() {
	if(enabled) {
		Environment& e = Environment::getEnvironment();
		e.removePointerListener(this);
	}
}

void TouchListBox::pointerPressEvent(MAPoint2d point) {
	locateItem(point);
}

void TouchListBox::pointerMoveEvent(MAPoint2d point) {
	locateItem(point);
}

void TouchListBox::pointerReleaseEvent(MAPoint2d point) {
	informListener();
}

void TouchListBox::locateItem(MAPoint2d point) {
	Point p;
	p.set(point.x, point.y);

	for(int i = 0; i < this->getChildren().size(); i++) {
		if(this->getChildren()[i]->contains(p)) {
			this->setSelectedIndex(i);
			break;
		}
	}
}

void TouchListBox::setListener(ITouchListBoxListener *listener) {
	l = listener;
}

void TouchListBox::informListener() {
	if(l != NULL)
		l->touchListBoxItemSelected(this->getSelectedIndex());
}

void TouchListBox::setEnabled(bool e) {
	Environment& env = Environment::getEnvironment();

	if(!enabled && e) {
		env.addPointerListener(this);
	} else if(enabled && !e) {
		env.removePointerListener(this);
	}

	enabled = e;
}
