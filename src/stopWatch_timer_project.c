/************************************************************************************************
 * Project: Stopwatch / Timer																	*
 * 																								*
 * Description: a stopwatch that has two modes (count up and count down) that displays			*
 * 				the time using six seven-segment displays 2 for seconds, 2 for minutes			*
 * 				and 2 for hours. for count down, you can set the starting time as desired.		*
 * 				There is also a buzzer that turns on when zero is reached in count down.		*
 * 																								*
 * Author: Eng. Omar Ashraf																 		*
 ************************************************************************************************/

#include "timer.h"
#include "buzzer.h"
#include "gpio.h"
#include "seven_segment_display.h"
#include "button.h"
#include "led.h"
#include <util/delay.h>
#include <avr/io.h>

/************************************************************************************************
 * 											Definitions											*
 ************************************************************************************************/
#define TIMER_INITIAL_VALUE 						0
#define TICKS_REQUIRED_FOR_1_SECOND 				15625
#define TIMER_PRESCALER_VALUE 						1024
#define COUNTUP_MODE								0
#define COUNTDOWN_MODE								1
#define TARGET_REACHED								1
#define TARGET_NOT_REACHED							0
#define BUZZER_STATE_ON								1
#define BUZZER_STATE_OFF							0
#define MULTIPLEXING_DELAY							5
#define BUTTON_DEBOUNCE_PERIOD						30
/************************************************************************************************
 * type definition																				*
 ************************************************************************************************/
typedef union
{
	uint16 flags_size;
	struct
	{
		uint16 reset_flag		: 1;
		uint16 resume_flag		: 1;
		uint16 pause_flag		: 1;
		uint16 timerMode_flag	: 1;
		uint16 hoursInc_flag	: 1;
		uint16 hoursDec_flag	: 1;
		uint16 minutesInc_flag	: 1;
		uint16 minutesDec_flag	: 1;
		uint16 secondsInc_flag	: 1;
		uint16 secondsDec_flag	: 1;
		uint16 	: 6;
	}flags;
}buttons_flag;
/************************************************************************************************
 * Global variables																				*
 ************************************************************************************************/
static uint8 timer_mode = 0;		/* flag to indicate timer mode (counting up = 0 / counting down = 1) */
static uint8 seconds_counter = 0; 	/* keeps track of the seconds display value */
static uint8 minutes_counter = 0; 	/* keeps track of the minutes display value */
static uint8 hours_counter = 0;   	/* keeps track of the hours display value */
static uint8 buzzer_state = 0;		/* keeps track of the state of buzzer (0 -> buzzer off, 1 -> buzzer on) */
static uint8 target_reached_flag = 0; /* "1" -> timer reached 99:59:59 in counting up / 00:00:00 in counting down and "0" otherwise */
static buttons_flag buttons_pressed_flags; /* contains 10 flags that keep track of state of all buttons */
/************************************************************************************************
 * function prototypes																			*
 ************************************************************************************************/
void count_up_mode(void);
void count_down_mode(void);

int main(void)
{
	SREG |=(1<<7); /* enable interrupts */
	/*********************************** Local Variables *****************************************/
	Timer_ConfigType s_timer1_init_values = {TIMER_INITIAL_VALUE, TICKS_REQUIRED_FOR_1_SECOND, timer1, prescaler_1024, compareMode};
	BUZZER_Config s_buzzer_init_values = {PORTD_ID, PIN0_ID};
	/* initialize the values required to set the seven segments */
	SEVEN_SEGMENT_Config s_sevenSegment_init_values = {PORTC_ID, PIN0_ID};

	/* set pins responsible for multiplexing the 6 seven_segments' display */
	GPIO_setupPortDirection(PORTA_ID, (PORT_OUTPUT&0x3F)); /* pins A0 to A5 are set to output */
	for(uint8 i=0; i<6; i++){GPIO_writePin(PORTA_ID, i, LOGIC_LOW);}; /* initialize A0 to A5 to low */

	/* initialize the 10 buttons required for the timer */
	BUTTON_Config	s_resetButton = {PORTD_ID, PIN2_ID, pullup},
					s_resumeButton = {PORTB_ID, PIN2_ID, pullup},
					s_pauseButton = {PORTD_ID, PIN3_ID, pulldown},
					s_timerModeButton = {PORTB_ID, PIN7_ID, pullup},
					s_hoursIncrement = {PORTB_ID, PIN1_ID, pullup},
					s_hoursDecrement = {PORTB_ID, PIN0_ID, pullup},
					s_minutesIncrement = {PORTB_ID, PIN4_ID, pullup},
					s_minutesDecrement = {PORTB_ID, PIN3_ID, pullup},
					s_secondsIncrement = {PORTB_ID, PIN6_ID, pullup},
					s_secondsDecrement = {PORTB_ID, PIN5_ID, pullup};
	buttons_pressed_flags.flags_size=0; /* set all button flags to zero */

	LED_config	s_ledCountUpMode = {PORTD_ID, PIN4_ID, positive_logic},
				s_ledCountDownMode = {PORTD_ID, PIN5_ID, positive_logic};

	/******* Turn on internal pull_up for all but the pause button *******/
	PORTB = 0xFF; /* turn on internal pull-up resistors for all pins on portB */
	PORTD |= 0x04; /* turn on internal pull-up resistor for pin D2 */

	/********************************** Initialize Drivers ***************************************/
	Buzzer_init(&s_buzzer_init_values);
	/* initialize all buttons */
	BUTTON_init(&s_resetButton);
	BUTTON_init(&s_resumeButton);
	BUTTON_init(&s_pauseButton);
	BUTTON_init(&s_timerModeButton);
	BUTTON_init(&s_hoursIncrement);
	BUTTON_init(&s_hoursDecrement);
	BUTTON_init(&s_minutesIncrement);
	BUTTON_init(&s_minutesDecrement);
	BUTTON_init(&s_secondsIncrement);
	BUTTON_init(&s_secondsDecrement);
	/* initialize seven segments */
	Seven_Segment_init(&s_sevenSegment_init_values);
	/* initialize LEDs that indicate mode */
	LEDS_init(&s_ledCountUpMode);
	LEDS_init(&s_ledCountDownMode);
	/* initially make timer count up using timer 1 */
	Timer_init(&s_timer1_init_values);
	Timer_setCallBack(count_up_mode, timer1);
	LED_on(&s_ledCountUpMode); /* turn on led that indicates count up mode */

	/* Super Loop */
	for(;;)
	{
		/* if change mode button is pressed */
		if(BUTTON_checkPressed(&s_timerModeButton))
		{
			_delay_ms(BUTTON_DEBOUNCE_PERIOD); /* delay to overcome debounce effect */
			if(BUTTON_checkPressed(&s_timerModeButton) && !buttons_pressed_flags.flags.timerMode_flag)
			{
				buttons_pressed_flags.flags.timerMode_flag = 1; /* indicates that button is still pressed */
				timer_mode^=1; /* switch timer mode */
				target_reached_flag = 0; /* start incrementing / decrementing again */
				/* change which mode the timer is running on */
				switch(timer_mode)
				{
				case COUNTUP_MODE:
					Timer_setCallBack(count_up_mode, timer1); /* make timer count up using timer 1 */
					LED_on(&s_ledCountUpMode);
					LED_off(&s_ledCountDownMode);
					break;
				case COUNTDOWN_MODE:
					Timer_setCallBack(count_down_mode, timer1); /* make timer count down using timer 1 */
					LED_on(&s_ledCountDownMode);
					LED_off(&s_ledCountUpMode);
					break;
				}
			}
		}
		else
		{
			buttons_pressed_flags.flags.timerMode_flag=0; /* indicate that button is released */
		}

		/********** if timer pause button is pressed **********/

		if(BUTTON_checkPressed(&s_pauseButton))
		{
			_delay_ms(BUTTON_DEBOUNCE_PERIOD); /* delay to overcome debounce effect */
			if(BUTTON_checkPressed(&s_pauseButton) && !buttons_pressed_flags.flags.pause_flag)
			{
				buttons_pressed_flags.flags.pause_flag = 1; /* indicates that button is still pressed */
				Timer_deInit(timer1);/* deinitialize timer1 */
			}
		}
		else
		{
			buttons_pressed_flags.flags.pause_flag=0; /* indicate that button is released */
		}

		/********** if timer resume button is pressed **********/

		if(BUTTON_checkPressed(&s_resumeButton))
		{
			_delay_ms(BUTTON_DEBOUNCE_PERIOD); /* delay to overcome debounce effect */
			if(BUTTON_checkPressed(&s_resumeButton) && !buttons_pressed_flags.flags.resume_flag)
			{
				buttons_pressed_flags.flags.resume_flag = 1; /* indicates that button is still pressed */
				Timer_init(&s_timer1_init_values);/* initialize timer1 */
			}
		}
		else
		{
			buttons_pressed_flags.flags.resume_flag=0; /* indicate that button is released */
		}

		/********** if timer reset button is pressed **********/

		if(BUTTON_checkPressed(&s_resetButton))
		{
			_delay_ms(BUTTON_DEBOUNCE_PERIOD); /* delay to overcome debounce effect */
			if(BUTTON_checkPressed(&s_resetButton) && !buttons_pressed_flags.flags.reset_flag)
			{
				buttons_pressed_flags.flags.reset_flag = 1; /* indicates that button is still pressed */
				/* reset timer values */
				seconds_counter = 0;
				minutes_counter = 0;
				hours_counter = 0;
			}
		}
		else
		{
			buttons_pressed_flags.flags.reset_flag=0; /* indicate that button is released */
		}

		/********** Increment/Decrement buttons **********/

		/* increment seconds */
		if(BUTTON_checkPressed(&s_secondsIncrement))
		{
			_delay_ms(BUTTON_DEBOUNCE_PERIOD); /* delay to overcome debounce effect */
			if(BUTTON_checkPressed(&s_secondsIncrement) && !buttons_pressed_flags.flags.secondsInc_flag)
			{
				buttons_pressed_flags.flags.secondsInc_flag = 1; /* indicates that button is still pressed */
				seconds_counter++; /* increment seconds by 1 */
			}
		}
		else
		{
			buttons_pressed_flags.flags.secondsInc_flag=0; /* indicate that button is released */
		}

		/* Decrement seconds */
		if(BUTTON_checkPressed(&s_secondsDecrement))
		{
			_delay_ms(BUTTON_DEBOUNCE_PERIOD); /* delay to overcome debounce effect */
			if(BUTTON_checkPressed(&s_secondsDecrement) && !buttons_pressed_flags.flags.secondsDec_flag)
			{
				buttons_pressed_flags.flags.secondsDec_flag = 1; /* indicates that button is still pressed */
				if(seconds_counter!=0)
					seconds_counter--; /* Decrement seconds by 1 */
			}
		}
		else
		{
			buttons_pressed_flags.flags.secondsDec_flag=0; /* indicate that button is released */
		}

		/* increment minutes */
		if(BUTTON_checkPressed(&s_minutesIncrement))
		{
			_delay_ms(BUTTON_DEBOUNCE_PERIOD); /* delay to overcome debounce effect */
			if(BUTTON_checkPressed(&s_minutesIncrement) && !buttons_pressed_flags.flags.minutesInc_flag)
			{
				buttons_pressed_flags.flags.minutesInc_flag = 1; /* indicates that button is still pressed */
				minutes_counter++; /* increment minutes by 1 */
			}
		}
		else
		{
			buttons_pressed_flags.flags.minutesInc_flag=0; /* indicate that button is released */
		}

		/* Decrement minutes */
		if(BUTTON_checkPressed(&s_minutesDecrement))
		{
			_delay_ms(BUTTON_DEBOUNCE_PERIOD); /* delay to overcome debounce effect */
			if(BUTTON_checkPressed(&s_minutesDecrement) && !buttons_pressed_flags.flags.minutesDec_flag)
			{
				buttons_pressed_flags.flags.minutesDec_flag = 1; /* indicates that button is still pressed */
				if(minutes_counter!=0)
					minutes_counter--; /* Decrement minutes by 1 */
			}
		}
		else
		{
			buttons_pressed_flags.flags.minutesDec_flag=0; /* indicate that button is released */
		}

		/* increment hours */
		if(BUTTON_checkPressed(&s_hoursIncrement))
		{
			_delay_ms(BUTTON_DEBOUNCE_PERIOD); /* delay to overcome debounce effect */
			if(BUTTON_checkPressed(&s_hoursIncrement) && !buttons_pressed_flags.flags.hoursInc_flag)
			{
				buttons_pressed_flags.flags.hoursInc_flag = 1; /* indicates that button is still pressed */
				hours_counter++; /* increment hours by 1 */
			}
		}
		else
		{
			buttons_pressed_flags.flags.hoursInc_flag=0; /* indicate that button is released */
		}

		/* Decrement hours */
		if(BUTTON_checkPressed(&s_hoursDecrement))
		{
			_delay_ms(BUTTON_DEBOUNCE_PERIOD); /* delay to overcome debounce effect */
			if(BUTTON_checkPressed(&s_hoursDecrement) && !buttons_pressed_flags.flags.hoursDec_flag)
			{
				buttons_pressed_flags.flags.hoursDec_flag = 1; /* indicates that button is still pressed */
				if(hours_counter!=0)
					hours_counter--; /* Decrement hours by 1 */
			}
		}
		else
		{
			buttons_pressed_flags.flags.hoursDec_flag=0; /* indicate that button is released */
		}

		/********** display values on seven-segments **********/

		/* display seconds */
		GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_HIGH); /* turn on 1st display from the right */
		Seven_Segment_display(seconds_counter%10,&s_sevenSegment_init_values); /* display the units of seconds on 1st display */
		_delay_ms(MULTIPLEXING_DELAY);
		GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_LOW); /* turn off 1st display from the right */

		GPIO_writePin(PORTA_ID, PIN4_ID, LOGIC_HIGH); /* turn on 2nd display from the right */
		Seven_Segment_display(seconds_counter/10,&s_sevenSegment_init_values); /* display the tens of seconds on 2nd display */
		_delay_ms(MULTIPLEXING_DELAY);
		GPIO_writePin(PORTA_ID, PIN4_ID, LOGIC_LOW); /* turn off 2nd display from the right */

		/* display minutes */
		GPIO_writePin(PORTA_ID, PIN3_ID, LOGIC_HIGH); /* turn on 3rd display from the right */
		Seven_Segment_display(minutes_counter%10,&s_sevenSegment_init_values); /* display the units of minutes on 3rd display */
		_delay_ms(MULTIPLEXING_DELAY);
		GPIO_writePin(PORTA_ID, PIN3_ID, LOGIC_LOW); /* turn off 3rd display from the right */

		GPIO_writePin(PORTA_ID, PIN2_ID, LOGIC_HIGH); /* turn on 4th display from the right */
		Seven_Segment_display(minutes_counter/10,&s_sevenSegment_init_values); /* display the tens of minutes on 4th display */
		_delay_ms(MULTIPLEXING_DELAY);
		GPIO_writePin(PORTA_ID, PIN2_ID, LOGIC_LOW); /* turn off 4th display from the right */

		/* display hours */
		GPIO_writePin(PORTA_ID, PIN1_ID, LOGIC_HIGH); /* turn on 5th display from the right */
		Seven_Segment_display(hours_counter%10,&s_sevenSegment_init_values); /* display the units of hours on 5th display */
		_delay_ms(MULTIPLEXING_DELAY);
		GPIO_writePin(PORTA_ID, PIN1_ID, LOGIC_LOW); /* turn off 5th display from the right */

		GPIO_writePin(PORTA_ID, PIN0_ID, LOGIC_HIGH); /* turn on 6th display from the right */
		Seven_Segment_display(hours_counter/10,&s_sevenSegment_init_values); /* display the tens of hours on 6th display */
		_delay_ms(MULTIPLEXING_DELAY);
		GPIO_writePin(PORTA_ID, PIN0_ID, LOGIC_LOW); /* turn off 6th display from the right */


		/*********** Buzzer state ***********/
		if(target_reached_flag && !buzzer_state) /* if target reached and buzzer is off */
		{
			Buzzer_on(&s_buzzer_init_values); /* turn buzzer on */
			buzzer_state = BUZZER_STATE_ON; /* update buzzer state to be on */
		}
		else if(!target_reached_flag && buzzer_state) /* if target is not reached and buzzer is on */
		{
			Buzzer_off(&s_buzzer_init_values); /* turn buzzer off */
			buzzer_state = BUZZER_STATE_OFF; /* update buzzer state to be off */
		}


	}

}

/*********************************************************************************
 * function definition
 *********************************************************************************/
void count_up_mode(void)
{
	/* check if target is reached */
	switch(target_reached_flag)
	{
	case TARGET_REACHED: /* if target is reached (99:59:59), do not add values */
		break;
	case TARGET_NOT_REACHED: /* if target is not reached (99:59:59), add values */
		if(seconds_counter<59) /* if seconds counter is 58 or less */
		{
			seconds_counter++; /* increment time by 1 second */
		}
		else
		{
			if(minutes_counter<59) /* if minutes counter is 58 or less */
			{
				seconds_counter = 0; /* reset seconds counter */
				minutes_counter++; /* increment time by 1 minute */
			}
			else
			{
				if(hours_counter<99) /* if hours counter is 99 or less */
				{
					seconds_counter = 0; /* reset seconds counter */
					minutes_counter = 0; /* reset minutes counter */
					hours_counter++; /* increment time by 1 hour */
				}
				else
				{
					target_reached_flag = 1; /* timer reached maximum value 99:59:59 */
				}
			}
		}
		break;
	}
}

void count_down_mode(void)
{
	/* check if target is reached */
	switch(target_reached_flag)
	{
	case TARGET_REACHED: /* if target is reached (00:00:00), do not decrement values */
		break;
	case TARGET_NOT_REACHED: /* if target is not reached (00:00:00), decrement values */
		if(seconds_counter>0) /* if seconds counter is more than 0 */
		{
			seconds_counter--; /* decrement time by 1 second */
		}
		else
		{
			if(minutes_counter>0) /* if minutes counter is more than 0 */
			{
				minutes_counter--; /* decrement time by 1 minute */
				seconds_counter = 59; /* recount from 59 seconds */
			}
			else
			{
				if(hours_counter>0) /* if hours counter is more than 0 */
				{
					hours_counter--; /* decrement time by 1 hour */
					minutes_counter = 59; /* recount from 59 minutes */
					seconds_counter = 59; /* recount from 59 seconds */
				}
				else
				{
					target_reached_flag = 1; /* timer reached value 00:00:00 in count down */
				}
			}
		}
		break;
	}


}
