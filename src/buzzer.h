#ifndef BUZZER_H_
#define BUZZER_H_

/************************************************************************************************************
 * Includes
 *************************************************************************************************************/
#include "std_types.h"


/************************************************************************************************************
 * Type Definition
 ************************************************************************************************************/
/*if buzzer is connected using negative logic
 *define NEGATIVE_LOGIC*/
#ifdef NEGATIVE_LOGIC
typedef enum
{
	BUZZER_ON, BUZZER_OFF
}BUZZER_pinState;
#else
typedef enum
{
	BUZZER_OFF, BUZZER_ON
}BUZZER_pinState;
#endif

typedef struct
{
	uint8 buzzerPort_ID;
	uint8 buzzerPin_ID;
}BUZZER_Config;

/************************************************************************************************************
 * Function Prototype
 ************************************************************************************************************/
void Buzzer_init(BUZZER_Config* a_buzzerConfig);
void Buzzer_on(BUZZER_Config* a_buzzerConfig);
void Buzzer_off(BUZZER_Config* a_buzzerConfig);

#endif /* BUZZER_H_ */
