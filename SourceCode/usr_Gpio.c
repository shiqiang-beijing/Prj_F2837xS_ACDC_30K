
//--------------------------------------------------------------------------------
#include "F28x_Project.h"

//--------------------------------------------------------------------------------
#include "usr_Gpio.h"

//--------------------------------------------------------------------------------
void usr_Gpio_Setup(void)
{
	//------------------------------
	GPIO_SetupPinMux(GPIO_EPWM_OE1, GPIO_MUX_CPU1, 0);					// OE1
	GPIO_SetupPinOptions(GPIO_EPWM_OE1, GPIO_OUTPUT, GPIO_PUSHPULL);

	GPIO_SetupPinMux(GPIO_EPWM_OE2, GPIO_MUX_CPU1, 0);					// OE2
	GPIO_SetupPinOptions(GPIO_EPWM_OE2, GPIO_OUTPUT, GPIO_PUSHPULL);

	GPIO_SetupPinMux(GPIO_BUZZER, GPIO_MUX_CPU1, 0);					// BUZZER
	GPIO_SetupPinOptions(GPIO_BUZZER, GPIO_OUTPUT, GPIO_PUSHPULL);

	GPIO_WritePin(GPIO_EPWM_OE1, 1);									// Disable OE1
	GPIO_WritePin(GPIO_EPWM_OE2, 1);									// Disable OE2
	GPIO_WritePin(GPIO_BUZZER, 0);										// Disable BUZZER

	//------------------------------
	GPIO_SetupPinMux(GPIO_OUT_D, GPIO_MUX_CPU1, 0);						// OUT_IO_D
	GPIO_SetupPinOptions(GPIO_OUT_D, GPIO_OUTPUT, GPIO_PUSHPULL);

	GPIO_SetupPinMux(GPIO_OUT_E, GPIO_MUX_CPU1, 0);						// OUT_IO_E
	GPIO_SetupPinOptions(GPIO_OUT_E, GPIO_OUTPUT, GPIO_PUSHPULL);

	GPIO_SetupPinMux(GPIO_OUT_F, GPIO_MUX_CPU1, 0);						// OUT_IO_F
	GPIO_SetupPinOptions(GPIO_OUT_F, GPIO_OUTPUT, GPIO_PUSHPULL);

	GPIO_WritePin(GPIO_OUT_D,0);
	GPIO_WritePin(GPIO_OUT_E,0);
	GPIO_WritePin(GPIO_OUT_F,0);
}

//--------------------------------------------------------------------------------
void usr_Gpio_OE1_En(void)
{
	GPIO_WritePin(GPIO_EPWM_OE1, 0);
}

void usr_Gpio_OE1_Dis(void)
{
	GPIO_WritePin(GPIO_EPWM_OE1, 1);
}

void usr_Gpio_OE2_En(void)
{
	GPIO_WritePin(GPIO_EPWM_OE2, 0);
}
