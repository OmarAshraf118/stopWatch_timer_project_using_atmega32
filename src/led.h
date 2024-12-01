#ifndef LED_H_
#define LED_H_

/************************************************************************************************************
 * Includes
 *************************************************************************************************************/
#include "std_types.h"


/************************************************************************************************************
 * Type Definition
 ************************************************************************************************************/
typedef enum
{
	positive_logic, negative_logic
}LED_connectionLogic;

typedef struct
{
	uint8 port_id;
	uint8 pin_id;
	LED_connectionLogic led_connection;
}LED_config;

/************************************************************************************************************
 * Function Prototype
 ************************************************************************************************************/

/*initialize led port and pin*/
void LEDS_init(LED_config* led_config);
void LED_on(LED_config* led_config);
void LED_off(LED_config* led_config);

#endif /*LED_H_*/
