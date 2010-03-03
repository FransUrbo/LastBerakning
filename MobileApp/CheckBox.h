/*
 * http://www.mosync.com/forum/viewtopic.php?f=19&t=466
 * Post by bruno on Thu Feb 25, 2010 12:10 pm
 */

#ifndef CHECKBOX_H_
#define CHECKBOX_H_

#include <MAUI/Widget.h>
#include <MAUI/Label.h>
#include <MAUI/Image.h>

using namespace MAUI;

class CheckBox : public Widget {
	public:
		CheckBox(int x, int y, int height, int width, Widget* parent);
        ~CheckBox();

        void setResources(MAHandle unchecked, MAHandle checked);

        void drawWidget();
        void flip();
        bool isChecked();
        void setChecked (bool checked) {
           _checked = checked;
           updateCheckBox();
        }

	private:
        Label* text;
        Image* image;
        bool _checked;

        MAHandle resChecked;
        MAHandle resUnchecked;

        void updateCheckBox();
};

#endif /* CHECKBOX_H_ */
