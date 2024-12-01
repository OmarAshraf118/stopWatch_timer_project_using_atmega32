#include <avr/interrupt.h>
#include "timer.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
static volatile void (*s_timer0CallbackPtr)(void) = NULL_PTR;
static volatile void (*s_timer1CallbackPtr)(void) = NULL_PTR;
static volatile void (*s_timer2CallbackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER0_OVF_vect)
{
	if(s_timer0CallbackPtr != NULL_PTR)
	{
		(*s_timer0CallbackPtr)();
	}
}

ISR(TIMER0_COMP_vect)
{
	if(s_timer0CallbackPtr != NULL_PTR)
	{
		(*s_timer0CallbackPtr)();
	}
}

ISR(TIMER1_OVF_vect)
{
	if(s_timer1CallbackPtr != NULL_PTR)
	{
		(*s_timer1CallbackPtr)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(s_timer1CallbackPtr != NULL_PTR)
	{
		(*s_timer1CallbackPtr)();
	}
}

ISR(TIMER2_OVF_vect)
{
	if(s_timer2CallbackPtr != NULL_PTR)
	{
		(*s_timer2CallbackPtr)();
	}
}

ISR(TIMER2_COMP_vect)
{
	if(s_timer2CallbackPtr != NULL_PTR)
	{
		(*s_timer2CallbackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void Timer_init(const Timer_ConfigType * Config_Ptr)
{
	switch(Config_Ptr->timer_ID)
	{
	case timer0:
		/*********TCCR0********
		 * FOC0 set to 1 in non_pwm mode
		 * WGM00:01 if WGM01 = 0 -> normal else WGM01 = 1 -> CTC
		 * COM00:01 to decide the effect the timer has on pin OC0
		 * CS00:02 to choose clock type*/


		TCCR0 |= (1<<FOC0); /*set FOC0 to 1 is non-pwm mode*/

		/*if Config_Ptr->timer_mode = 0 then Normal mode
		 *if Config_Ptr->timer_mode = 1 then CTC mode*/
		TCCR0 |= ((Config_Ptr->timer_mode)<<WGM01); /*decide wave generation mode normal/CTC*/

		if(Config_Ptr->timer_clock & 0x08) /*clockType have a value of 8 or more*/
		{
			/*Do nothing*/
		}
		else
		{
			/* set values of CS10:12 to decide the timer clock
			 * CS10:12 are the first 3 bits in TCCR0 register*/
			TCCR0 = (TCCR0 & 0xf8) | (Config_Ptr->timer_clock & 0x07);
		}

		/*********TCNT0********
		 * set starting value of timer*/
		TCNT0 = (Config_Ptr->timer_InitialValue & 0x00FF); /*take 1st 8-bits only (timer0 is 8-bit timer)*/

		/*********OCR0*********/
		if(Config_Ptr->timer_mode==compareMode)
			OCR0 = (Config_Ptr->timer_compare_MatchValue & 0x00FF); /*take 1st 8-bits only (timer0 is 8-bit timer)*/

		/******TIMSK******/
		if(Config_Ptr->timer_mode==normalMode)
			TIMSK |= (1<<TOIE0); /*enable interrupt on overflow mode*/
		else
			TIMSK |= (1<<OCIE0); /*enable interrupt on CTC mode*/
		break;
/***************************************************************************Timer1**************************************************************************************/
	case timer1:
		/*********TCCR1A********
		 * FOC1A:B set to 1 in non_pwm mode
		 * COM1B0:1 and COM1A0:1 to decide the effect the timer has on pin OC1A and OC1B
		 */


		TCCR1A |= (1<<FOC1A) | (1<<FOC1B); /*set FOC0 to 1 is non-pwm mode*/

		/*******TCCR1B*********
		 * WGM12 if WGM12 = 0 -> normal else WGM12 = 1 -> CTC
		 * CS10:12 to choose clock type*/

		/*if Config_Ptr->timer_mode = 0 then Normal mode
		 *if Config_Ptr->timer_mode = 1 then CTC mode*/
		TCCR1B |= ((Config_Ptr->timer_mode)<<WGM12); /*decide wave generation mode normal/CTC*/

		if(Config_Ptr->timer_clock & 0x08) /*clockType have a value of 8 or more*/
		{
			/*Do nothing*/
		}
		else
		{
			/* set values of CS10:12 to decide the timer clock
			 * CS10:12 are the first 3 bits in TCCR0 register*/
			TCCR1B = (TCCR1B & 0xf8) | (Config_Ptr->timer_clock & 0x07);
		}

		/*********TCNT0********
		 * set starting value of timer*/
		TCNT1 = (Config_Ptr->timer_InitialValue); /*take 16-bits*/

		/*********OCR1A*********/
		if(Config_Ptr->timer_mode==compareMode)
			OCR1A = (Config_Ptr->timer_compare_MatchValue); /*take 16-bits*/

		/******TIMSK******/
		if(Config_Ptr->timer_mode==normalMode)
			TIMSK |= (1<<TOIE1); /*enable interrupt on overflow mode*/
		else
			TIMSK |= (1<<OCIE1A); /*enable interrupt on CTC mode*/
		break;
/***************************************************************************Timer2**************************************************************************************/
	case timer2:
		/*********TCCR2********
		 * FOC2 set to 1 in non_pwm mode
		 * WGM20:21 if WGM01 = 0 -> normal else WGM01 = 1 -> CTC
		 * COM20:21 to decide the effect the timer has on pin OC0
		 * CS00:02 to choose clock type*/

		TCCR2 |= (1<<FOC2); /*set FOC2 to 1 is non-pwm mode*/

		/*if Config_Ptr->timer_mode = 0 then Normal mode
		 *if Config_Ptr->timer_mode = 1 then CTC mode*/
		TCCR2 |= ((Config_Ptr->timer_mode)<<WGM21); /*decide wave generation mode normal/CTC*/

		if(Config_Ptr->timer_clock & 0x08) /*clockType have a value of 8 or more*/
		{
			/* set values of CS20:22 to decide the timer clock
			 * CS20:22 are the first 3 bits in TCCR2 register*/
			TCCR2 |= (TCCR2 & 0xf8) | ((Config_Ptr->timer_clock-8) & 0x07);
		}
		else
		{
			/*Do nothing*/
		}

		/*********TCNT2********
		 * set starting value of timer*/
		TCNT2 = (Config_Ptr->timer_InitialValue & 0x00FF); /*take 1st 8-bits only (timer2 is 8-bit timer)*/

		/*********OCR2*********/
		if(Config_Ptr->timer_mode==compareMode)
			OCR2 = (Config_Ptr->timer_compare_MatchValue & 0x00FF); /*take 1st 8-bits only (timer2 is 8-bit timer)*/

		/******TIMSK******/
		if(Config_Ptr->timer_mode==normalMode)
			TIMSK |= (1<<TOIE2); /*enable interrupt on overflow mode*/
		else
			TIMSK |= (1<<OCIE2); /*enable interrupt on CTC mode*/
		break;
	}

}

void Timer_deInit(Timer_ID_Type timer_type)
{
	switch(timer_type)
	{
	case timer0:
		TCNT0  = 0;
		OCR0   = 0;
		TIMSK  = 0;
		TCCR0 &= (0xf8);
		break;
	case timer1:
		TCNT1  = 0;
		OCR1A  = 0;
		OCR1B  = 0;
		TIMSK  &= (0xEC);
		TCCR1B &= (0xf8);
		break;
	case timer2:
		TCNT2  = 0;
		OCR2   = 0;
		TIMSK  = 0;
		TCCR2 &= (0xf8);
		break;
	}
}

void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID )
{
	switch(a_timer_ID)
	{
	case timer0:
		s_timer0CallbackPtr = a_ptr;
		break;
	case timer1:
		s_timer1CallbackPtr = a_ptr;
		break;
	case timer2:
		s_timer2CallbackPtr = a_ptr;
		break;
	}
}
