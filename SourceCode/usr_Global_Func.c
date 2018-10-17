
//--------------------------------------------------------------------------------
#include "F28x_Project.h"

//--------------------------------------------------------------------------------
#include	"usr_Global_Data.h"
#include	"usr_ePWM.h"

#include	<math.h>

//--------------------------------------------------------------------------------
#ifdef	_FLASH
//
#ifdef	POWER_MODE_PARALLEL
#pragma CODE_SECTION(ePwm_width_P, ".TI.ramfunc");		// Output Stage Parallel Connection
#else
#pragma CODE_SECTION(ePwm_width_S, ".TI.ramfunc");		// Output Stage Serial Connection
#endif
//
#endif


//--------------------------------------------------------------------------------
// Close Not Used Peripheral
//--------------------------------------------------------------------------------
void usr_InitSysCtrl(void)
{
	EALLOW;

//	CpuSysRegs.PCLKCR0.bit.CLA1 = 1;
//	CpuSysRegs.PCLKCR0.bit.DMA = 1;

//	CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = 1;
//	CpuSysRegs.PCLKCR0.bit.CPUTIMER1 = 1;
//	CpuSysRegs.PCLKCR0.bit.CPUTIMER2 = 1;

//	CpuSysRegs.PCLKCR0.bit.HRPWM = 1;
//	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;		// Globally Synchronize All Enabled ePWM Modules

	CpuSysRegs.PCLKCR1.bit.EMIF1 = 0;
	CpuSysRegs.PCLKCR1.bit.EMIF2 = 0;

//	CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
//	CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
//	CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;
//	CpuSysRegs.PCLKCR2.bit.EPWM4 = 1;
//	CpuSysRegs.PCLKCR2.bit.EPWM5 = 1;
//	CpuSysRegs.PCLKCR2.bit.EPWM6 = 1;
//	CpuSysRegs.PCLKCR2.bit.EPWM7 = 1;
//	CpuSysRegs.PCLKCR2.bit.EPWM8 = 1;
//	CpuSysRegs.PCLKCR2.bit.EPWM9 = 1;
//	CpuSysRegs.PCLKCR2.bit.EPWM10 = 1;
//	CpuSysRegs.PCLKCR2.bit.EPWM11 = 1;
//	CpuSysRegs.PCLKCR2.bit.EPWM12 = 1;

//	CpuSysRegs.PCLKCR3.bit.ECAP1 = 1;
//	CpuSysRegs.PCLKCR3.bit.ECAP2 = 1;
//	CpuSysRegs.PCLKCR3.bit.ECAP3 = 1;
	CpuSysRegs.PCLKCR3.bit.ECAP4 = 0;
	CpuSysRegs.PCLKCR3.bit.ECAP5 = 0;
	CpuSysRegs.PCLKCR3.bit.ECAP6 = 0;

	CpuSysRegs.PCLKCR4.bit.EQEP1 = 0;
	CpuSysRegs.PCLKCR4.bit.EQEP2 = 0;
	CpuSysRegs.PCLKCR4.bit.EQEP3 = 0;

	CpuSysRegs.PCLKCR6.bit.SD1 = 0;
	CpuSysRegs.PCLKCR6.bit.SD2 = 0;

//	CpuSysRegs.PCLKCR7.bit.SCI_A = 1;
//	CpuSysRegs.PCLKCR7.bit.SCI_B = 1;
	CpuSysRegs.PCLKCR7.bit.SCI_C = 0;
	CpuSysRegs.PCLKCR7.bit.SCI_D = 0;

	CpuSysRegs.PCLKCR8.bit.SPI_A = 0;
	CpuSysRegs.PCLKCR8.bit.SPI_B = 0;
	CpuSysRegs.PCLKCR8.bit.SPI_C = 0;

	CpuSysRegs.PCLKCR9.bit.I2C_A = 0;
	CpuSysRegs.PCLKCR9.bit.I2C_B = 0;

//	CpuSysRegs.PCLKCR10.bit.CAN_A = 1;
//	CpuSysRegs.PCLKCR10.bit.CAN_B = 1;

	CpuSysRegs.PCLKCR11.bit.McBSP_A = 0;
	CpuSysRegs.PCLKCR11.bit.McBSP_B = 0;
	CpuSysRegs.PCLKCR11.bit.USB_A = 0;

//	CpuSysRegs.PCLKCR12.bit.uPP_A = 1;

//	CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
//	CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
//	CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
//	CpuSysRegs.PCLKCR13.bit.ADC_D = 1;

//	CpuSysRegs.PCLKCR14.bit.CMPSS1 = 1;
//	CpuSysRegs.PCLKCR14.bit.CMPSS2 = 1;
//	CpuSysRegs.PCLKCR14.bit.CMPSS3 = 1;
//	CpuSysRegs.PCLKCR14.bit.CMPSS4 = 1;
//	CpuSysRegs.PCLKCR14.bit.CMPSS5 = 1;
//	CpuSysRegs.PCLKCR14.bit.CMPSS6 = 1;
//	CpuSysRegs.PCLKCR14.bit.CMPSS7 = 1;
//	CpuSysRegs.PCLKCR14.bit.CMPSS8 = 1;

//	CpuSysRegs.PCLKCR16.bit.DAC_A = 1;
//	CpuSysRegs.PCLKCR16.bit.DAC_B = 1;
//	CpuSysRegs.PCLKCR16.bit.DAC_C = 1;

    EDIS;
}

//--------------------------------------------------------------------------------
// Convert Functions
//--------------------------------------------------------------------------------

// Uint16 to ASCII Array		Format: XX
//
void Uint16_to_ASCII_2P(Uint16 num, char *p)
{
	Uint16 tmpf, tmpi;

	tmpf = num;

	*(p++) = tmpf/10 + 0x30;
	tmpi = tmpf % 10;

	*(p) = tmpi + 0x30;
}

// Uint16 to ASCII Array		Format: XXXX
//
void Uint16_to_ASCII_4P(Uint16 num, char *p)
{
	Uint16 tmpf, tmpi;

	tmpf = num;

	*(p++) = tmpf/1000 + 0x30;
	tmpi = tmpf % 1000;

	*(p++) = tmpi/100 + 0x30;
	tmpi %= 100;

	*(p++) = tmpi/10 + 0x30;
	tmpi %= 10;

	*(p++) = tmpi + 0x30;
}

// Uint16 to ASCII Array		Format: XXXXX
//
void Uint16_to_ASCII_5P(Uint16 num, char *p)
{
	Uint16 tmpf, tmpi;

	tmpf = num;

	*(p++) = tmpf/10000 + 0x30;
	tmpi = tmpf % 10000;

	*(p++) = tmpi/1000 + 0x30;
	tmpi %= 1000;

	*(p++) = tmpi/100 + 0x30;
	tmpi %= 100;

	*(p++) = tmpi/10 + 0x30;
	tmpi %= 10;

	*(p++) = tmpi + 0x30;
}

// Uint16 to ASCII Array		Format: XXXXXXX
//
void Uint32_to_ASCII_7P(Uint32 num, char *p)
{
	Uint32 tmpf, tmpi;

	tmpf = num;

	*(p++) = tmpf/1000000 + 0x30;
	tmpi = tmpf % 1000000;

	*(p++) = tmpi/100000 + 0x30;
	tmpi %= 100000;

	*(p++) = tmpi/10000 + 0x30;
	tmpi %= 10000;

	*(p++) = tmpi/1000 + 0x30;
	tmpi %= 1000;

	*(p++) = tmpi/100 + 0x30;
	tmpi %= 100;

	*(p++) = tmpi/10 + 0x30;
	tmpi %= 10;

	*(p++) = tmpi + 0x30;
}

// Float to ASCII Array			Format: XXX.X
//
void float_to_ASCII_3P1(float num, char *p)
{
	Uint16 tmpf, tmpi;

	tmpf = (Uint16)(10*num);

	*(p++) = tmpf/1000 + 0x30;
	tmpi = tmpf % 1000;

	*(p++) = tmpi/100 + 0x30;
	tmpi %= 100;

	*(p++) = tmpi/10 + 0x30;
	tmpi %= 10;

	*(p++) = 0x2E;

	*(p) = tmpi + 0x30;
}

// Float to ASCII Array			Format: XXXXX.X
//
void float_to_ASCII_5P1(float num, char *p)
{
	Uint16 tmpf, tmpi;

	tmpf = (Uint16)(10*num);

	*(p++) = tmpf/100000 + 0x30;
	tmpi = tmpf % 100000;

	*(p++) = tmpi/10000 + 0x30;
	tmpi %= 10000;

	*(p++) = tmpi/1000 + 0x30;
	tmpi %= 1000;

	*(p++) = tmpi/100 + 0x30;
	tmpi %= 100;

	*(p++) = tmpi/10 + 0x30;
	tmpi %= 10;

	*(p++) = 0x2E;

	*(p) = tmpi + 0x30;
}

//--------------------------------------------------------------------------------
// Calculate RMS Value from an Int Array
//--------------------------------------------------------------------------------
float RMS_IntArray(volatile Uint16 *SampleArry, Uint16 ArrayNumber, float Rate_RMS)
{
	Uint16	i;
	Uint32	tmpSQ = 0;
	Uint64	tmpACC = 0;
	float	tmpRMS = 0;

	#ifdef	VCM_TEST_MODE

	for(i=0; i<ArrayNumber; i++)							// Average Value
	{
		tmpACC += SampleArry[i];
	}
	tmpRMS = tmpACC/ArrayNumber;

	#else

	for(i=0; i<ArrayNumber; i++)							// RMS Value
	{
		tmpSQ   = SampleArry[i];
		tmpACC += tmpSQ * tmpSQ;
	}
	tmpRMS = tmpACC;
	tmpRMS = sqrt( tmpRMS/ArrayNumber ) * Rate_RMS;

	#endif

	return	tmpRMS;
}

//--------------------------------------------------------------------------------
// Calculate Average Value of a Float Array
//--------------------------------------------------------------------------------
float Avg_Float(float *DataArry, Uint16 AvgTimes)
{
	Uint16	i;
	float	tmpACC = 0;
	float	tmpRMS = 0;

	for(i=0; i<AvgTimes; i++)
	{
		tmpACC += DataArry[i];
	}
	tmpRMS = tmpACC / AvgTimes;

	return	tmpRMS;
}

//--------------------------------------------------------------------------------
// Calculate Average Value of a Uint16 Array
//--------------------------------------------------------------------------------
Uint16 Avg_Uint16(volatile Uint16 *DataArry, Uint16 AvgTimes)
{
	Uint16	i;
	Uint32	tmpACC = 0;
	Uint16	tmpRMS = 0;

	for(i=0; i<AvgTimes; i++)
	{
		tmpACC += DataArry[i];
	}
	tmpRMS = tmpACC / AvgTimes;

	return	tmpRMS;
}

//--------------------------------------------------------------------------------
// Calculate Average Value of a Uint32 Array
//--------------------------------------------------------------------------------
Uint32 Avg_Uint32(volatile Uint32 *DataArry, Uint16 AvgTimes)
{
	Uint16	i;
	Uint32	tmpACC = 0;
	Uint32	tmpRMS = 0;

	for(i=0; i<AvgTimes; i++)
	{
		tmpACC += DataArry[i];
	}
	tmpRMS = tmpACC / AvgTimes;

	return	tmpRMS;
}

//--------------------------------------------------------------------------------
// Output Order Changeless
//--------------------------------------------------------------------------------
void ePwm_width_O(void)										// Run Time in RAM ( CPU_Cycle ) :  Uint16: 162/180		Uint32: 158/182
{
	Uint32	ePwm_On_A = 0, ePwm_Off_A = 0;
	Uint32	ePwm_On_B = 0, ePwm_Off_B = 0;
	Uint32	ePwm_On_C = 0, ePwm_Off_C = 0;

	//------------------------------
	ePwm_Width_A = UiA_Trans * Rate_UiA_Pwm;
	ePwm_Width_B = UiB_Trans * Rate_UiB_Pwm;
	ePwm_Width_C = UiC_Trans * Rate_UiC_Pwm;

	//------------------------------
	/*
	if( ePwm_Width_A < ePwm_Width_Min )
	{
		ePwm_Width_A = 0;
	}
	if( ePwm_Width_B < ePwm_Width_Min )
	{
		ePwm_Width_B = 0;
	}
	if( ePwm_Width_C < ePwm_Width_Min )
	{
		ePwm_Width_C = 0;
	}

	if( ePwm_Width_A > ePwm_Width_Max )
	{
		ePwm_Width_A = ePwm_Width_Max;
	}
	if( ePwm_Width_B > ePwm_Width_Max )
	{
		ePwm_Width_B = ePwm_Width_Max;
	}
	if( ePwm_Width_C > ePwm_Width_Max )
	{
		ePwm_Width_C = ePwm_Width_Max;
	}
	*/

	//------------------------------
	ePwm_On_A  = EPWM_CMP_START;
	ePwm_Off_A = ePwm_On_A + ePwm_Width_A + EPWM_CMP_OVERLAY;

	ePwm_On_B  = ePwm_On_A + ePwm_Width_A;
	ePwm_Off_B = ePwm_On_B + ePwm_Width_B + EPWM_CMP_OVERLAY;

	ePwm_On_C  = ePwm_On_B + ePwm_Width_B;
	ePwm_Off_C = ePwm_On_C + ePwm_Width_C;

	//------------------------------

	EPwm1Regs.CMPA.bit.CMPA = ePwm_On_A;
	EPwm1Regs.CMPB.bit.CMPB = ePwm_Off_A;

	EPwm2Regs.CMPA.bit.CMPA = ePwm_On_B;
	EPwm2Regs.CMPB.bit.CMPB = ePwm_Off_B;

	EPwm3Regs.CMPA.bit.CMPA = ePwm_On_C;
	EPwm3Regs.CMPB.bit.CMPB = ePwm_Off_C;

	EALLOW;
	EPwm1Regs.GLDCTL2.bit.GFRCLD = 1;						// Software Force One Global Load Event
	EDIS;
}

//--------------------------------------------------------------------------------
// Compare Method 1
//--------------------------------------------------------------------------------
void ePwm_width_1(void)										// Run Time in RAM ( CPU_Cycle ): Uint16: 170 ~ 175
{
	ePwm_Width_A = UiA_Trans * Rate_UiA_Pwm;
	ePwm_Width_B = UiB_Trans * Rate_UiB_Pwm;
	ePwm_Width_C = UiC_Trans * Rate_UiC_Pwm;

	if( ( ePwm_Width_A < ePwm_Width_B ) && ( ePwm_Width_A < ePwm_Width_C ) )		// Min -> UiA
	{
		// Min Limit -> A

		EPwm6Regs.CMPA.bit.CMPA = EPWM_CMP_START;
		EPwm6Regs.CMPB.bit.CMPB = EPWM_CMP_START + ePwm_Width_A;

		if( ePwm_Width_B < ePwm_Width_C )											// UiA < UiB < UiC
		{
			EPwm7Regs.CMPA.bit.CMPA = EPwm6Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm7Regs.CMPB.bit.CMPB = EPwm7Regs.CMPA.bit.CMPA + ePwm_Width_B;

			if( ePwm_Width_C > ePwm_Width_Max )										// Max -> UiC
			{
				ePwm_Width_C = ePwm_Width_Max;
			}
			EPwm8Regs.CMPA.bit.CMPA = EPwm7Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm8Regs.CMPB.bit.CMPB = EPwm8Regs.CMPA.bit.CMPA + ePwm_Width_C;
		}
		else																		// UiA < UiC < UiB
		{
			EPwm8Regs.CMPA.bit.CMPA = EPwm6Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm8Regs.CMPB.bit.CMPB = EPwm8Regs.CMPA.bit.CMPA + ePwm_Width_C;

			if( ePwm_Width_B > ePwm_Width_Max )										// Max -> UiB
			{
				ePwm_Width_B = ePwm_Width_Max;
			}
			EPwm7Regs.CMPA.bit.CMPA = EPwm8Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm7Regs.CMPB.bit.CMPB = EPwm7Regs.CMPA.bit.CMPA + ePwm_Width_B;
		}
	}
	else if( ( ePwm_Width_B < ePwm_Width_A ) && ( ePwm_Width_B < ePwm_Width_C ) )	// Min -> UiB
	{
		// Min Limit -> B

		EPwm7Regs.CMPA.bit.CMPA = EPWM_CMP_START;
		EPwm7Regs.CMPB.bit.CMPB = EPWM_CMP_START + ePwm_Width_B;

		if( ePwm_Width_A < ePwm_Width_C )											// UiB < UiA < UiC
		{
			EPwm6Regs.CMPA.bit.CMPA = EPwm7Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm6Regs.CMPB.bit.CMPB = EPwm6Regs.CMPA.bit.CMPA + ePwm_Width_A;

			if( ePwm_Width_C > ePwm_Width_Max )										// Max -> UiC
			{
				ePwm_Width_C = ePwm_Width_Max;
			}
			EPwm8Regs.CMPA.bit.CMPA = EPwm6Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm8Regs.CMPB.bit.CMPB = EPwm8Regs.CMPA.bit.CMPA + ePwm_Width_C;
		}
		else																		// UiB < UiC < UiA
		{
			EPwm8Regs.CMPA.bit.CMPA = EPwm7Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm8Regs.CMPB.bit.CMPB = EPwm8Regs.CMPA.bit.CMPA + ePwm_Width_C;

			if( ePwm_Width_A > ePwm_Width_Max )										// Max -> UiA
			{
				ePwm_Width_A = ePwm_Width_Max;
			}
			EPwm6Regs.CMPA.bit.CMPA = EPwm8Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm6Regs.CMPB.bit.CMPB = EPwm6Regs.CMPA.bit.CMPA + ePwm_Width_A;
		}
	}
	else																			// Min -> UiC
	{
		// Min Limit -> C

		EPwm8Regs.CMPA.bit.CMPA = EPWM_CMP_START;
		EPwm8Regs.CMPB.bit.CMPB = EPWM_CMP_START + ePwm_Width_C;

		if( ePwm_Width_A < ePwm_Width_B )											// UiC < UiA < UiB
		{
			EPwm6Regs.CMPA.bit.CMPA = EPwm8Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm6Regs.CMPB.bit.CMPB = EPwm6Regs.CMPA.bit.CMPA + ePwm_Width_A;

			if( ePwm_Width_B > ePwm_Width_Max )										// Max -> UiB
			{
				ePwm_Width_B = ePwm_Width_Max;
			}
			EPwm7Regs.CMPA.bit.CMPA = EPwm6Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm7Regs.CMPB.bit.CMPB = EPwm7Regs.CMPA.bit.CMPA + ePwm_Width_B;
		}
		else																		// UiC < UiB < UiA
		{
			EPwm7Regs.CMPA.bit.CMPA = EPwm8Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm7Regs.CMPB.bit.CMPB = EPwm7Regs.CMPA.bit.CMPA + ePwm_Width_B;

			if( ePwm_Width_A > ePwm_Width_Max )										// Max -> UiA
			{
				ePwm_Width_A = ePwm_Width_Max;
			}
			EPwm6Regs.CMPA.bit.CMPA = EPwm7Regs.CMPB.bit.CMPB - EPWM_CMP_OVERLAY;
			EPwm6Regs.CMPB.bit.CMPB = EPwm6Regs.CMPA.bit.CMPA + ePwm_Width_A;
		}
	}
}

//--------------------------------------------------------------------------------
// Compare Method 2 ( Output Stage Parallel Connection : Stable )
//--------------------------------------------------------------------------------
void ePwm_width_P(void)										// Run Time in RAM ( CPU_Cycle ): 131 ( No  Min Width_Limit )
{															// Run Time in RAM ( CPU_Cycle ): 138 ( Add Min Width_Limit )
	Uint16	ePwm_On_A = 0, ePwm_Off_A = 0;					// Run Time in RAM ( CPU_Cycle ): 185 ( Add Max Width_Limit )
	Uint16	ePwm_On_B = 0, ePwm_Off_B = 0;					// Run Time in RAM ( CPU_Cycle ): 190 ( Add Min and Max Width_Limit )
	Uint16	ePwm_On_C = 0, ePwm_Off_C = 0;

	float	Width_Accumulator = 0;
	float	Width_RateAdj = 1;

	ePwm_Width_A = UiA_Trans * Rate_UiA_Pwm;
	ePwm_Width_B = UiB_Trans * Rate_UiB_Pwm;
	ePwm_Width_C = UiC_Trans * Rate_UiC_Pwm;

	Width_Accumulator = ePwm_Width_A + ePwm_Width_B + ePwm_Width_C;

	ePwm_Width_Arry[ePwm_Width_Index++] = Width_Accumulator;
	if( ePwm_Width_Index >= WDH_AVG_NUM_PWM )
	{
		ePwm_Width_Index = 0;
		ePwm_Width_Full = 1;
	}

	if( Width_Accumulator > ePwm_Width_Max )				// Max ePwm_Width Limit
	{
		Width_RateAdj = ePwm_Width_Max / Width_Accumulator;
		ePwm_Width_A = Width_RateAdj * ePwm_Width_A;
		ePwm_Width_B = Width_RateAdj * ePwm_Width_B;
		ePwm_Width_C = Width_RateAdj * ePwm_Width_C;
	}

	//------------------------------

	if( ePwm_Width_A < ePwm_Width_B )						// A < B
	{
		if( ePwm_Width_B < ePwm_Width_C )									// A < B < C	( Section 6 )  A-D B-U C-D
		{
			if( ePwm_Width_A < ePwm_Width_Min )								// A Too Small ?
			{
				ePwm_On_A  = EPWM_CMP_START;
				ePwm_Off_A = EPWM_CMP_START;

				ePwm_On_B  = EPWM_CMP_START;
				ePwm_Off_B = ePwm_On_B + ePwm_Width_B;

				ePwm_On_C  = ePwm_Off_B - EPWM_CMP_OVERLAY;
				ePwm_Off_C = ePwm_On_C + ePwm_Width_C;
			}
			else
			{
				ePwm_On_A  = EPWM_CMP_START;
				ePwm_Off_A = ePwm_On_A + ePwm_Width_A;

				ePwm_On_B  = ePwm_Off_A - EPWM_CMP_OVERLAY;
				ePwm_Off_B = ePwm_On_B + ePwm_Width_B;

				ePwm_On_C  = ePwm_Off_B - EPWM_CMP_OVERLAY;
				ePwm_Off_C = ePwm_On_C + ePwm_Width_C;
			}
		}
		else
		{
			if( ePwm_Width_A < ePwm_Width_C)								// A < C < B	( Section 1 )  A-U C-D B-U
			{
				if( ePwm_Width_A < ePwm_Width_Min )							// A Too Small ?
				{
					ePwm_On_A  = EPWM_CMP_START;
					ePwm_Off_A = EPWM_CMP_START;

					ePwm_On_C  = EPWM_CMP_START;
					ePwm_Off_C = ePwm_On_C + ePwm_Width_C;

					ePwm_On_B  = ePwm_Off_C - EPWM_CMP_OVERLAY;
					ePwm_Off_B = ePwm_On_B + ePwm_Width_B;
				}
				else
				{
					ePwm_On_A  = EPWM_CMP_START;
					ePwm_Off_A = ePwm_On_A + ePwm_Width_A;

					ePwm_On_C  = ePwm_Off_A - EPWM_CMP_OVERLAY;
					ePwm_Off_C = ePwm_On_C + ePwm_Width_C;

					ePwm_On_B  = ePwm_Off_C - EPWM_CMP_OVERLAY;
					ePwm_Off_B = ePwm_On_B + ePwm_Width_B;
				}
			}
			else															// C < A < B	( Section 2 )  C-D A-U B-D
			{
				if( ePwm_Width_C < ePwm_Width_Min )							// C Too Small ?
				{
					ePwm_On_C  = EPWM_CMP_START;
					ePwm_Off_C = EPWM_CMP_START;

					ePwm_On_A  = EPWM_CMP_START;
					ePwm_Off_A = ePwm_On_A + ePwm_Width_A;

					ePwm_On_B  = ePwm_Off_A - EPWM_CMP_OVERLAY;
					ePwm_Off_B = ePwm_On_B + ePwm_Width_B;
				}
				else
				{
					ePwm_On_C  = EPWM_CMP_START;
					ePwm_Off_C = ePwm_On_C + ePwm_Width_C;

					ePwm_On_A  = ePwm_Off_C - EPWM_CMP_OVERLAY;
					ePwm_Off_A = ePwm_On_A + ePwm_Width_A;

					ePwm_On_B  = ePwm_Off_A - EPWM_CMP_OVERLAY;
					ePwm_Off_B = ePwm_On_B + ePwm_Width_B;
				}
			}
		}
	}
	else													// B <= A
	{
		if( ePwm_Width_A < ePwm_Width_C )									// B < A < C	( Section 5 )  B-U A-D C-U
		{
			if( ePwm_Width_B < ePwm_Width_Min )								// B Too Small ?
			{
				ePwm_On_B  = EPWM_CMP_START;
				ePwm_Off_B = EPWM_CMP_START;

				ePwm_On_A  = EPWM_CMP_START;
				ePwm_Off_A = ePwm_On_A + ePwm_Width_A;

				ePwm_On_C  = ePwm_Off_A - EPWM_CMP_OVERLAY;
				ePwm_Off_C = ePwm_On_C + ePwm_Width_C;
			}
			else
			{
				ePwm_On_B  = EPWM_CMP_START;
				ePwm_Off_B = ePwm_On_B + ePwm_Width_B;

				ePwm_On_A  = ePwm_Off_B - EPWM_CMP_OVERLAY;
				ePwm_Off_A = ePwm_On_A + ePwm_Width_A;

				ePwm_On_C  = ePwm_Off_A - EPWM_CMP_OVERLAY;
				ePwm_Off_C = ePwm_On_C + ePwm_Width_C;
			}
		}
		else
		{
			if( ePwm_Width_B < ePwm_Width_C )								// B < C < A	( Section 4)  B-D C-U A-D
			{
				if( ePwm_Width_B < ePwm_Width_Min )							// B Too Small ?
				{
					ePwm_On_B  = EPWM_CMP_START;
					ePwm_Off_B = EPWM_CMP_START;

					ePwm_On_C  = EPWM_CMP_START;
					ePwm_Off_C = ePwm_On_C + ePwm_Width_C;

					ePwm_On_A  = ePwm_Off_C - EPWM_CMP_OVERLAY;
					ePwm_Off_A = ePwm_On_A + ePwm_Width_A;
				}
				else
				{
					ePwm_On_B  = EPWM_CMP_START;
					ePwm_Off_B = ePwm_On_B + ePwm_Width_B;

					ePwm_On_C  = ePwm_Off_B - EPWM_CMP_OVERLAY;
					ePwm_Off_C = ePwm_On_C + ePwm_Width_C;

					ePwm_On_A  = ePwm_Off_C - EPWM_CMP_OVERLAY;
					ePwm_Off_A = ePwm_On_A + ePwm_Width_A;
				}
			}
			else															// C < B < A	( Section 3 )  C-U B-D A-U
			{
				if( ePwm_Width_C < ePwm_Width_Min )							// C Too Small ?
				{
					ePwm_On_C  = EPWM_CMP_START;
					ePwm_Off_C = EPWM_CMP_START;

					ePwm_On_B  = EPWM_CMP_START;
					ePwm_Off_B = ePwm_On_B + ePwm_Width_B;

					ePwm_On_A  = ePwm_Off_B - EPWM_CMP_OVERLAY;
					ePwm_Off_A = ePwm_On_A + ePwm_Width_A;
				}
				else
				{
					ePwm_On_C  = EPWM_CMP_START;
					ePwm_Off_C = ePwm_On_C + ePwm_Width_C;

					ePwm_On_B  = ePwm_Off_C - EPWM_CMP_OVERLAY;
					ePwm_Off_B = ePwm_On_B + ePwm_Width_B;

					ePwm_On_A  = ePwm_Off_B - EPWM_CMP_OVERLAY;
					ePwm_Off_A = ePwm_On_A + ePwm_Width_A;
				}
			}
		}
	}

	//------------------------------

	EPwm1Regs.CMPA.bit.CMPA = ePwm_On_A;
	EPwm1Regs.CMPB.bit.CMPB = ePwm_Off_A;

	EPwm2Regs.CMPA.bit.CMPA = ePwm_On_B;
	EPwm2Regs.CMPB.bit.CMPB = ePwm_Off_B;

	EPwm3Regs.CMPA.bit.CMPA = ePwm_On_C;
	EPwm3Regs.CMPB.bit.CMPB = ePwm_Off_C;

	EALLOW;
	EPwm1Regs.GLDCTL2.bit.GFRCLD = 1;						// Software Force One Global Load Event
	EDIS;
}

//--------------------------------------------------------------------------------
// Compare Method 3 ( Output Stage Serial Connection )
//--------------------------------------------------------------------------------
void ePwm_width_S(void)										// Run Time in RAM ( CPU_Cycle ): 131 ( No  Min Width_Limit )
{															// Run Time in RAM ( CPU_Cycle ): 138 ( Add Min Width_Limit )
	Uint16	ePwm_On_A = 0, ePwm_Off_A = 0;					// Run Time in RAM ( CPU_Cycle ): 185 ( Add Max Width_Limit )
	Uint16	ePwm_On_B = 0, ePwm_Off_B = 0;					// Run Time in RAM ( CPU_Cycle ): 190 ( Add Min and Max Width_Limit )
	Uint16	ePwm_On_C = 0, ePwm_Off_C = 0;

	float	Width_Accumulator = 0;
	float	Width_RateAdj = 1;

	ePwm_Width_A = UiA_Trans * Rate_UiA_Pwm;
	ePwm_Width_B = UiB_Trans * Rate_UiB_Pwm;
	ePwm_Width_C = UiC_Trans * Rate_UiC_Pwm;

	Width_Accumulator = ePwm_Width_A + ePwm_Width_B + ePwm_Width_C;

	ePwm_Width_Arry[ePwm_Width_Index++] = Width_Accumulator;
	if( ePwm_Width_Index >= WDH_AVG_NUM_PWM )
	{
		ePwm_Width_Index = 0;
		ePwm_Width_Full = 1;
	}

	if( Width_Accumulator > ePwm_Width_Max )				// Max ePwm_Width Limit
	{
		Width_RateAdj = ePwm_Width_Max / Width_Accumulator;
		ePwm_Width_A = Width_RateAdj * ePwm_Width_A;
		ePwm_Width_B = Width_RateAdj * ePwm_Width_B;
		ePwm_Width_C = Width_RateAdj * ePwm_Width_C;
	}

	//------------------------------

	ePwm_On_A  = EPWM_CMP_START;
	ePwm_Off_A = EPWM_CMP_START + ePwm_Width_A;

	ePwm_On_B  = EPWM_CMP_START;
	ePwm_Off_B = EPWM_CMP_START + ePwm_Width_B;

	ePwm_On_C  = EPWM_CMP_START;
	ePwm_Off_C = EPWM_CMP_START + ePwm_Width_C;


	//------------------------------

	EPwm1Regs.CMPA.bit.CMPA = ePwm_On_A;
	EPwm1Regs.CMPB.bit.CMPB = ePwm_Off_A;

	EPwm2Regs.CMPA.bit.CMPA = ePwm_On_B;
	EPwm2Regs.CMPB.bit.CMPB = ePwm_Off_B;

	EPwm3Regs.CMPA.bit.CMPA = ePwm_On_C;
	EPwm3Regs.CMPB.bit.CMPB = ePwm_Off_C;

	EALLOW;
	EPwm1Regs.GLDCTL2.bit.GFRCLD = 1;						// Software Force One Global Load Event
	EDIS;
}
