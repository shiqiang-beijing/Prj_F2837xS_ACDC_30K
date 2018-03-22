
//--------------------------------------------------------------------------------
#include "F28x_Project.h"

//--------------------------------------------------------------------------------
#include "usr_ADC.h"
#include "usr_Global_Data.h"
#include "usr_Global_Func.h"

//--------------------------------------------------------------------------------
#ifdef	_FLASH

#pragma CODE_SECTION(ISR_AdcA_INT1, ".TI.ramfunc");
#pragma CODE_SECTION(ISR_AdcA_INT2, ".TI.ramfunc");
#pragma CODE_SECTION(ISR_AdcA_INT3, ".TI.ramfunc");
#pragma CODE_SECTION(ISR_AdcA_INT4, ".TI.ramfunc");

#pragma CODE_SECTION(ISR_AdcB_INT1, ".TI.ramfunc");

#pragma CODE_SECTION(ISR_AdcC_INT1, ".TI.ramfunc");
#pragma CODE_SECTION(ISR_AdcC_INT2, ".TI.ramfunc");

#pragma CODE_SECTION(ISR_AdcD_INT1, ".TI.ramfunc");
#pragma CODE_SECTION(ISR_AdcD_INT2, ".TI.ramfunc");

#endif

//--------------------------------------------------------------------------------
// Setup ADC Basic Parameters ( ADCCLOCK, WorkingMode, PowerUp )
//--------------------------------------------------------------------------------
void usr_AdcA_Initial(void)					// Initial ADC-A
{
	EALLOW;

	AdcaRegs.ADCCTL2.bit.PRESCALE = ADC_PRESCALE;						// ADCLCK
	AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);	// WorkMode: ADCA 12-Bit Single-Ended
	AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;								// Int Time: 0->Sample End,  1->Convertion End
	AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;									// Power:    0->Power-Down,  1->Power-Up
	DELAY_US(ADC_POWERUP_US);											// Wait for ADC Power-Up Time

	EDIS;
}

void usr_AdcB_Initial(void)					// Initial ADC-B
{
	EALLOW;

	AdcbRegs.ADCCTL2.bit.PRESCALE = ADC_PRESCALE;
	AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
	AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    DELAY_US(ADC_POWERUP_US);

	EDIS;
}

void usr_AdcC_Initial(void)					// Initial ADC-C
{
	EALLOW;

	AdccRegs.ADCCTL2.bit.PRESCALE = ADC_PRESCALE;						// ADCLCK
	AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);	// WorkMode: ADCA 12-Bit Single-Ended
	AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;								// Int Time: 0->Sample End,  1->Convertion End
	AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;									// Power:    0->Power-Down,  1->Power-Up
	DELAY_US(ADC_POWERUP_US);											// Wait for ADC Power-Up Time

	EDIS;
}

void usr_AdcD_Initial(void)					// Initial ADC-D
{
	EALLOW;

	AdcdRegs.ADCCTL2.bit.PRESCALE = ADC_PRESCALE;						// ADCLCK
	AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);	// WorkMode: ADCA 12-Bit Single-Ended
	AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;								// Int Time: 0->Sample End,  1->Convertion End
	AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;									// Power:    0->Power-Down,  1->Power-Up
	DELAY_US(ADC_POWERUP_US);											// Wait for ADC Power-Up Time

	EDIS;
}

//--------------------------------------------------------------------------------
// Setup ADC Working Parameters ( Channel, Trigger, Interrupt )
//--------------------------------------------------------------------------------
void usr_AdcA_Setup(void)
{
	EALLOW;

	//------------------------------ SOC ( Sample-Window, Channel, Trigger-Select )
	AdcaRegs.ADCSOC1CTL.bit.ACQPS  = ADC_ACQPS_Uac;
	AdcaRegs.ADCSOC1CTL.bit.CHSEL  = ADC_CHANNEL_UiA1;			// UiA1 -> Channel A1
	AdcaRegs.ADCSOC1CTL.bit.TRIGSEL= ADC_TRIGGER_UiA1;

	AdcaRegs.ADCSOC3CTL.bit.ACQPS  = ADC_ACQPS_Uac;
	AdcaRegs.ADCSOC3CTL.bit.CHSEL  = ADC_CHANNEL_IiA;			// IiA  -> Channel A3
	AdcaRegs.ADCSOC3CTL.bit.TRIGSEL= ADC_TRIGGER_IiA;

	AdcaRegs.ADCSOC4CTL.bit.ACQPS  = ADC_ACQPS_Uac;
	AdcaRegs.ADCSOC4CTL.bit.CHSEL  = ADC_CHANNEL_Uo1;			// Uo1  -> Channel A4
	AdcaRegs.ADCSOC4CTL.bit.TRIGSEL= ADC_TRIGGER_Uo1;

	AdcaRegs.ADCSOC0CTL.bit.ACQPS  = ADC_ACQPS_Uac;
	AdcaRegs.ADCSOC0CTL.bit.CHSEL  = ADC_CHANNEL_Io;			// Io   -> Channel A0
	AdcaRegs.ADCSOC0CTL.bit.TRIGSEL= ADC_TRIGGER_Io;

	//------------------------------ SOC ( Trigger: ePWM1_SOCA -> UiA1 )
	EPwm1Regs.ETSEL.bit.SOCAEN = 0;
	EPwm1Regs.ETSEL.bit.SOCASELCMP = 0;							// 0 -> Trigger by CMPA/CMPB	1 -> Trigger by CMPC/CMPD

	EPwm1Regs.ETSEL.bit.SOCASEL= 1;								// ePWMx_SOCx Trigger:
																// 1->TBCTR=0   2->TBCTR=TBPRD  3->TBCTR=0 or TBCTR=TBPRD
																// 4->TBCTR=CMPA/CMPC InCrease  5->TBCTR=CMPA/CMPC DeCrease
																// 6->TBCTR=CMPB/CMPD InCrease  7->TBCTR=CMPB/CMPD DeCrease

	EPwm1Regs.ETPS.bit.SOCAPRD = 1;								// ePWMx_SOCx Trigger On: 1->1st; 2->2nd; 3->3rd

	//------------------------------ SOC ( Trigger: ePWM7_SOCA -> IiA )
	EPwm7Regs.ETSEL.bit.SOCAEN = 0;
	EPwm7Regs.ETSEL.bit.SOCASELCMP = 0;
	EPwm7Regs.ETSEL.bit.SOCASEL= 4;
	EPwm7Regs.ETPS.bit.SOCAPRD = 2;

	//------------------------------ SOC ( Trigger: ePWM7_SOCB -> Uo1 )
	EPwm7Regs.ETSEL.bit.SOCBEN = 0;
	EPwm7Regs.ETSEL.bit.SOCBSELCMP = 0;
	EPwm7Regs.ETSEL.bit.SOCBSEL= 6;
	EPwm7Regs.ETPS.bit.SOCBPRD = 2;

	//------------------------------ SOC ( Trigger: ePWM9_SOCB -> Io )
	EPwm9Regs.ETSEL.bit.SOCBEN = 0;
	EPwm9Regs.ETSEL.bit.SOCBSELCMP = 0;
	EPwm9Regs.ETSEL.bit.SOCBSEL= 6;
	EPwm9Regs.ETPS.bit.SOCBPRD = 2;

	//------------------------------ INT ( INT_1 -> UiA1 )
	AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = ADC_CHANNEL_UiA1;		// ADCA1_INT Trigger:  0->EOC0 ~ 15->EOC15
	AdcaRegs.ADCINTSEL1N2.bit.INT1E   = 1;						// ADCA1_INT Enable
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;						// ADCA1_INT Flag Clear

	//------------------------------ INT ( INT_2 -> Uo1 )
	AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = ADC_CHANNEL_Uo1;
	AdcaRegs.ADCINTSEL1N2.bit.INT2E   = 1;
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;

	//------------------------------ INT ( INT_3 -> Io )
	AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = ADC_CHANNEL_Io;
	AdcaRegs.ADCINTSEL3N4.bit.INT3E   = 1;
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;

	//------------------------------ INT ( INT_4 -> IiA )
	AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = ADC_CHANNEL_IiA;
	AdcaRegs.ADCINTSEL3N4.bit.INT4E   = 1;
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1;

	EDIS;
}

void usr_AdcB_Setup(void)
{
	EALLOW;

	//------------------------------ SOC ( Sample-Window, Channel, Trigger-Select )
	AdcbRegs.ADCSOC2CTL.bit.ACQPS  = ADC_ACQPS_Uac;
	AdcbRegs.ADCSOC2CTL.bit.CHSEL  = ADC_CHANNEL_UiB1;			// UiB1 -> Channel B2
	AdcbRegs.ADCSOC2CTL.bit.TRIGSEL= ADC_TRIGGER_UiB1;

	//------------------------------ SOC ( Trigger: ePWM2_SOCA -> UiB1 )
	EPwm2Regs.ETSEL.bit.SOCAEN = 0;
	EPwm2Regs.ETSEL.bit.SOCASELCMP = 0;							// 0 -> Trigger by CMPA/CMPB		1 -> Trigger by CMPC/CMPD
	EPwm2Regs.ETSEL.bit.SOCASEL= 1;								// ePWMx_SOCx Trigger:				1->TBCTR=0
	EPwm2Regs.ETPS.bit.SOCAPRD = 1;								// ePWMx_SOCx Trigger On: 1->1st; 2->2nd; 3->3rd

	//------------------------------ INT ( INT_1 -> UiB1 )
	AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = ADC_CHANNEL_UiB1;		// ADCB1_INT Trigger:  0->EOC0 ~ 15->EOC15
	AdcbRegs.ADCINTSEL1N2.bit.INT1E   = 1;						// ADCB1_INT Enable
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;						// ADCB1_INT Flag Clear

	EDIS;
}

void usr_AdcC_Setup(void)
{
	EALLOW;

	//------------------------------ SOC ( Sample-Window, Channel, Trigger-Select )
	AdccRegs.ADCSOC2CTL.bit.ACQPS  = ADC_ACQPS_Uac;
	AdccRegs.ADCSOC2CTL.bit.CHSEL  = ADC_CHANNEL_UiC1;			// UiC1 -> Channel C2
	AdccRegs.ADCSOC2CTL.bit.TRIGSEL= ADC_TRIGGER_UiC1;

	AdccRegs.ADCSOC4CTL.bit.ACQPS  = ADC_ACQPS_Uac;
	AdccRegs.ADCSOC4CTL.bit.CHSEL  = ADC_CHANNEL_IiB;			// IiB  -> Channel C4
	AdccRegs.ADCSOC4CTL.bit.TRIGSEL= ADC_TRIGGER_IiB;

	//------------------------------ SOC ( Trigger: ePWM3_SOCA -> UiC1 )
	EPwm3Regs.ETSEL.bit.SOCAEN = 0;
	EPwm3Regs.ETSEL.bit.SOCASELCMP = 0;							// 0 -> Trigger by CMPA/CMPB		1 -> Trigger by CMPC/CMPD
	EPwm3Regs.ETSEL.bit.SOCASEL= 1;								// ePWMx_SOCx Trigger:				1->TBCTR=0
	EPwm3Regs.ETPS.bit.SOCAPRD = 1;								// ePWMx_SOCx Trigger On: 1->1st; 2->2nd; 3->3rd

	//------------------------------ SOC ( Trigger: ePWM8_SOCA -> IiB )
	EPwm8Regs.ETSEL.bit.SOCAEN = 0;
	EPwm8Regs.ETSEL.bit.SOCASELCMP = 0;
	EPwm8Regs.ETSEL.bit.SOCASEL= 4;
	EPwm8Regs.ETPS.bit.SOCAPRD = 2;

	//------------------------------ INT ( INT_1 -> UiC1 )
	AdccRegs.ADCINTSEL1N2.bit.INT1SEL = ADC_CHANNEL_UiC1;		// ADCC1_INT Trigger:  0->EOC0 ~ 15->EOC15
	AdccRegs.ADCINTSEL1N2.bit.INT1E   = 1;						// ADCC1_INT Enable
	AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;						// ADCC1_INT Flag Clear

	//------------------------------ INT ( INT_2 -> IiB )
	AdccRegs.ADCINTSEL1N2.bit.INT2SEL = ADC_CHANNEL_IiB;
	AdccRegs.ADCINTSEL1N2.bit.INT2E   = 1;
	AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;

	EDIS;
}

void usr_AdcD_Setup(void)
{
	EALLOW;

	//------------------------------ SOC ( Sample-Window, Channel, Trigger-Select )
	AdcdRegs.ADCSOC1CTL.bit.ACQPS  = ADC_ACQPS_Uac;
	AdcdRegs.ADCSOC1CTL.bit.CHSEL  = ADC_CHANNEL_Uo2;			// Uo2 -> Channel D1
	AdcdRegs.ADCSOC1CTL.bit.TRIGSEL= ADC_TRIGGER_Uo2;

	AdcdRegs.ADCSOC0CTL.bit.ACQPS  = ADC_ACQPS_Uac;
	AdcdRegs.ADCSOC0CTL.bit.CHSEL  = ADC_CHANNEL_IiC;			// IiC -> Channel D0
	AdcdRegs.ADCSOC0CTL.bit.TRIGSEL= ADC_TRIGGER_IiC;

	//------------------------------ SOC ( Trigger: ePWM8_SOCB -> Uo2 )
	EPwm8Regs.ETSEL.bit.SOCBEN = 0;
	EPwm8Regs.ETSEL.bit.SOCBSELCMP = 0;							// 0 -> Trigger by CMPA/CMPB		1 -> Trigger by CMPC/CMPD
	EPwm8Regs.ETSEL.bit.SOCBSEL= 6;								// ePWMx_SOCx Trigger:				6->TBCTR=CMPB/CMPD InCrease
	EPwm8Regs.ETPS.bit.SOCBPRD = 2;								// ePWMx_SOCx Trigger On: 1->1st; 2->2nd; 3->3rd

	//------------------------------ SOC ( Trigger: ePWM9_SOCA -> IiC )
	EPwm9Regs.ETSEL.bit.SOCAEN = 0;
	EPwm9Regs.ETSEL.bit.SOCASELCMP = 0;
	EPwm9Regs.ETSEL.bit.SOCASEL= 4;
	EPwm9Regs.ETPS.bit.SOCAPRD = 2;

	//------------------------------ INT ( INT_1 -> Uo2 )
	AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = ADC_CHANNEL_Uo2;		// ADCD1_INT Trigger:  0->EOC0 ~ 15->EOC15
	AdcdRegs.ADCINTSEL1N2.bit.INT1E   = 1;						// ADCD1_INT Enable
	AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;						// ADCD1_INT Flag Clear

	//------------------------------ INT ( INT_2 -> IiC )
	AdcdRegs.ADCINTSEL1N2.bit.INT2SEL = ADC_CHANNEL_IiC;
	AdcdRegs.ADCINTSEL1N2.bit.INT2E   = 1;
	AdcdRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;

	EDIS;
}

//--------------------------------------------------------------------------------
// ADC Triggers
//--------------------------------------------------------------------------------
void usr_Adc_Trigger_On(void)
{
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;								// UiA1
	EPwm2Regs.ETSEL.bit.SOCAEN = 1;								// UiB1
	EPwm3Regs.ETSEL.bit.SOCAEN = 1;								// UiC1

	EPwm7Regs.ETSEL.bit.SOCAEN = 1;								// IiA
	EPwm8Regs.ETSEL.bit.SOCAEN = 1;								// IiB
	EPwm9Regs.ETSEL.bit.SOCAEN = 1;								// IiC

	EPwm7Regs.ETSEL.bit.SOCBEN = 1;								// Uo1
	EPwm8Regs.ETSEL.bit.SOCBEN = 1;								// Uo2
	EPwm9Regs.ETSEL.bit.SOCBEN = 1;								// Io
}

void usr_Adc_Trigger_Off(void)
{
	EPwm1Regs.ETSEL.bit.SOCAEN = 0;
	EPwm2Regs.ETSEL.bit.SOCAEN = 0;
	EPwm3Regs.ETSEL.bit.SOCAEN = 0;
}

//--------------------------------------------------------------------------------
// ADC-A INT
//--------------------------------------------------------------------------------
__interrupt void ISR_AdcA_INT1(void)						// UiA1 -> Channle A1 -> INT_A1
{
	// ------ Get the Data
	#ifdef	VCM_TEST_MODE
	UiA_Trans = AdcaResultRegs.ADCRESULT1;					// No VCM
	#else
	if( AdcaResultRegs.ADCRESULT1 > ADC_ZERO_UiA )
	{
		UiA_Trans = AdcaResultRegs.ADCRESULT1 - ADC_ZERO_UiA;
	}
	else
	{
		UiA_Trans = ADC_ZERO_UiA - AdcaResultRegs.ADCRESULT1;
	}
	#endif

	// ------ Data into Array
	AdcResult_UiA[AdcResult_UiA_Index++] = UiA_Trans;

	if( AdcResult_UiA_Index >= BUFFER_SIZE_UAC )
	{
		AdcResult_UiA_Index = 0;
		AdcBuffer_UiA_Full = 1;
	}

	AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;					// Clear ADCA_INT1 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;					// Clear ADC Group flag
}

__interrupt void ISR_AdcA_INT2(void)						// Uo1  -> Channle A4 -> INT_A2	 ( in the Front of  Lo )
{
	// ------ Get the Data
	Uo1_Trans = AdcaResultRegs.ADCRESULT4;

	// ------ Data into Array
	AdcResult_Uo1[AdcResult_Uo1_Index++] = Uo1_Trans;

	if( AdcResult_Uo1_Index >= BUFFER_SIZE_UDC )
	{
		AdcResult_Uo1_Index = 0;
		AdcBuffer_Uo1_Full = 1;
	}

	AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;					// Clear ADCA_INT2 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;				// Clear ADC Group flag
}

__interrupt void ISR_AdcA_INT3(void)						// Io   -> Channle A0 -> INT_A3
{
	// ------ Get the Data
	Io_Trans = AdcaResultRegs.ADCRESULT0;

	// ------ Data into Array
	AdcResult_Io[AdcResult_Io_Index++] = Io_Trans;

	if( AdcResult_Io_Index >= BUFFER_SIZE_IDC )
	{
		AdcResult_Io_Index = 0;
		AdcBuffer_Io_Full = 1;
	}

	AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;					// Clear ADCA_INT3 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;				// Clear ADC Group flag
}

__interrupt void ISR_AdcA_INT4(void)						// IiA  -> Channle A3 -> INT_A4
{
	// ------ Get the Data
	#ifdef	VCM_TEST_MODE
	IiA_Trans = AdcaResultRegs.ADCRESULT3;					// No VCM
	#else
	if( AdcaResultRegs.ADCRESULT3 > ADC_ZERO_IiA )
	{
		IiA_Trans = AdcaResultRegs.ADCRESULT3 - ADC_ZERO_IiA;
	}
	else
	{
		IiA_Trans = ADC_ZERO_IiA - AdcaResultRegs.ADCRESULT3;
	}
	#endif

	// ------ Data into Array
	AdcResult_IiA[AdcResult_IiA_Index++] = IiA_Trans;

	if( AdcResult_IiA_Index >= BUFFER_SIZE_IAC )
	{
		AdcResult_IiA_Index = 0;
		AdcBuffer_IiA_Full = 1;
	}

	AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1;					// Clear ADCA_INT4 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;				// Clear ADC Group flag
}

//--------------------------------------------------------------------------------
// ADC-B INT
//--------------------------------------------------------------------------------
__interrupt void ISR_AdcB_INT1(void)						// UiB1 -> Channle B2 -> INT_B1
{
	// ------ Get the Data
	#ifdef	VCM_TEST_MODE									// No VCM
	UiB_Trans = AdcbResultRegs.ADCRESULT2;
	#else
	if( AdcbResultRegs.ADCRESULT2 > ADC_ZERO_UiB )
	{
		UiB_Trans = AdcbResultRegs.ADCRESULT2 - ADC_ZERO_UiB;
	}
	else
	{
		UiB_Trans = ADC_ZERO_UiB - AdcbResultRegs.ADCRESULT2;
	}
	#endif

	// ------ Data into Array
	AdcResult_UiB[AdcResult_UiB_Index++] = UiB_Trans;

	if( AdcResult_UiB_Index >= BUFFER_SIZE_UAC )
	{
		AdcResult_UiB_Index = 0;
		AdcBuffer_UiB_Full = 1;
	}

	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;					// Clear ADCB_INT1 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;					// Clear ADC Group flag
}

__interrupt void ISR_AdcB_INT2(void)
{
	// ------ Get the Data


	// ------ Data into Array

	AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;					// Clear ADCB_INT2 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}

__interrupt void ISR_AdcB_INT3(void)
{
	// ------ Get the Data


	// ------ Data into Array


	AdcbRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;					// Clear ADCB_INT3 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}

__interrupt void ISR_AdcB_INT4(void)
{


	AdcbRegs.ADCINTFLGCLR.bit.ADCINT4 = 1;					// Clear ADCB_INT4 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}

//--------------------------------------------------------------------------------
// ADC-C INT
//--------------------------------------------------------------------------------
__interrupt void ISR_AdcC_INT1(void)						// UiC1 -> Channle C2 -> INT_C1
{
	// ------ Get the Data
	#ifdef	VCM_TEST_MODE									// No VCM
	UiC_Trans = AdccResultRegs.ADCRESULT2;
	#else
	if( AdccResultRegs.ADCRESULT2 > ADC_ZERO_UiC )
	{
		UiC_Trans = AdccResultRegs.ADCRESULT2 - ADC_ZERO_UiC;
	}
	else
	{
		UiC_Trans = ADC_ZERO_UiC - AdccResultRegs.ADCRESULT2;
	}
	#endif

	// ------ Data into Array
	AdcResult_UiC[AdcResult_UiC_Index++] = UiC_Trans;

	if( AdcResult_UiC_Index >= BUFFER_SIZE_UAC )
	{
		AdcResult_UiC_Index = 0;
		AdcBuffer_UiC_Full = 1;
	}

	AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;					// Clear ADCC_INT1 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;					// Clear ADC Group flag
}

__interrupt void ISR_AdcC_INT2(void)						// IiB  -> Channle C4 -> INT_C2
{
	// ------ Get the Data
	#ifdef	VCM_TEST_MODE
	IiB_Trans = AdccResultRegs.ADCRESULT4;					// No VCM
	#else
	if( AdccResultRegs.ADCRESULT4 > ADC_ZERO_IiB )
	{
		IiB_Trans = AdccResultRegs.ADCRESULT4 - ADC_ZERO_IiB;
	}
	else
	{
		IiB_Trans = ADC_ZERO_IiB - AdccResultRegs.ADCRESULT4;
	}
	#endif

	// ------ Data into Array
	AdcResult_IiB[AdcResult_IiB_Index++] = IiB_Trans;

	if( AdcResult_IiB_Index >= BUFFER_SIZE_IAC )
	{
		AdcResult_IiB_Index = 0;
		AdcBuffer_IiB_Full = 1;
	}

	AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;					// Clear ADCC_INT2 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}

//--------------------------------------------------------------------------------
// ADC-D INT
//--------------------------------------------------------------------------------
__interrupt void ISR_AdcD_INT1(void)						// Uo2  -> Channle D1 -> INT_D1  ( at the Behind of Lo )
{
	// ------ Get the Data
	Uo2_Trans = AdcdResultRegs.ADCRESULT1;

	// ------ Data into Array
	AdcResult_Uo2[AdcResult_Uo2_Index++] = Uo2_Trans;

	if( AdcResult_Uo2_Index >= BUFFER_SIZE_UDC )
	{
		AdcResult_Uo2_Index = 0;
		AdcBuffer_Uo2_Full = 1;
	}

	AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;					// Clear ADCD_INT1 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;					// Clear ADC Group flag
}

__interrupt void ISR_AdcD_INT2(void)						// IiC  -> Channle D0 -> INT_D2
{
	// ------ Get the Data
	#ifdef	VCM_TEST_MODE
	IiC_Trans = AdcdResultRegs.ADCRESULT0;					// No VCM
	#else
	if( AdcdResultRegs.ADCRESULT0 > ADC_ZERO_IiC )
	{
		IiC_Trans = AdcdResultRegs.ADCRESULT0 - ADC_ZERO_IiC;
	}
	else
	{
		IiC_Trans = ADC_ZERO_IiC - AdcdResultRegs.ADCRESULT0;
	}
	#endif

	// ------ Data into Array
	AdcResult_IiC[AdcResult_IiC_Index++] = IiC_Trans;

	if( AdcResult_IiC_Index >= BUFFER_SIZE_IAC )
	{
		AdcResult_IiC_Index = 0;
		AdcBuffer_IiC_Full = 1;
	}

	AdcdRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;					// Clear ADCD_INT2 flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}

