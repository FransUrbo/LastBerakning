/*
 * RadioButtonGroup.cpp
 *
 * http://www.mosync.com/forum/viewtopic.php?f=19&t=507&p=1041#p1041
 * Created on: 2 Mar 2010
 *      Author: sjp
 */

#include "RadioButtonGroup.h"

void RadioButtonGroup::addRadioButton(RadioButton* rb)
{
	_buttons.add(rb);
}

void RadioButtonGroup::setSelectedButton(int index)
{
	int ctr = 0;
	Vector_each(RadioButton*, itr, _buttons)
	(*itr)->setSelected(ctr++ == index ? true : false);
}
