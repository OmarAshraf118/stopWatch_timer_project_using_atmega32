#include "gpio.h"
#include "button.h"

void BUTTON_init(BUTTON_Config* buttonConfig)
{
	GPIO_setupPinDirection(buttonConfig->ButtonPort_ID, buttonConfig->ButtonPin_ID, PIN_INPUT);
}
/* a function that checks if a button is pressed and returns 1 if true */
uint8 BUTTON_checkPressed(BUTTON_Config* buttonConfig)
{
	switch(buttonConfig->ButtonConnection)
	{
	case pullup:
		if(!GPIO_readPin(buttonConfig->ButtonPort_ID, buttonConfig->ButtonPin_ID))
			return BUTTON_PRESSED;
		else
			return BUTTON_RELEASED;
		break;
	case pulldown:
		if(GPIO_readPin(buttonConfig->ButtonPort_ID, buttonConfig->ButtonPin_ID))
			return BUTTON_PRESSED;
		else
			return BUTTON_RELEASED;
		break;
	}
	return 0;
}

