/*
 * RadioButton.cpp
 *
 * http://www.mosync.com/forum/viewtopic.php?f=19&t=507&p=1041#p1041
 * Created on: 2 Mar 2010
 *      Author: sjp
 */

#include <conprint.h> /* lprintfln() */

#include "RadioButton.h"
#include "Util.h"

RadioButton::RadioButton(int x, int y, int width, int height, Widget* parent, MAHandle selectedImage, MAHandle unselectedImage)
			: Widget(x, y, width, height, parent),
			_sel(selectedImage),
			_unsel(unselectedImage)
{
	_button = new Image(0, 0, width, height, this, true, true, unselectedImage);
	_button->setPaddingTop(2);

	_label  = new Label(_button->getWidth(), 0, width - _button->getWidth(), height, this);

	_label->setFont(gFont);
	_label->setDrawBackground(false);

	_label->setPaddingRight(PADDING);
	_label->setPaddingLeft(PADDING);
}

RadioButton::~RadioButton()
{}

void RadioButton::drawWidget()
{}

void RadioButton::setSelected(bool selected)
{
	_selected = selected;

	if(_selected && _sel != NULL)
		_button->setResource(_sel);
	if(!_selected && _unsel != NULL)
		_button->setResource(_unsel);
}

bool RadioButton::checkSelected(int sel)
{
	lprintfln("  checkSelected(%d) == %d", sel, _selected);
	if(_selected)
		return(true);
	else
		return(false);
}

void RadioButton::setResources(MAHandle selectedImage, MAHandle unselectedImage)
{
	_sel = selectedImage;
	_unsel = unselectedImage;
}

void RadioButton::setCaption(const char *caption)
{
	_label->setCaption(caption);
}
