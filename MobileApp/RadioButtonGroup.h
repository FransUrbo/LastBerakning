/*
 * RadioButtonGroup.h
 *
 * http://www.mosync.com/forum/viewtopic.php?f=19&t=507&p=1041#p1041
 * Created on: 2 Mar 2010
 *      Author: sjp
 */

#ifndef RADIOBUTTONGROUP_H_
#define RADIOBUTTONGROUP_H_

#include "RadioButton.h"
#include <MAUtil/Vector.h>

using namespace MAUtil;

class RadioButtonGroup
{
	public:
		RadioButtonGroup(){};
        ~RadioButtonGroup(){};

        void addRadioButton(RadioButton* rb);
        void setSelectedButton(int index);
        int getSelectedButton();

      private:
        Vector<RadioButton*> _buttons;

};

#endif /* RADIOBUTTONGROUP_H_ */
