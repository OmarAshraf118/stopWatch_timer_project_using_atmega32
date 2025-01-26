#include "seven_segment_display.h"
#include "gpio.h"


/* function to configure seven segment display ports and pins */
void Seven_Segment_init(SEVEN_SEGMENT_Config* a_sevenSegmentConfig)
{
	/* loop according to the number of pins that the seven segment need to operate */
	for(uint8 pinSetup_iterator = 0; pinSetup_iterator < SEVEN_SEGMENT_NUMBER_OF_PINS; pinSetup_iterator++)
	{
		/* setup the pin direction of each individual pin */
		GPIO_setupPinDirection(a_sevenSegmentConfig->sevenSegmentLedsPort_ID, a_sevenSegmentConfig->sevenSegmentLedsFirstPin_ID + pinSetup_iterator, PIN_OUTPUT);
	}
}

/* function to display a number on seven segment */
void Seven_Segment_display(const uint8 number, SEVEN_SEGMENT_Config* a_sevenSegmentConfig)
{
	/* loop according to the number of pins that the seven segment need to operate */
	for(uint8 pinWrite_iterator = 0; pinWrite_iterator < SEVEN_SEGMENT_NUMBER_OF_PINS; pinWrite_iterator++)
	{
		/* write the value of the number to the pins */
		GPIO_writePin(a_sevenSegmentConfig->sevenSegmentLedsPort_ID, a_sevenSegmentConfig->sevenSegmentLedsFirstPin_ID + pinWrite_iterator, ((number & (1<<pinWrite_iterator) )? 1 : 0));
	}
}


