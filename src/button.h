#ifndef BUTTON_H_
#define BUTTON_H_

/************************************************************************************************************
 * Definition
 ************************************************************************************************************/
#define BUTTON_PRESSED				1
#define BUTTON_RELEASED				0
/************************************************************************************************************
 * Type Definition
 ************************************************************************************************************/
typedef enum
{
	pullup, pulldown
}BUTTON_connectionType;

typedef struct
{
	uint8 ButtonPort_ID;
	uint8 ButtonPin_ID;
	BUTTON_connectionType ButtonConnection;
}BUTTON_Config;

/************************************************************************************************************
 * Function Prototype
 ************************************************************************************************************/
void BUTTON_init(BUTTON_Config* buttonConfig);
uint8 BUTTON_checkPressed(BUTTON_Config* buttonConfig);

#endif /* BUTTON_H_ */
