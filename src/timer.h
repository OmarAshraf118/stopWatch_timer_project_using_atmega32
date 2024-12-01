#ifndef TIMER_H_
#define TIMER_H_

/*******************************************************************************
 *                               Includes	                                   *
 *******************************************************************************/
#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* choose to use the OC0 setting feature or not
 * if set to 0 --> do not use this feature
 * if set to 1 --> use this feature*/
#define COMPARE_MATCH_OUTPUT_MODE 1
/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	timer0, timer1, timer2
}Timer_ID_Type;

typedef enum
{
	/*values of clock types for timer0 and timer1*/
	stopTimer=0, noPrescaler, prescaler_8, prescaler_64, prescaler_256,
	prescaler_1024, externalClockFallingEdge, externalClockRisingEdge,
	/*values of clock types for timer2*/
	timer2_stopTimer=8, timer2_noPrescaler, timer2_prescaler_8,
	timer2_prescaler_32, timer2_prescaler_64, timer2_prescaler_128,
	timer2_prescaler_256, timer2_prescaler_1024
}Timer_ClockType;

typedef enum
{
	normalMode, compareMode
}Timer_ModeType;

typedef struct
{
uint16 timer_InitialValue;
uint16 timer_compare_MatchValue;     /*it will be used in compare mode only*/
Timer_ID_Type  timer_ID;
Timer_ClockType timer_clock;
Timer_ModeType  timer_mode;
}Timer_ConfigType;
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void Timer_init(const Timer_ConfigType * Config_Ptr);
void Timer_deInit(Timer_ID_Type timer_type);
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID );
#endif /* TIMER_H_ */
