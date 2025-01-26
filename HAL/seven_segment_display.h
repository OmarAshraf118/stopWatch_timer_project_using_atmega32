#ifndef SEVEN_SEGMENT_DISPLAY_H_
#define SEVEN_SEGMENT_DISPLAY_H_

/************************************************************************************************************
 * Includes
 *************************************************************************************************************/
#include "std_types.h"

/************************************************************************************************************
 * Definitions
 *************************************************************************************************************/
#define SEVEN_SEGMENT_NUMBER_OF_PINS 			4 /* assuming the use of BCD to seven segment decoder */

/************************************************************************************************************
 * Types Definition
 ************************************************************************************************************/
typedef struct
{
	uint8 sevenSegmentLedsPort_ID; /* the port that the leds of the seven segment are connected */
	uint8 sevenSegmentLedsFirstPin_ID; /* the first pin that the leds of the seven segment are connected */
}SEVEN_SEGMENT_Config;


/************************************************************************************************************
 * Function Prototype
 ************************************************************************************************************/
void Seven_Segment_init(SEVEN_SEGMENT_Config* a_sevenSegmentConfig);
void Seven_Segment_display(const uint8 number, SEVEN_SEGMENT_Config* a_sevenSegmentConfig);
#endif /* SEVEN_SEGMENT_DISPLAY_H_ */
