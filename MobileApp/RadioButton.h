/*
 * RadioButton.h
 *
 * http://www.mosync.com/forum/viewtopic.php?f=19&t=507&p=1041#p1041
 * Created on: 2 Mar 2010
 *      Author: sjp
 */

#ifndef RADIOBUTTON_H_
#define RADIOBUTTON_H_

#include <MAUI/Label.h>
#include <MAUI/Image.h>

using namespace MAUI;

class RadioButton : public Widget
{
	public:
		RadioButton(int x, int y, int width, int height, Widget* parent = NULL, MAHandle selectedImage = 0, MAHandle unselectedImage = 0);
        ~RadioButton();

        void setResources(MAHandle selectedImage, MAHandle unselectedImage);

        void drawWidget();

        void setSelected(bool selected);
        void setCaption(const char *caption);

      private:
        Image* _button;
        Label* _label;
        bool _selected;
        MAHandle _sel;
        MAHandle _unsel;
};

#endif /* RADIOBUTTON_H_ */
