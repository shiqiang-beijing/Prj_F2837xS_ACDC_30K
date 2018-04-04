
//--------------------------------------------------------------------------------
#include	"F28x_Project.h"						// DSP28x Headerfile

//--------------------------------------------------------------------------------
#include	"usr_Global_Data.h"
#include	"usr_Global_Func.h"

#include	"usr_CpuTimers.h"
#include	"usr_Gpio.h"
#include	"usr_ADC.h"
#include	"usr_ePWM.h"
#include	"usr_eCAP.h"
#include	"usr_SCIB.h"

//--------------------------------------------------------------------------------
void main(void)
{
	Uint16	DelayCnt;

	//------------------------------ Initial CPU & Peripheral Clocks
	InitSysCtrl();									// Initial All Peripheral Clocks

	usr_InitSysCtrl();								// Close unUsed Peripheral Clocks

	DINT;

	InitPieCtrl();									// Initial Vector-Table

	IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();								// Setup PIE and ISR Table

	//------------------------------ User's ISR Replace the ISR name in F2837xS_pievect.h
	EALLOW;

	PieVectTable.TIMER0_INT = &ISR_CpuTimer0;		// CPU Timers
	PieVectTable.TIMER1_INT = &ISR_CpuTimer1;
	PieVectTable.TIMER2_INT = &ISR_CpuTimer2;

	PieVectTable.ADCA1_INT = &ISR_AdcA_INT1;		// ADC_A
	PieVectTable.ADCA2_INT = &ISR_AdcA_INT2;
	PieVectTable.ADCA3_INT = &ISR_AdcA_INT3;
	PieVectTable.ADCA4_INT = &ISR_AdcA_INT4;

	PieVectTable.ADCB1_INT = &ISR_AdcB_INT1;		// ADC_B
//	PieVectTable.ADCB2_INT = &ISR_AdcB_INT2;
//	PieVectTable.ADCB3_INT = &ISR_AdcB_INT3;
//	PieVectTable.ADCB4_INT = &ISR_AdcB_INT4;

	PieVectTable.ADCC1_INT = &ISR_AdcC_INT1;		// ADC_C
	PieVectTable.ADCC2_INT = &ISR_AdcC_INT2;
//	PieVectTable.ADCC3_INT = &ISR_AdcC_INT3;
//	PieVectTable.ADCC4_INT = &ISR_AdcC_INT4;

	PieVectTable.ADCD1_INT = &ISR_AdcD_INT1;		// ADC_D
	PieVectTable.ADCD2_INT = &ISR_AdcD_INT2;
//	PieVectTable.ADCD3_INT = &ISR_AdcD_INT3;
//	PieVectTable.ADCD4_INT = &ISR_AdcD_INT4;

	PieVectTable.EPWM1_INT = &ISR_ePWM1;			// ePWM
	PieVectTable.EPWM2_INT = &ISR_ePWM2;
	PieVectTable.EPWM3_INT = &ISR_ePWM3;

	PieVectTable.EPWM10_INT = &ISR_ePWM10;

	PieVectTable.ECAP1_INT = &ISR_eCAP1;			// eCAP
	PieVectTable.ECAP2_INT = &ISR_eCAP2;
	PieVectTable.ECAP3_INT = &ISR_eCAP3;

	EDIS;

	//------------------------------ Setup CPU Timers
	InitCpuTimers();								// Stop Counting	:	CpuTimerxRegs.TCR.bit.TSS = 1

	ConfigCpuTimer(&CpuTimer0, 200, 100);			// 100	us
	ConfigCpuTimer(&CpuTimer1, 200, 10000);			// 10	ms
	ConfigCpuTimer(&CpuTimer2, 200, 3000000);		// 3	 s

//	CpuTimer0Regs.TCR.all = 0x4000;					// Start  Counting	:	CpuTimerxRegs.TCR.bit.TSS = 0
	CpuTimer1Regs.TCR.all = 0x4000;					// Enable Interrupt	:	CpuTimerxRegs.TCR.bit.TIE = 1
	CpuTimer2Regs.TCR.all = 0x4000;

	//------------------------------ Setup GPIO
	InitGpio();										// input mode with pull-ups enabled
	usr_Gpio_Setup();
	usr_ePWM_Initial();								// Initial ePWM GPIO ; Initial ePWM Clock ( Divider and Enable ) ;
	usr_eCAP_Initial();								// Initial eCAP GPIO

	//------------------------------ Setup ADC ( Basic Parameters )
	usr_AdcA_Initial();
	usr_AdcB_Initial();
	usr_AdcC_Initial();
	usr_AdcD_Initial();

	//------------------------------ Setup SCI
//	usr_SCIA_Setup();
	usr_SCIB_Setup();

	//------------------------------ Setup ePWM
	EALLOW;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;			// STOP the Time-Base Clock of all ePWM Modules
	EDIS;

	usr_ePWM1_Setup();								// Used for IGBT Drive
	usr_ePWM2_Setup();
	usr_ePWM3_Setup();

	usr_ePWM7_Setup();								// Used for ADC Trigger
	usr_ePWM8_Setup();
	usr_ePWM9_Setup();

	usr_ePWM10_Setup();								// Used for Task Trigger

	EALLOW;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;			// START Counting of All ePWM Modules
	EDIS;

	//------------------------------ Setup eCAP
	usr_eCAP1_Setup();
	usr_eCAP2_Setup();
	usr_eCAP3_Setup();

	//------------------------------ Setup ADC ( Working Parameters )
	usr_AdcA_Setup();
	usr_AdcB_Setup();
	usr_AdcC_Setup();
	usr_AdcD_Setup();

	//------------------------------ CUP Interrupts
	IER |= M_INT1;									// Enable CPU INT1  for ADCx1, XINTx, TIMER0, WAKE
//	IER |= M_INT2;									// Enable CPU INT2  for ePWM1_TZ ~ ePWM12_TZ
	IER |= M_INT3;									// Enable CPU INT3  for ePWM1 ~ ePWM12
	IER |= M_INT4;									// Enable CPU INT4  for eCAP1 ~ eCAP6
	IER |= M_INT10;									// Enable CPU INT10 for ADCx2, ADCx3, ADCx4

	IER |= M_INT13;									// Enable CPU INT13 for CpuTimer1
	IER |= M_INT14;									// Enable CPU INT14 for CpuTimer2

	//------------------------------ PIE Interrupts
//	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;				// Enable TIMER0_INT

	PieCtrlRegs.PIEIER1.bit.INTx1 = 1;				// Enable ADCA1_INT in the PIE
	PieCtrlRegs.PIEIER1.bit.INTx2 = 1;				// Enable ADCB1_INT in the PIE
	PieCtrlRegs.PIEIER1.bit.INTx3 = 1;				// Enable ADCC1_INT in the PIE
	PieCtrlRegs.PIEIER1.bit.INTx6 = 1;				// Enable ADCD1_INT in the PIE

	PieCtrlRegs.PIEIER10.bit.INTx2 = 1;				// Enable ADCA2_INT in the PIE
	PieCtrlRegs.PIEIER10.bit.INTx3 = 1;				// Enable ADCA3_INT in the PIE
	PieCtrlRegs.PIEIER10.bit.INTx4 = 1;				// Enable ADCA4_INT in the PIE

//	PieCtrlRegs.PIEIER10.bit.INTx6 = 1;				// Enable ADCB2_INT in the PIE
//	PieCtrlRegs.PIEIER10.bit.INTx7 = 1;				// Enable ADCB3_INT in the PIE
//	PieCtrlRegs.PIEIER10.bit.INTx8 = 1;				// Enable ADCB4_INT in the PIE

	PieCtrlRegs.PIEIER10.bit.INTx10= 1;				// Enable ADCC2_INT in the PIE
//	PieCtrlRegs.PIEIER10.bit.INTx11= 1;				// Enable ADCC3_INT in the PIE
//	PieCtrlRegs.PIEIER10.bit.INTx12= 1;				// Enable ADCC4_INT in the PIE

	PieCtrlRegs.PIEIER10.bit.INTx14= 1;				// Enable ADCD2_INT in the PIE
//	PieCtrlRegs.PIEIER10.bit.INTx15= 1;				// Enable ADCD3_INT in the PIE
//	PieCtrlRegs.PIEIER10.bit.INTx16= 1;				// Enable ADCD4_INT in the PIE

//	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;				// Enable EPWM1_INT in the PIE
//	PieCtrlRegs.PIEIER3.bit.INTx2 = 1;				// Enable EPWM2_INT in the PIE
//	PieCtrlRegs.PIEIER3.bit.INTx3 = 1;				// Enable EPWM3_INT in the PIE

//	PieCtrlRegs.PIEIER3.bit.INTx7 = 1;				// Enable EPWM7_INT in the PIE
//	PieCtrlRegs.PIEIER3.bit.INTx8 = 1;				// Enable EPWM8_INT in the PIE
//	PieCtrlRegs.PIEIER3.bit.INTx9 = 1;				// Enable EPWM9_INT in the PIE

	PieCtrlRegs.PIEIER3.bit.INTx10 = 1;				// Enable EPWM10_INT in the PIE

	PieCtrlRegs.PIEIER4.bit.INTx1 = 1;				// Enable eCAP1_INT in the PIE
	PieCtrlRegs.PIEIER4.bit.INTx2 = 1;				// Enable eCAP2_INT in the PIE
	PieCtrlRegs.PIEIER4.bit.INTx3 = 1;				// Enable eCAP3_INT in the PIE

	//------------------------------
	EINT;											// Enable Global interrupt INTM
	ERTM;											// Enable Global realtime interrupt DBGM

	for( DelayCnt=0; DelayCnt<3; DelayCnt++ )
	{
		DELAY_US(1000);
	}

	//------------------------------ Start Working

	usr_Adc_Trigger_On();							// ADCs Working Start

//	usr_SCIA_485_En();								// 485 Enable

	SCIB_msg = "\r\n\n\nHello! Let's Start Working!\r\n\0";
	usr_SCIB_MsgPut(SCIB_msg);

	for(;;)
	{
		//------------------------------ Parameter Calculation ------------------------------

		//------------------------------ RMS of Uac ( UiA, UiB, UiC )
		if( AdcBuffer_UiA_Full )											// Sample Array Completed
		{
			UiA_Avg[i_UiA++] = RMS_IntArray(AdcResult_UiA, BUFFER_SIZE_UAC, Rate_RMS_UAB);
			AdcBuffer_UiA_Full = 0;
			if( i_UiA >= RMS_AVG_NUM_UAC )									// RMS Average Array Completed
			{
				UiA_Rms = Avg_Float(UiA_Avg, RMS_AVG_NUM_UAC);
				i_UiA = 0;
				UiA_RmsNew = 1;
			}
		}
		if( AdcBuffer_UiB_Full )
		{
			UiB_Avg[i_UiB++] = RMS_IntArray(AdcResult_UiB, BUFFER_SIZE_UAC, Rate_RMS_UBC);
			AdcBuffer_UiB_Full = 0;
			if( i_UiB >= RMS_AVG_NUM_UAC )
			{
				UiB_Rms = Avg_Float(UiB_Avg, RMS_AVG_NUM_UAC);
				i_UiB = 0;
				UiB_RmsNew = 1;
			}
		}
		if( AdcBuffer_UiC_Full )
		{
			UiC_Avg[i_UiC++] = RMS_IntArray(AdcResult_UiC, BUFFER_SIZE_UAC, Rate_RMS_UCA);
			AdcBuffer_UiC_Full = 0;
			if( i_UiC >= RMS_AVG_NUM_UAC )
			{
				UiC_Rms = Avg_Float(UiC_Avg, RMS_AVG_NUM_UAC);
				i_UiC = 0;
				UiC_RmsNew = 1;
			}
		}

		//------------------------------ RMS of Iac ( IiA, IiB, IiC )
		if( AdcBuffer_IiA_Full )											// Sample Array Completed
		{
			IiA_Avg[i_IiA++] = RMS_IntArray(AdcResult_IiA, BUFFER_SIZE_IAC, Rate_RMS_IAB);
			AdcBuffer_IiA_Full = 0;
			if( i_IiA >= RMS_AVG_NUM_IAC )
			{
				IiA_Rms = Avg_Float(IiA_Avg, RMS_AVG_NUM_IAC);
				i_IiA = 0;
			}
		}
		if( AdcBuffer_IiB_Full )
		{
			IiB_Avg[i_IiB++] = RMS_IntArray(AdcResult_IiB, BUFFER_SIZE_IAC, Rate_RMS_IBC);
			AdcBuffer_IiB_Full = 0;
			if( i_IiB >= RMS_AVG_NUM_IAC )
			{
				IiB_Rms = Avg_Float(IiB_Avg, RMS_AVG_NUM_IAC);
				i_IiB = 0;
			}
		}
		if( AdcBuffer_IiC_Full )
		{
			IiC_Avg[i_IiC++] = RMS_IntArray(AdcResult_IiC, BUFFER_SIZE_IAC, Rate_RMS_ICA);
			AdcBuffer_IiC_Full = 0;
			if( i_IiC >= RMS_AVG_NUM_IAC )
			{
				IiC_Rms = Avg_Float(IiC_Avg, RMS_AVG_NUM_IAC);
				i_IiC = 0;
			}
		}

		//------------------------------ RMS of Uo ( Uo1 Uo2 )
		if( AdcBuffer_Uo1_Full )
		{
			float	tmpData = 0;
			tmpData = Avg_Uint16(AdcResult_Uo1, BUFFER_SIZE_UDC);
//			Uo1 = ( tmpData / 4.955 ) - 139.21;
			Uo1 = ( tmpData * 0.2018 ) - 139.21;							// Use Multiplication take the place of Division
			AdcBuffer_Uo1_Full = 0;
		}
		if( AdcBuffer_Uo2_Full )
		{
			float	tmpData = 0;
			tmpData = Avg_Uint16(AdcResult_Uo2, BUFFER_SIZE_UDC);
//			Uo2 = ( tmpData / 4.955 ) - 139.21;
			Uo2 = ( tmpData * 0.2018 ) - 139.21;
			AdcBuffer_Uo2_Full = 0;
		}

		//------------------------------ RMS of Io
		if( AdcBuffer_Io_Full )
		{
			float	tmpData = 0;
			tmpData = Avg_Uint16(AdcResult_Io, BUFFER_SIZE_IDC);
			Iout = ( tmpData / 4.92 ) - 138.76;
			AdcBuffer_Io_Full = 0;
		}

		//------------------------------ Period / Frequency of Uac
		if( CapData_UiA_Full )
		{
			CapData_UiA_Mean = Avg_Uint32(CapData_UiA,BUFFER_SIZE_CAP);		// Ui Period : nS/5 ( CPU-Cycle )

			UiA_Prd = CapData_UiA_Mean / 200;								// Ui Period : uS
			UiA_Frq = 1000000 / UiA_Prd;									// Frequency : Hz

			CapData_UiA_Full = 0;
			CapMean_UiA_Renew = 1;
		}
		if( CapData_UiB_Full )
		{
			CapData_UiB_Mean = Avg_Uint32(CapData_UiB,BUFFER_SIZE_CAP);

			UiB_Prd = CapData_UiB_Mean / 200;
			UiB_Frq = 1000000 / UiB_Prd;

			CapData_UiB_Full = 0;
			CapMean_UiB_Renew = 1;
		}
		if( CapData_UiC_Full )
		{
			CapData_UiC_Mean = Avg_Uint32(CapData_UiC,BUFFER_SIZE_CAP);

			UiC_Prd = CapData_UiC_Mean / 200;
			UiC_Frq = 1000000 / UiC_Prd;

			CapData_UiC_Full = 0;
			CapMean_UiC_Renew = 1;
		}

		if( CapMean_UiA_Renew || CapMean_UiB_Renew || CapMean_UiC_Renew )	// Mean Value of Period/Frequency
		{
			CapData_Uin_Mean = ( CapData_UiA_Mean + CapData_UiB_Mean + CapData_UiC_Mean )/3;

			CapMean_UiA_Renew = 0;
			CapMean_UiB_Renew = 0;
			CapMean_UiC_Renew = 0;
		}

		//------------------------------ Power-In & Power-Out
		if( UiA_RmsNew && UiB_RmsNew && UiC_RmsNew )
		{
			Ui_Rms_Mean = ( UiA_Rms + UiB_Rms + UiC_Rms ) / 3.0;
			Ii_Rms_Mean = ( IiA_Rms + IiB_Rms + IiC_Rms ) / 3.0;
			Power_In = Ui_Rms_Mean * Ii_Rms_Mean * 1.732;
		}
		Power_Out = Uo2 * Iout;
		Power_Efficiency = Power_Out / Power_In * 100;


		//------------------------------ System Control ------------------------------

		//------------------------------ ePWM OutPut Enable
		#ifdef	EPWM_TEST_MODE
		usr_Gpio_OE1_En();
		#else
		if( (UiA_Rms>WORKING_VALUE_UAC) && (UiB_Rms>WORKING_VALUE_UAC) && (UiC_Rms>WORKING_VALUE_UAC) )
		{
			usr_Gpio_OE1_En();							// PWM Output Enable
		}
		else
		{
			usr_Gpio_OE1_Dis();							// PWM Output Disable
		}
		#endif

		//------------------------------ Adjust the Coefficient of Control 
		if( UiA_RmsNew && UiB_RmsNew && UiC_RmsNew )
		{
			//------ Mean Value : All Ui
//			Ui_Rms_Mean = ( UiA_Rms + UiB_Rms + UiC_Rms ) / 3.0;
//			Rate_UiA_Pwm = Rate_Uac_Pwm * ( Ui_Rms_Mean / UiA_Rms );
//			Rate_UiB_Pwm = Rate_Uac_Pwm * ( Ui_Rms_Mean / UiB_Rms );
//			Rate_UiC_Pwm = Rate_Uac_Pwm * ( Ui_Rms_Mean / UiC_Rms );
/*
			//------ Mean Value : Max and Min
			if( UiA_Rms < UiB_Rms )								// A < B
			{
				if( UiB_Rms < UiC_Rms )							// A < B < C
				{
					Rate_UiA_Pwm = Rate_Uac_Pwm * ( UiC_Rms / UiA_Rms );
					Rate_UiC_Pwm = Rate_Uac_Pwm * ( UiA_Rms / UiC_Rms );
				}
				else
				{
					if( UiA_Rms < UiC_Rms )						// A < C < B
					{
						Rate_UiA_Pwm = Rate_Uac_Pwm * ( UiB_Rms / UiA_Rms );
						Rate_UiB_Pwm = Rate_Uac_Pwm * ( UiA_Rms / UiB_Rms );
					}
					else										// C < A < B
					{
						Rate_UiC_Pwm = Rate_Uac_Pwm * ( UiB_Rms / UiC_Rms );
						Rate_UiB_Pwm = Rate_Uac_Pwm * ( UiC_Rms / UiB_Rms );
					}
				}
			}
			else												// B <= A
			{
				if( UiA_Rms < UiC_Rms )							// B < A < C
				{
					Rate_UiB_Pwm = Rate_Uac_Pwm * ( UiC_Rms / UiB_Rms );
					Rate_UiC_Pwm = Rate_Uac_Pwm * ( UiB_Rms / UiC_Rms );
				}
				else
				{
					if( UiB_Rms < UiC_Rms )						// B < C < A
					{
						Rate_UiB_Pwm = Rate_Uac_Pwm * ( UiA_Rms / UiB_Rms );
						Rate_UiA_Pwm = Rate_Uac_Pwm * ( UiB_Rms / UiA_Rms );
					}
					else										// C < B < A
					{
						Rate_UiC_Pwm = Rate_Uac_Pwm * ( UiA_Rms / UiC_Rms );
						Rate_UiA_Pwm = Rate_Uac_Pwm * ( UiC_Rms / UiA_Rms );
					}
				}
			}
*/
			UiA_RmsNew = 0;
			UiB_RmsNew = 0;
			UiC_RmsNew = 0;
		}


		//------------------------------ SciA
		if( En_SciB_Send != 0 )							// Sign of SCI-A
		{
			usr_SCIB_DebugInfo();
			En_SciB_Send = 0;
		}

		//------------------------------ Buzzer
		if( En_Buzzer != 0 )
		{
			GPIO_WritePin(GPIO_BUZZER, 1);
		}
		else
		{
			GPIO_WritePin(GPIO_BUZZER, 0);
		}


	}
}
