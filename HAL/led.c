#include "led.h"
#include "gpio.h"

/*initialize led port and pin*/
 void LEDS_init(LED_config* led_config)
{
	 /* Initialize LED */
	GPIO_setupPinDirection(led_config->port_id, led_config->pin_id, PIN_OUTPUT); /*set pin direction*/
	/* Turn LED off */
	switch(led_config->led_connection)
	{
	case positive_logic:
		GPIO_writePin(led_config->port_id, led_config->pin_id, LOGIC_LOW); /*turn led off*/
		break;
	case negative_logic:
		GPIO_writePin(led_config->port_id, led_config->pin_id, LOGIC_HIGH); /*turn led off*/
		break;
	}

}

 void LED_on(LED_config* led_config)
{
	 /* Turn LED on */
	switch(led_config->led_connection)
	{
	case positive_logic:
		GPIO_writePin(led_config->port_id, led_config->pin_id, LOGIC_HIGH); /*turn led on*/
		break;
	case negative_logic:
		GPIO_writePin(led_config->port_id, led_config->pin_id, LOGIC_LOW); /*turn led on*/
		break;
	}
}

 void LED_off(LED_config* led_config)
{
	/* Turn LED off */
	switch(led_config->led_connection)
	{
	case positive_logic:
		GPIO_writePin(led_config->port_id, led_config->pin_id, LOGIC_LOW); /*turn led off*/
		break;
	case negative_logic:
		GPIO_writePin(led_config->port_id, led_config->pin_id, LOGIC_HIGH); /*turn led off*/
		break;
	}
}

