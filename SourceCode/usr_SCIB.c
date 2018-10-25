
//--------------------------------------------------------------------------------
#include "F28x_Project.h"

//--------------------------------------------------------------------------------
#include "usr_SCIB.h"
#include "usr_Global_Data.h"
#include "usr_Global_Func.h"

//--------------------------------------------------------------------------------
char	* SCIB_msg;

char	TmpArray_3[3] = {0,0,'\0'};								// Format:			XX
char	TmpArray_5[5] = {0,0,0,0,'\0'};							// Format:			XXXX
char	TmpArray_6[6] = {0,0,0,0,0,'\0'};						// Format: XXX.X	XXXXX
char	TmpArray_8[8] = {0,0,0,0,0,0,0,'\0'};					// Format: XXXXX.X	XXXXXXX

char	TmpArray_16[16] = {81,48,49,0,0,0,0,0,0,0,0,0,0,0,0,'\0'};	// Default Start:	Q01

//--------------------------------------------------------------------------------
// 配置：开启 Rx 中断，RxBuff 长度=1（应对变化长度的输入帧）；采用 16-Level FIFO 进行短帧数据发送
//--------------------------------------------------------------------------------
#ifndef	SCIB_MONITOR
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
#else
void usr_SCIB_Setup(void)
{
	// SCI Setup : GPIO
	GPIO_SetupPinMux(SCIB_RXD, GPIO_MUX_CPU1, 3);
	GPIO_SetupPinOptions(SCIB_RXD, GPIO_INPUT, GPIO_PUSHPULL);
	GPIO_SetupPinMux(SCIB_TXD, GPIO_MUX_CPU1, 3);
	GPIO_SetupPinOptions(SCIB_TXD, GPIO_OUTPUT, GPIO_ASYNC);

	GPIO_SetupPinMux(SCIB_RTS_485, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(SCIB_RTS_485, GPIO_OUTPUT, GPIO_PUSHPULL);

	// SCI Setup : Clear Buffer
	Uint16 i;
	for( i=0; i<18; i++ )
	{
		SCIB_RxBuf[i] = 0;
	}

	// SCI Setup : FIFO
	ScibRegs.SCIFFTX.all = 0xC040;			//
	ScibRegs.SCIFFRX.all = 0x0021;			// RXFFIENA = 1, RXFFIL = 1 : Enable FIFO Rx INT, Rx Buf Length=1;
	//ScibRegs.SCIFFRX.all = 0x0044;		// My Old Version
	//ScibRegs.SCIFFTX.all = 0xC022;		// Demo Used
	//ScibRegs.SCIFFRX.all = 0x0022;		// Demo Used

	ScibRegs.SCIFFCT.all = 0x0;				// FIFO transfer delay -> 0

	// SCI Setup : General
	ScibRegs.SCICCR.all = 0x0007;			// 1 Stop-bit, No Loopback, No Parity, 8-Bit Char, Async Mode, Idle-line protocol
	ScibRegs.SCICTL1.all= 0x0003;			// TXENA = 1;  RXENA = 1;   Internal SCICLK, Disable RX ERR, SLEEP, TXWAKE

	// SCI Setup : Interrupt
	ScibRegs.SCICTL2.bit.RXBKINTENA = 1;	// 1 -> Enable RXRDY or BRKDT interrupt
	//ScibRegs.SCICTL2.bit.TXINTENA = 1;	// 1 -> Enable TXRDY interrupt

	// SCI Setup : BAUD
	//ScibRegs.SCIHBAUD.all = 0x0002;		// Baud-Rate = 9600		@ LSPCLK = 50 MHz (SYSCLK = 200 MHz)
	//ScibRegs.SCILBAUD.all = 0x008A;
	//ScibRegs.SCIHBAUD.all = 0x0001;		// Baud-Rate = 19200
	//ScibRegs.SCILBAUD.all = 0x0045;
	ScibRegs.SCIHBAUD.all = 0x0000;			// Baud-Rate = 115200
	ScibRegs.SCILBAUD.all = 0x0035;

	// SCI Setup : Reset
	ScibRegs.SCICTL1.all = 0x0023;			// Relinquish SCI from Reset
	ScibRegs.SCIFFTX.bit.TXFIFORESET = 1;	// Re-enable Transmit FIFO operation
	ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;	// Re-enable Receive FIFO operation
}
#endif

void usr_SCIB_485_En(void)
{
	GPIO_WritePin(SCIB_RTS_485, 1);					// OE1 Enabled
}
//--------------------------------------------------------------------------------
// Transmit Functions
//--------------------------------------------------------------------------------
void usr_SCIB_Xmit(int a)							// Transmit a character
{
    while (ScibRegs.SCIFFTX.bit.TXFFST > 0x0F) {}	// Wait a moment! If FIFO is Full up ( Level = 16 )
    ScibRegs.SCITXBUF.all = a;
}

void usr_SCIB_MsgPut(char * msg)					// Transmit a character string ( End with '\0' )
{
	int i = 0;

	while(msg[i] != '\0')							// The Output Frame Terminator : ASCII Code 0 NUL(null)
	{
		usr_SCIB_Xmit(msg[i]);
		i++;
	}
}

//--------------------------------------------------------------------------------
// ISR
//--------------------------------------------------------------------------------
__interrupt void ISR_SCIB_RX(void)
{
	Uint16 tmpData;

	tmpData = ScibRegs.SCIRXBUF.all;
	SCIB_RxBuf[SCIB_RxCnt] = tmpData & 0x00FF;

	if( SCIB_RxBuf[SCIB_RxCnt] == 10 )				// the Input Frame Terminator: 10->'\n'  or  13->'\r'
	{												// Windows Terminal Software : '\r'			1 Letter Only
		SCIB_RxCnt = 0;								// Arduino Serial.println()  : '\r'+'\n'	2 Letters
		SCIB_RxGet = 1;
	}
	else
	{
		SCIB_RxCnt += 1;
	}

	ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
	ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;			// Clear SCI Group flag
}

__interrupt void ISR_SCIB_TX(void)
{
	ScibRegs.SCIFFTX.bit.TXFFINTCLR = 1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;			// Clear SCI Group flag
}

//--------------------------------------------------------------------------------
// Monitor Response Function
//--------------------------------------------------------------------------------

// ASCII Code 48 -> Data 0
// ......
// ASCII Code 57 -> Data 9

// ASCII Code 65 -> A ( Uppercase Letter )
// ......
// ASCII Code 90 -> Z ( Uppercase Letter )

// ASCII Code 97 -> a ( Lowercase Letter )
// ......
// ASCII Code 122-> z ( Lowercase Letter )

void usr_SCIB_MonitorRes(void)
{
	Uint16	tmp_data;

	if( SCIB_RxBuf[0] == 81 )						// Instruction Q -> Query
	{
		if( SCIB_RxBuf[1] != 48 )
		{
			tmp_data = ( SCIB_RxBuf[1] - 48 ) * 10;
			tmp_data = tmp_data + ( SCIB_RxBuf[2] - 48 );
		}
		else
		{
			tmp_data = SCIB_RxBuf[2] - 48;
		}

		switch( tmp_data )
		{
			//-----------------------------------
            //      Send Internal Parameters
            //-----------------------------------
			case 1 :TmpArray_16[0] = 81;			// Q01
					TmpArray_16[1] = 48;
					TmpArray_16[2] = 49;
					Uint16_to_ASCII_4P(Bias_ADC_UAB, TmpArray_5);
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					Uint16_to_ASCII_4P(Bias_ADC_UBC, TmpArray_5);
					TmpArray_16[7] = TmpArray_5[0];
					TmpArray_16[8] = TmpArray_5[1];
					TmpArray_16[9] = TmpArray_5[2];
					TmpArray_16[10] = TmpArray_5[3];
					Uint16_to_ASCII_4P(Bias_ADC_UCA, TmpArray_5);
					TmpArray_16[11] = TmpArray_5[0];
					TmpArray_16[12] = TmpArray_5[1];
					TmpArray_16[13] = TmpArray_5[2];
					TmpArray_16[14] = TmpArray_5[3];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
			case 2 :TmpArray_16[0] = 81;			// Q02
					TmpArray_16[1] = 48;
					TmpArray_16[2] = 50;
					Uint16_to_ASCII_4P(Rate_RMS_UAB*10000, TmpArray_5);
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					Uint16_to_ASCII_4P(Rate_RMS_UBC*10000, TmpArray_5);
					TmpArray_16[7] = TmpArray_5[0];
					TmpArray_16[8] = TmpArray_5[1];
					TmpArray_16[9] = TmpArray_5[2];
					TmpArray_16[10] = TmpArray_5[3];
					Uint16_to_ASCII_4P(Rate_RMS_UCA*10000, TmpArray_5);
					TmpArray_16[11] = TmpArray_5[0];
					TmpArray_16[12] = TmpArray_5[1];
					TmpArray_16[13] = TmpArray_5[2];
					TmpArray_16[14] = TmpArray_5[3];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
			case 3 :TmpArray_16[0] = 81;			// Q03
					TmpArray_16[1] = 48;
					TmpArray_16[2] = 51;
					Uint16_to_ASCII_4P(Bias_ADC_IAB, TmpArray_5);
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					Uint16_to_ASCII_4P(Bias_ADC_IBC, TmpArray_5);
					TmpArray_16[7] = TmpArray_5[0];
					TmpArray_16[8] = TmpArray_5[1];
					TmpArray_16[9] = TmpArray_5[2];
					TmpArray_16[10] = TmpArray_5[3];
					Uint16_to_ASCII_4P(Bias_ADC_ICA, TmpArray_5);
					TmpArray_16[11] = TmpArray_5[0];
					TmpArray_16[12] = TmpArray_5[1];
					TmpArray_16[13] = TmpArray_5[2];
					TmpArray_16[14] = TmpArray_5[3];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
			case 4 :TmpArray_16[0] = 81;			// Q04
					TmpArray_16[1] = 48;
					TmpArray_16[2] = 52;
					Uint16_to_ASCII_4P(Rate_RMS_IAB*10000, TmpArray_5);
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					Uint16_to_ASCII_4P(Rate_RMS_IBC*10000, TmpArray_5);
					TmpArray_16[7] = TmpArray_5[0];
					TmpArray_16[8] = TmpArray_5[1];
					TmpArray_16[9] = TmpArray_5[2];
					TmpArray_16[10] = TmpArray_5[3];
					Uint16_to_ASCII_4P(Rate_RMS_ICA*10000, TmpArray_5);
					TmpArray_16[11] = TmpArray_5[0];
					TmpArray_16[12] = TmpArray_5[1];
					TmpArray_16[13] = TmpArray_5[2];
					TmpArray_16[14] = TmpArray_5[3];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
			case 5 :TmpArray_16[0] = 81;			// Q05
					TmpArray_16[1] = 48;
					TmpArray_16[2] = 53;
					Uint16_to_ASCII_4P(Slope_RMS_UDC*10000, TmpArray_5);
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					Uint16_to_ASCII_5P(Apart_RMS_UDC*100, TmpArray_6);
					TmpArray_16[7] = TmpArray_6[0];
					TmpArray_16[8] = TmpArray_6[1];
					TmpArray_16[9] = TmpArray_6[2];
					TmpArray_16[10] = TmpArray_6[3];
					TmpArray_16[11] = TmpArray_6[4];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
			case 6 :TmpArray_16[0] = 81;			// Q06
					TmpArray_16[1] = 48;
					TmpArray_16[2] = 54;
					Uint16_to_ASCII_4P(Slope_RMS_IDC*10000, TmpArray_5);
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					Uint16_to_ASCII_5P(Apart_RMS_IDC*100, TmpArray_6);
					TmpArray_16[7] = TmpArray_6[0];
					TmpArray_16[8] = TmpArray_6[1];
					TmpArray_16[9] = TmpArray_6[2];
					TmpArray_16[10] = TmpArray_6[3];
					TmpArray_16[11] = TmpArray_6[4];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
			//-----------------------------------
			//      Send External Parameters
			//-----------------------------------
			case 7 :TmpArray_16[0] = 81;			// Q07
					TmpArray_16[1] = 48;
					TmpArray_16[2] = 55;
					Uint16_to_ASCII_4P(UiA_Rms*10, TmpArray_5);
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					Uint16_to_ASCII_4P(UiB_Rms*10, TmpArray_5);
					TmpArray_16[7] = TmpArray_5[0];
					TmpArray_16[8] = TmpArray_5[1];
					TmpArray_16[9] = TmpArray_5[2];
					TmpArray_16[10] = TmpArray_5[3];
					Uint16_to_ASCII_4P(UiC_Rms*10, TmpArray_5);
					TmpArray_16[11] = TmpArray_5[0];
					TmpArray_16[12] = TmpArray_5[1];
					TmpArray_16[13] = TmpArray_5[2];
					TmpArray_16[14] = TmpArray_5[3];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
			case 8 :TmpArray_16[0] = 81;			// Q08
					TmpArray_16[1] = 48;
					TmpArray_16[2] = 56;
					Uint16_to_ASCII_4P(UiA_Frq*10, TmpArray_5);
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					Uint16_to_ASCII_4P(UiB_Frq*10, TmpArray_5);
					TmpArray_16[7] = TmpArray_5[0];
					TmpArray_16[8] = TmpArray_5[1];
					TmpArray_16[9] = TmpArray_5[2];
					TmpArray_16[10] = TmpArray_5[3];
					Uint16_to_ASCII_4P(UiC_Frq*10, TmpArray_5);
					TmpArray_16[11] = TmpArray_5[0];
					TmpArray_16[12] = TmpArray_5[1];
					TmpArray_16[13] = TmpArray_5[2];
					TmpArray_16[14] = TmpArray_5[3];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
			case 9 :TmpArray_16[0] = 81;			// Q09
					TmpArray_16[1] = 48;
					TmpArray_16[2] = 57;
					Uint16_to_ASCII_4P(IiA_Rms*10, TmpArray_5);
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					Uint16_to_ASCII_4P(IiB_Rms*10, TmpArray_5);
					TmpArray_16[7] = TmpArray_5[0];
					TmpArray_16[8] = TmpArray_5[1];
					TmpArray_16[9] = TmpArray_5[2];
					TmpArray_16[10] = TmpArray_5[3];
					Uint16_to_ASCII_4P(IiC_Rms*10, TmpArray_5);
					TmpArray_16[11] = TmpArray_5[0];
					TmpArray_16[12] = TmpArray_5[1];
					TmpArray_16[13] = TmpArray_5[2];
					TmpArray_16[14] = TmpArray_5[3];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
			case 10:TmpArray_16[0] = 81;			// Q10
					TmpArray_16[1] = 49;
					TmpArray_16[2] = 48;
					Uint16_to_ASCII_4P(Uo2*10, TmpArray_5);
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					Uint16_to_ASCII_4P(Iout*10, TmpArray_5);
					TmpArray_16[7] = TmpArray_5[0];
					TmpArray_16[8] = TmpArray_5[1];
					TmpArray_16[9] = TmpArray_5[2];
					TmpArray_16[10] = TmpArray_5[3];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
			case 11:TmpArray_16[0] = 81;			// Q11
					TmpArray_16[1] = 49;
					TmpArray_16[2] = 49;
					Uint16_to_ASCII_4P(WorkingTime_H, TmpArray_5);		// Time: Hour
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					Uint16_to_ASCII_4P(WorkingTime_M, TmpArray_5);		// Time: Minute
					TmpArray_16[7] = TmpArray_5[0];
					TmpArray_16[8] = TmpArray_5[1];
					TmpArray_16[9] = TmpArray_5[2];
					TmpArray_16[10] = TmpArray_5[3];
					Uint16_to_ASCII_4P(ePwm_DutyCycle, TmpArray_5);		// DutyCycle
					TmpArray_16[11] = TmpArray_5[0];
					TmpArray_16[12] = TmpArray_5[1];
					TmpArray_16[13] = TmpArray_5[2];
					TmpArray_16[14] = TmpArray_5[3];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
			case 12:TmpArray_16[0] = 81;			// Q12
					TmpArray_16[1] = 49;
					TmpArray_16[2] = 50;
					if( CapData_UiA_Error < 10000 )	{					// CapData_UiA_Error
						Uint16_to_ASCII_4P(CapData_UiA_Error, TmpArray_5);
					}
					else	{
						Uint16_to_ASCII_4P(9999, TmpArray_5);
					}
					TmpArray_16[3] = TmpArray_5[0];
					TmpArray_16[4] = TmpArray_5[1];
					TmpArray_16[5] = TmpArray_5[2];
					TmpArray_16[6] = TmpArray_5[3];
					if( CapData_UiB_Error < 10000 )	{					// CapData_UiB_Error
						Uint16_to_ASCII_4P(CapData_UiB_Error, TmpArray_5);
					}
					else	{
						Uint16_to_ASCII_4P(9999, TmpArray_5);
					}
					TmpArray_16[7] = TmpArray_5[0];
					TmpArray_16[8] = TmpArray_5[1];
					TmpArray_16[9] = TmpArray_5[2];
					TmpArray_16[10] = TmpArray_5[3];
					if( CapData_UiC_Error < 10000 )	{					// CapData_UiC_Error
						Uint16_to_ASCII_4P(CapData_UiC_Error, TmpArray_5);
					}
					else	{
						Uint16_to_ASCII_4P(9999, TmpArray_5);
					}
					TmpArray_16[11] = TmpArray_5[0];
					TmpArray_16[12] = TmpArray_5[1];
					TmpArray_16[13] = TmpArray_5[2];
					TmpArray_16[14] = TmpArray_5[3];
					usr_SCIB_MsgPut(TmpArray_16);
					break;
		}
	}
	else if( SCIB_RxBuf[0] == 83 )					// Instruction S -> Setup
	{
		Uint16	tmp_data1, tmp_data2, tmp_data3, tmp_data4;
		Uint32	tmp_data0;
		float	tmp_ans;

		SCIB_SetupGet = 1;

		switch( SCIB_RxBuf[1] )
		{
			case 49:tmp_data1 = SCIB_RxBuf[2]-48;
					tmp_data2 = SCIB_RxBuf[3]-48;
					tmp_data3 = SCIB_RxBuf[4]-48;
					tmp_data4 = SCIB_RxBuf[5]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					if( (tmp_ans>ADC_MIN_UAC_ZERO) && (tmp_ans<ADC_MAX_UAC_ZERO) )
					{
						Bias_ADC_UAB = tmp_ans;
					}
					tmp_data1 = SCIB_RxBuf[6]-48;
					tmp_data2 = SCIB_RxBuf[7]-48;
					tmp_data3 = SCIB_RxBuf[8]-48;
					tmp_data4 = SCIB_RxBuf[9]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					if( (tmp_ans>ADC_MIN_UAC_ZERO) && (tmp_ans<ADC_MAX_UAC_ZERO) )
					{
						Bias_ADC_UBC = tmp_ans;
					}
					tmp_data1 = SCIB_RxBuf[10]-48;
					tmp_data2 = SCIB_RxBuf[11]-48;
					tmp_data3 = SCIB_RxBuf[12]-48;
					tmp_data4 = SCIB_RxBuf[13]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					if( (tmp_ans>ADC_MIN_UAC_ZERO) && (tmp_ans<ADC_MAX_UAC_ZERO) )
					{
						Bias_ADC_UCA = tmp_ans;
					}
					break;
			case 50:tmp_data1 = SCIB_RxBuf[2]-48;
					tmp_data2 = SCIB_RxBuf[3]-48;
					tmp_data3 = SCIB_RxBuf[4]-48;
					tmp_data4 = SCIB_RxBuf[5]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					Rate_RMS_UAB = tmp_ans / 10000;
					tmp_data1 = SCIB_RxBuf[6]-48;
					tmp_data2 = SCIB_RxBuf[7]-48;
					tmp_data3 = SCIB_RxBuf[8]-48;
					tmp_data4 = SCIB_RxBuf[9]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					Rate_RMS_UBC = tmp_ans / 10000;
					tmp_data1 = SCIB_RxBuf[10]-48;
					tmp_data2 = SCIB_RxBuf[11]-48;
					tmp_data3 = SCIB_RxBuf[12]-48;
					tmp_data4 = SCIB_RxBuf[13]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					Rate_RMS_UCA = tmp_ans / 10000;
					break;
			case 51:tmp_data1 = SCIB_RxBuf[2]-48;
					tmp_data2 = SCIB_RxBuf[3]-48;
					tmp_data3 = SCIB_RxBuf[4]-48;
					tmp_data4 = SCIB_RxBuf[5]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					if( (tmp_ans>ADC_MIN_IAC_ZERO) && (tmp_ans<ADC_MAX_IAC_ZERO) )
					{
						Bias_ADC_IAB = tmp_ans;
					}
					tmp_data1 = SCIB_RxBuf[6]-48;
					tmp_data2 = SCIB_RxBuf[7]-48;
					tmp_data3 = SCIB_RxBuf[8]-48;
					tmp_data4 = SCIB_RxBuf[9]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					if( (tmp_ans>ADC_MIN_IAC_ZERO) && (tmp_ans<ADC_MAX_IAC_ZERO) )
					{
						Bias_ADC_IBC = tmp_ans;
					}
					tmp_data1 = SCIB_RxBuf[10]-48;
					tmp_data2 = SCIB_RxBuf[11]-48;
					tmp_data3 = SCIB_RxBuf[12]-48;
					tmp_data4 = SCIB_RxBuf[13]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					if( (tmp_ans>ADC_MIN_IAC_ZERO) && (tmp_ans<ADC_MAX_IAC_ZERO) )
					{
						Bias_ADC_ICA = tmp_ans;
					}
					break;
			case 52:tmp_data1 = SCIB_RxBuf[2]-48;
					tmp_data2 = SCIB_RxBuf[3]-48;
					tmp_data3 = SCIB_RxBuf[4]-48;
					tmp_data4 = SCIB_RxBuf[5]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					Rate_RMS_IAB = tmp_ans / 10000;
					tmp_data1 = SCIB_RxBuf[6]-48;
					tmp_data2 = SCIB_RxBuf[7]-48;
					tmp_data3 = SCIB_RxBuf[8]-48;
					tmp_data4 = SCIB_RxBuf[9]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					Rate_RMS_IBC = tmp_ans / 10000;
					tmp_data1 = SCIB_RxBuf[10]-48;
					tmp_data2 = SCIB_RxBuf[11]-48;
					tmp_data3 = SCIB_RxBuf[12]-48;
					tmp_data4 = SCIB_RxBuf[13]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					Rate_RMS_ICA = tmp_ans / 10000;
					break;
			case 53:tmp_data1 = SCIB_RxBuf[2]-48;
					tmp_data2 = SCIB_RxBuf[3]-48;
					tmp_data3 = SCIB_RxBuf[4]-48;
					tmp_data4 = SCIB_RxBuf[5]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					Slope_RMS_UDC = tmp_ans / 10000;
					tmp_data0 = SCIB_RxBuf[6]-48;
					tmp_data1 = SCIB_RxBuf[7]-48;
					tmp_data2 = SCIB_RxBuf[8]-48;
					tmp_data3 = SCIB_RxBuf[9]-48;
					tmp_data4 = SCIB_RxBuf[10]-48;
					tmp_ans = tmp_data0*10000 + tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					Apart_RMS_UDC = tmp_ans / 100;
					break;
			case 54:tmp_data1 = SCIB_RxBuf[2]-48;
					tmp_data2 = SCIB_RxBuf[3]-48;
					tmp_data3 = SCIB_RxBuf[4]-48;
					tmp_data4 = SCIB_RxBuf[5]-48;
					tmp_ans = tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					Slope_RMS_IDC = tmp_ans / 10000;
					tmp_data0 = SCIB_RxBuf[6]-48;
					tmp_data1 = SCIB_RxBuf[7]-48;
					tmp_data2 = SCIB_RxBuf[8]-48;
					tmp_data3 = SCIB_RxBuf[9]-48;
					tmp_data4 = SCIB_RxBuf[10]-48;
					tmp_ans = tmp_data0*10000 + tmp_data1*1000 + tmp_data2*100 + tmp_data3*10 + tmp_data4;
					Apart_RMS_IDC = tmp_ans / 100;
					break;
		}
	}
}

//--------------------------------------------------------------------------------
// These Functions Used Without Monitor
//--------------------------------------------------------------------------------
void usr_SCIB_InfoStart(void)						// Welcome information
{
	SCIB_msg = "\r\n\n\n Hello! Let's Start Working!\r\n\0";
	usr_SCIB_MsgPut(SCIB_msg);
}

void usr_SCIB_InfoDebug(void)						// Running information
{
	//------------------------------ 		// Working Time ( ** H, ** M, ** S )
	SCIB_msg = "\r\n Working Time : \0";
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

	//------------------------------ 		// AC Voltage : UiA, UiB, UiC
	SCIB_msg = "\r\n VacIn-AB  : \0";
	float_to_ASCII_3P1(UiA_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n VacIn-BC  : \0";
	float_to_ASCII_3P1(UiB_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n VacIn-CA  : \0";
	float_to_ASCII_3P1(UiC_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	//------------------------------ 		// AC Current : IiA, IiB, IiC
	SCIB_msg = "\r\n IacIn-A   : \0";
	float_to_ASCII_3P1(IiA_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n IacIn-B   : \0";
	float_to_ASCII_3P1(IiB_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n IacIn-C   : \0";
	float_to_ASCII_3P1(IiC_Rms, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	//------------------------------ 		// Frequency of AC Voltage : UiA, UiB UiC
	SCIB_msg = "\r\n FacIn-AB  : \0";
	float_to_ASCII_3P1(UiA_Frq, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n FacIn-BC  : \0";
	float_to_ASCII_3P1(UiB_Frq, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n FacIn-CA  : \0";
	float_to_ASCII_3P1(UiC_Frq, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	//------------------------------ 		// Frequency Error Times
	SCIB_msg = "\r\n Error_F-AB: \0";
	Uint32_to_ASCII_7P(CapData_UiA_Error, TmpArray_8);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_8);
	SCIB_msg = " Times. \0";
	usr_SCIB_MsgPut(SCIB_msg);

	SCIB_msg = "\r\n Error_F-BC: \0";
	Uint32_to_ASCII_7P(CapData_UiB_Error, TmpArray_8);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_8);
	SCIB_msg = " Times. \0";
	usr_SCIB_MsgPut(SCIB_msg);

	SCIB_msg = "\r\n Error_F-CA: \0";
	Uint32_to_ASCII_7P(CapData_UiC_Error, TmpArray_8);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_8);
	SCIB_msg = " Times. \0";
	usr_SCIB_MsgPut(SCIB_msg);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	//------------------------------ 		// Uo1, Uo2, Iout, DutyCycle
	/*
	SCIB_msg = "\r\nVDC-Uo1   : \0";
	float_to_ASCII_3P1(Uo1, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);
	*/

	SCIB_msg = "\r\n Udc-ADC   : \0";					// Uo2 : ADC
	Uint16_to_ASCII_4P(Uo2_ADC, TmpArray_5);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_5);

	SCIB_msg = "\r\n Udc-RMS   : \0";					// Uo2 : RMS
	float_to_ASCII_3P1(Uo2, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	SCIB_msg = "\r\n Idc-ADC   : \0";					// Iout : ADC
	Uint16_to_ASCII_4P(Iout_ADC, TmpArray_5);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_5);

	SCIB_msg = "\r\n Idc-RMS   : \0";					// Iout : RMS
	float_to_ASCII_3P1(Iout, TmpArray_6);
	usr_SCIB_MsgPut(SCIB_msg);
	usr_SCIB_MsgPut(TmpArray_6);

	SCIB_msg = "\r\n\0";					// Blank Line
	usr_SCIB_MsgPut(SCIB_msg);

	SCIB_msg = "\r\n DutyCycle : \0";					// Duty Cycle
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
