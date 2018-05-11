
//--------------------------------------------------------------------------------
#include "F28x_Project.h"

//--------------------------------------------------------------------------------
#include "usr_SCIB.h"
#include "usr_Global_Data.h"
#include "usr_Global_Func.h"

//--------------------------------------------------------------------------------
char	* SCIB_msg;
char	TmpArray_3[3] = {0,0,'\0'};				// 			XX
char	TmpArray_5[5] = {0,0,0,0,'\0'};			// 			XXXX
char	TmpArray_6[6] = {0,0,0,0,0,'\0'};		// XXX.X	XXXXX
char	TmpArray_8[8] = {0,0,0,0,0,0,0,'\0'};	// XXXXX.X	XXXXXXX

//--------------------------------------------------------------------------------
// 简化型 SCI 驱动：无中断、无数据队列管理；利用 16-Level FIFO 进行短帧数据发送
//--------------------------------------------------------------------------------

void usr_SCIB_Setup(void)
{
	// SCIA GPIO Select
	GPIO_SetupPinMux(SCIB_TXD, GPIO_MUX_CPU1, 3); 
	GPIO_SetupPinOptions(SCIB_TXD, GPIO_INPUT, GPIO_PUSHPULL);
	GPIO_SetupPinMux(SCIB_RXD, GPIO_MUX_CPU1, 3);
	GPIO_SetupPinOptions(SCIB_RXD, GPIO_OUTPUT, GPIO_ASYNC);

	GPIO_SetupPinMux(SCIB_RTS_485, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(SCIB_RTS_485, GPIO_OUTPUT, GPIO_PUSHPULL);

	// FIFO Setup
	ScibRegs.SCIFFTX.all = 0xE040;			// 0xE000 -> SCI Reset,  SCI FIFO enable,  Transmit FIFO Rest
											// 0x0040 -> Clear Transmit FIFO
	ScibRegs.SCIFFRX.all = 0x2044;			// 0x2000 -> Receive FIFO reset
											// 0x0044 -> Receive FIFO Interrupt Clear, Receive FIFO Interrupt Level Bits = 0x04
	ScibRegs.SCIFFCT.all = 0x0;				// FIFO transfer delay -> 0

	ScibRegs.SCIFFTX.bit.TXFFIENA = 0;		// TXINT Disable
	ScibRegs.SCIFFRX.bit.RXFFIENA = 0;		// RXINT Disable

	// General Setup
	ScibRegs.SCICCR.all = 0x0007;			// 1 Stop-bit, No Loopback, No Parity, 8-Bit Char, Async Mode, Idle-line protocol

	ScibRegs.SCICTL1.all = 0x0003;			// TXENA = 1;  RXENA = 1;   Internal SCICLK, Disable RX ERR, SLEEP, TXWAKE

//	ScibRegs.SCICTL2.all = 0x0003;			// RX/BKINTENA = 1; TXINTENA = 1;

//	ScibRegs.SCIHBAUD.all = 0x0002;			// Baud-Rate = 9600		@ LSPCLK = 50 MHz (SYSCLK = 200 MHz)
//	ScibRegs.SCILBAUD.all = 0x008A;
//	ScibRegs.SCIHBAUD.all = 0x0001;			// Baud-Rate = 19200
//	ScibRegs.SCILBAUD.all = 0x0045;
	ScibRegs.SCIHBAUD.all = 0x0000;			// Baud-Rate = 115200
	ScibRegs.SCILBAUD.all = 0x0035;

	ScibRegs.SCICTL1.all = 0x0023;			// Relinquish SCI from Reset
}

void usr_SCIB_Xmit(int a)							// Transmit a character
{
    while (ScibRegs.SCIFFTX.bit.TXFFST > 0x0F) {}	// If FIFO is Full up ( Level = 16 )
    ScibRegs.SCITXBUF.all =a;
}

void usr_SCIB_MsgPut(char * msg)					// Transmit a character string ( End with '\0' )
{
	int i = 0;

	while(msg[i] != '\0')
	{
		usr_SCIB_Xmit(msg[i]);
		i++;
	}
}

void usr_SCIB_485_En(void)
{
	GPIO_WritePin(SCIB_RTS_485, 1);			// OE1 Enabled
}

void usr_SCIB_DebugInfo(void)
{
	//------------------------------ 		// Working Time ( ** H, ** M, ** S )
	SCIB_msg = "\r\nWork Time : \0";
	usr_SCIB_MsgPut(SCIB_msg);

	Uint16_to_ASCII_2P(WorkingTime_H, TmpArray_3);
	usr_SCIB_MsgPut(TmpArray_3);
	SCIB_msg = " Hour, \0";
	usr_SCIB_MsgPut(SCIB_msg);

	Uint16_to_ASCII_2P(WorkingTime_M, TmpArray_3);
	usr_SCIB_MsgPut(TmpArray_3);
	SCIB_msg = " Minute, \0";
	usr_SCIB_MsgPut(SCIB_msg);

	Uint16_to_ASCII_2P(WorkingTime_S, TmpArray_3);
	usr_SCIB_MsgPut(TmpArray_3);
	SCIB_msg = " Second;\0";
	usr_SCIB_MsgPut(SCIB_msg);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	//------------------------------ 		// UiA, UiB, UiC
	SCIB_msg = "\r\nVacIn-AB  : \0";
	float_to_ASCII_3P1(UiA_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\nVacIn-BC  : \0";
	float_to_ASCII_3P1(UiB_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\nVacIn-CA  : \0";
	float_to_ASCII_3P1(UiC_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	//------------------------------ 		// Frq of UiA, UiB UiC
	SCIB_msg = "\r\nF-Vin-AB  : \0";
	float_to_ASCII_3P1(UiA_Frq, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\nF-Vin-BC  : \0";
	float_to_ASCII_3P1(UiB_Frq, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\nF-Vin-CA  : \0";
	float_to_ASCII_3P1(UiC_Frq, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	//------------------------------ 		// Frq Error Times
	SCIB_msg = "\r\nError-F_A : \0";
	Uint32_to_ASCII_7P(CapData_UiA_Error, TmpArray_8);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_8);
	SCIB_msg = " Times. \0";
	usr_SCIB_MsgPut(SCIB_msg);

	SCIB_msg = "\r\nError-F_B : \0";
	Uint32_to_ASCII_7P(CapData_UiB_Error, TmpArray_8);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_8);
	SCIB_msg = " Times. \0";
	usr_SCIB_MsgPut(SCIB_msg);

	SCIB_msg = "\r\nError-F_C : \0";
	Uint32_to_ASCII_7P(CapData_UiC_Error, TmpArray_8);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_8);
	SCIB_msg = " Times. \0";
	usr_SCIB_MsgPut(SCIB_msg);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	//------------------------------ 		// IiA, IiB, IiC
	SCIB_msg = "\r\nIacIn-A   : \0";
	float_to_ASCII_3P1(IiA_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\nIacIn-B   : \0";
	float_to_ASCII_3P1(IiB_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\nIacIn-C   : \0";
	float_to_ASCII_3P1(IiC_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	//------------------------------ 		// Uo1, Uo2, Iout, DutyCycle
/*	SCIB_msg = "\r\nVDC-Uo1   : \0";
	float_to_ASCII_3P1(Uo1, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);
*/
	SCIB_msg = "\r\nUo2-ADC   : \0";					// ADC-Uo2
	Uint16_to_ASCII_4P(Uo2_ADC, TmpArray_5);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_5);

	SCIB_msg = "\r\nUo2-RMS   : \0";
	float_to_ASCII_3P1(Uo2, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\nIdc-ADC   : \0";					// ADC-Iout
	Uint16_to_ASCII_4P(Iout_ADC, TmpArray_5);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_5);

	SCIB_msg = "\r\nIdc-RMS   : \0";
	float_to_ASCII_3P1(Iout, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	SCIB_msg = "\r\nDutyCycle : \0";					// Duty Cycle
	float_to_ASCII_3P1(ePwm_DutyCycle, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	//------------------------------ 		// Power & Efficiency
/*
	SCIB_msg = "\r\nPowerIn   : \0";
	float_to_ASCII_5P1(Power_In, TmpArray_8);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_8);

	SCIB_msg = "\r\nPowerOut  : \0";
	float_to_ASCII_5P1(Power_Out, TmpArray_8);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_8);

	SCIB_msg = "\r\nEfficiency: \0";
	float_to_ASCII_3P1(Power_Efficiency, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);
*/
	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

}

