#include "buzzer.h"
#include "gpio.h"

/*initialize the buzzer*/
void Buzzer_init(BUZZER_Config* a_buzzerConfig)
{
	GPIO_setupPinDirection(a_buzzerConfig->buzzerPort_ID, a_buzzerConfig->buzzerPin_ID, PIN_OUTPUT); /*set buzzer pin to output*/
	GPIO_writePin(a_buzzerConfig->buzzerPort_ID, a_buzzerConfig->buzzerPin_ID, BUZZER_OFF); /*set buzzer off*/
}

/*turn buzzer on*/
void Buzzer_on(BUZZER_Config* a_buzzerConfig)
{
	GPIO_writePin(a_buzzerConfig->buzzerPort_ID, a_buzzerConfig->buzzerPin_ID, BUZZER_ON);
}

/*turn buzzer off*/
void Buzzer_off(BUZZER_Config* a_buzzerConfig)
{
	GPIO_writePin(a_buzzerConfig->buzzerPort_ID, a_buzzerConfig->buzzerPin_ID, BUZZER_OFF);
}

