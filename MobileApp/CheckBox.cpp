/*
 * http://www.mosync.com/forum/viewtopic.php?f=19&t=466
 * Post by bruno on Thu Feb 25, 2010 12:10 pm
 *
 * Example:
 * 	label = createLabel("Visible", 32);
 *	CheckBox * checkBox1 = new CheckBox(scrWidth - 50, 0, 20, 20, label);
 *	checkBox1->setResources(RES_CHECKBOX_UNCHECKED, RES_CHECKBOX_CHECKED);
 *	listBox->add(label);
 *
 *	void ConfigurationService::keyPressEvent(int keyCode) {
 *		switch(keyCode) {
 *			case MAK_FIRE:
 *				if (listBox->getSelectedIndex() ==1) {
 *					CheckBox * cb = (CheckBox *)((Label*)listBox->getChildren()[listBox->getSelectedIndex()])->getChildren()[0];
 *					cb->flip();
 *				}
 *				break;
 *		}
 *	}
 */

/* Modified by Turbo Fredriksson <turbo@bayour.com>
 *
 * This screen is the main input data screen.
 * $Id: CheckBox.cpp,v 1.1 2010-03-03 13:50:55 turbo Exp $
 */

#include <conprint.h> /* lprintfln() */

#include "Checkbox.h"

CheckBox::CheckBox(int x, int y, int width, int height, Widget* parent = NULL)
		 : Widget(x, y, width, height, parent)
{
	image = new Image(0, 0, width, height, this);
	text = new Label(image->getWidth(), 0, width - image->getWidth(), height, this);

	_checked = false;
}

CheckBox::~CheckBox()
{}

void CheckBox::drawWidget()
{}

void CheckBox::setResources(MAHandle unchecked, MAHandle checked)
{
	resChecked = checked;
	resUnchecked = unchecked;

	updateCheckBox();
}

void CheckBox::flip()
{
	_checked = !_checked;
	updateCheckBox();
}

bool CheckBox::isChecked()
{
	return _checked;
}

void CheckBox::updateCheckBox()
{
	if(_checked)
		image->setResource(resChecked);
	else
		image->setResource(resUnchecked);
}
