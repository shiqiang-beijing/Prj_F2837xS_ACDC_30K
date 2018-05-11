
//--------------------------------------------------------------------------------
#include	"F28x_Project.h"

//--------------------------------------------------------------------------------
#include	"usr_Gpio.h"
#include	"usr_ePWM.h"

#include	"usr_Global_Data.h"
#include	"usr_Global_Func.h"

//--------------------------------------------------------------------------------
#ifdef	_FLASH
#pragma CODE_SECTION(ISR_ePWM10, ".TI.ramfunc");
#endif

//--------------------------------------------------------------------------------
void usr_ePWM_Initial(void)
{
	//------------------------------ ePWM Clock Divide Select ( Default : EPWMCLK=SYSCLKOUT/2 )

	EALLOW;
	ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;			// 0 -> EPWMCLK=SYSCLKOUT;  1 -> EPWMCLK=SYSCLKOUT/2
	EDIS;

	//------------------------------ Enable Used ePWM Clock

	EALLOW;												// All Peripheral Clock Enabled : InitSysCtrl() -> InitPeripheralClocks()

	CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;

	CpuSysRegs.PCLKCR2.bit.EPWM4 = 0;					// Disable
	CpuSysRegs.PCLKCR2.bit.EPWM5 = 0;
	CpuSysRegs.PCLKCR2.bit.EPWM6 = 0;

	CpuSysRegs.PCLKCR2.bit.EPWM7 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM8 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM9 = 1;

	CpuSysRegs.PCLKCR2.bit.EPWM10 = 1;
	CpuSysRegs.PCLKCR2.bit.EPWM11 = 0;					// Disable
	CpuSysRegs.PCLKCR2.bit.EPWM12 = 0;

	EDIS;

	//------------------------------ Initial ePWM GPIO pins
	InitEPwm1Gpio();									// GPIO00-> EPWM1A,   GPIO01-> EPWM1B	(PZP -> No 1A, 1B Pins)
	InitEPwm2Gpio();									// GPIO02-> EPWM2A,   GPIO03-> EPWM2B
	InitEPwm3Gpio();									// GPIO04-> EPWM3A,   GPIO05-> EPWM3B	(PZP -> No 3B Pin)

//	InitEPwm4Gpio();									// GPIO06-> EPWM4A,   GPIO07-> EPWM4B
//	InitEPwm5Gpio();									// GPIO08-> EPWM5A,   GPIO09-> EPWM5B
//	InitEPwm6Gpio();									// GPIO10-> EPWM6A,   GPIO11-> EPWM6B

//	InitEPwm7Gpio();									// GPIO12-> EPWM7A,   GPIO13-> EPWM7B
//	InitEPwm8Gpio();									// GPIO14-> EPWM8A,   GPIO15-> EPWM8B
//	InitEPwm9Gpio();									// GPIO161->EPWM9A,   GPIO162->EPWM9B	(ZWP -> C9, D9)

//	InitEPwm10Gpio();									// GPIO163->EPWM10A,  GPIO164->EPWM10B	(ZWP -> C9, D9)
//	InitEPwm11Gpio();									// GPIO163->EPWM11A,  GPIO164->EPWM11B	(ZWP -> A8, B8)
//	InitEPwm12Gpio();									// GPIO165->EPWM12A,  GPIO165->EPWM12B	(ZWP -> C5, D5)
}

//--------------------------------------------------------------------------------
// Used for IGBT Drive ( and ADC-UiA1 ADC-UiB1 ADC-UiC1 Trigger)
//--------------------------------------------------------------------------------
void usr_ePWM1_Setup(void)
{
	//------------------------------ (TB) Setup TBCLK
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// Pre-Scale of High Speed Time Base Clock
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;			// Pre-Scale of Time Base Clock

	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;		// Count Mode:	 TB_COUNT_UP, TB_COUNT_DOWN, TB_COUNT_UPDOWN, TB_FREEZE
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		// Sync Output:	 TB_SYNC_IN(External Sync),  ZERO,  CMPB,  Disable
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;			// SHADOW Mode:	 0->TB_SHADOW ( Load On TBCTR=0 )   1->TB_IMMEDIATE

	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;			// TBCTR Load from TBPHS:  0->TB_DISABLE   1->TB_ENABLE
	EPwm1Regs.TBCTL.bit.PHSDIR = 1;					// Count-Up after Synchronization Event.

	EPwm1Regs.TBCTL2.bit.PRDLDSYNC = 1;				// TBPRD Load from Shadow: 0->TBCTR=0  1->TBCTR=0 OR SYNC  2->SYNC

	EPwm1Regs.TBPRD = EPWM_TBPRD_INITIAL;			// Set Count Period
	EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;				// Phase is 0
	EPwm1Regs.TBCTR = 0x0000;						// Clear counter

	//------------------------------ (CC) Setup Shadow Register£¨CMPA¡¢CMPB£©load on ZERO

	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;	// CC_SHADOW, CC_IMMEDIATE
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;
/*
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		// CC_SHADOW, CC_IMMEDIATE
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
*/
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	// CC_CTR_ZERO, CC_CTR_PRD, CC_CTR_ZERO_PRD, CC_LD_DISABLE
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	// .LOADBMODE : Hase No Effect in Immediate Mode


	//------------------------------ (CC) Setup Compare values£¨CMPA¡¢CMPB£©
	EPwm1Regs.CMPA.bit.CMPA = EPWM_CMP_START;
	#ifdef	EPWM_TEST_MODE
	EPwm1Regs.CMPB.bit.CMPB = EPWM_CMP_START + 1984;// ePWM Test Mode ( 25% Pulse Width )
	#else
	EPwm1Regs.CMPB.bit.CMPB = EPWM_CMP_START;
	#endif

	//------------------------------ (AQ) Setup Actions	   Set Points£ºZRO, PRD, CAU, CAD, CBU, CBD
	EPwm1Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
	EPwm1Regs.AQCTLA.bit.PRD = AQ_CLEAR;
	EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm1Regs.AQCTLA.bit.CBU = AQ_CLEAR;

	EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm1Regs.AQCTLB.bit.PRD = AQ_CLEAR;
	EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;
	EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;

	//------------------------------ (DB) Setup Dead-Band Generator

	//------------------------------ (ET) Setup Interrupt	   Where we will change the Compare Values
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;		// INT Event: ET_CTR_ZERO, ET_CTR_PRD, ET_CTR_PRDZERO
													// 			  ET_CTRU_CMPA,ET_CTRD_CMPA,ET_CTRU_CMPB,ET_CTRD_CMPB
	EPwm1Regs.ETSEL.bit.INTEN = 0;					// 1 -> INT Enable,					0 -> INT Disable
	EPwm1Regs.ETSEL.bit.INTSELCMP = 0;				// 0-> INT Trigger by CMPA/CMPB     1-> INT Trigger by CMPC/CMPD
	EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;				// INT Generate on 1st event

	//------------------------------ (PC) Setup PWM Chopper
	EPwm1Regs.PCCTL.bit.CHPEN = 0;					// CHPEN = 1;
	EPwm1Regs.PCCTL.bit.OSHTWTH = 0xF;				// One-shot pulse width = N ¡Á EPWMCLK*8£»N =£¨0x0~0xF£©
	EPwm1Regs.PCCTL.bit.CHPFREQ = CHP_DIV8;			// Chopping clock frequency = EPWMCLK/N/8£»N =£¨CHP_DIV1~CHP_DIV8£©
	EPwm1Regs.PCCTL.bit.CHPDUTY = CHP4_8TH;			// Chopping clock Duty cycle CHP1_8TH£¨12.5%£©~ CHP7_8TH£¨87.5%£©

	//------------------------------ (TZ) Setup

	//------------------------------ (DC) Setup

	//------------------------------ EPWMx Link
	EPwm1Regs.EPWMXLINK.bit.TBPRDLINK = 0;			// TBPRD   Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12
	EPwm1Regs.EPWMXLINK.bit.GLDCTL2LINK = 0;		// GLDCTL2 Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12

	//------------------------------ Global ReLoad
	EALLOW;

	EPwm1Regs.GLDCTL.bit.GLD = 1;					// the Global Load Mode :     1 -> Enable        0 -> Disable
	EPwm1Regs.GLDCTL.bit.GLDPRD = 1;				// How Many Selected events : 001 -> 1 event  ~  111 -> 7 event
	EPwm1Regs.GLDCTL.bit.GLDMODE = 1;				// Global Load on LoadCounter = Period

	EPwm1Regs.GLDCFG.bit.CMPA_CMPAHR = 1;
	EPwm1Regs.GLDCFG.bit.CMPB_CMPBHR = 1;
	EPwm1Regs.GLDCFG.bit.TBPRD_TBPRDHR = 1;

	EDIS;
}

void usr_ePWM2_Setup(void)
{
	//------------------------------ (TB) Setup TBCLK
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// Pre-Scale of High Speed Time Base Clock
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;			// Pre-Scale of Time Base Clock

	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;		// Count Mode:	 TB_COUNT_UP, TB_COUNT_DOWN, TB_COUNT_UPDOWN, TB_FREEZE
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		// Sync Output:	 TB_SYNC_IN(External Sync),  ZERO,  CMPB,  Disable
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;			// SHADOW Mode:	 0->TB_SHADOW ( Load On TBCTR=0 )   1->TB_IMMEDIATE

	EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;			// TBCTR Load from TBPHS:  0->TB_DISABLE   1->TB_ENABLE
	EPwm2Regs.TBCTL.bit.PHSDIR = 1;					// Count-Up after Synchronization Event.

	EPwm2Regs.TBCTL2.bit.PRDLDSYNC = 1;				// TBPRD Load from Shadow: 0->TBCTR=0  1->TBCTR=0 OR SYNC  2->SYNC

	EPwm2Regs.TBPRD = EPWM_TBPRD_INITIAL;			// Set Count Period
	EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;				// Phase is 0
	EPwm2Regs.TBCTR = 0x0000;						// Clear counter

	//------------------------------ (CC) Setup Shadow Register£¨CMPA¡¢CMPB£©load on ZERO
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;	// CC_SHADOW, CC_IMMEDIATE
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;
/*
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		// CC_SHADOW, CC_IMMEDIATE
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
*/
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	// CC_CTR_ZERO, CC_CTR_PRD, CC_CTR_ZERO_PRD, CC_LD_DISABLE
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	// .LOADBMODE : Hase No Effect in Immediate Mode

	//------------------------------ (CC) Setup Compare values£¨CMPA¡¢CMPB£©
	EPwm2Regs.CMPA.bit.CMPA = EPWM_CMP_START;
	#ifdef	EPWM_TEST_MODE
	EPwm2Regs.CMPB.bit.CMPB = EPWM_CMP_START + 3968;// ePWM Test Mode ( 50% Pulse Width )
	#else
	EPwm2Regs.CMPB.bit.CMPB = EPWM_CMP_START;
	#endif

	//------------------------------ (AQ) Setup Actions	   Set Points£ºZRO, PRD, CAU, CAD, CBU, CBD
	EPwm2Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
	EPwm2Regs.AQCTLA.bit.PRD = AQ_CLEAR;
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm2Regs.AQCTLA.bit.CBU = AQ_CLEAR;

	EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm2Regs.AQCTLB.bit.PRD = AQ_CLEAR;
	EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;
	EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;

	//------------------------------ (DB) Setup Dead-Band Generator

	//------------------------------ (ET) Setup Interrupt	   Where we will change the Compare Values
	EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;		// INT Event: ET_CTR_ZERO, ET_CTR_PRD, ET_CTR_PRDZERO
													// 			  ET_CTRU_CMPA,ET_CTRD_CMPA,ET_CTRU_CMPB,ET_CTRD_CMPB
	EPwm2Regs.ETSEL.bit.INTEN = 0;					// 1 -> INT Enable,					0 -> INT Disable
	EPwm2Regs.ETSEL.bit.INTSELCMP = 0;				// 0-> INT Trigger by CMPA/CMPB     1-> INT Trigger by CMPC/CMPD
	EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;				// INT Generate on 1st event

	//------------------------------ (PC) Setup PWM Chopper
	EPwm2Regs.PCCTL.bit.CHPEN = 0;					// CHPEN = 1;
	EPwm2Regs.PCCTL.bit.OSHTWTH = 0xF;				// One-shot pulse width = N ¡Á EPWMCLK*8£»N =£¨0x0~0xF£©
	EPwm2Regs.PCCTL.bit.CHPFREQ = CHP_DIV8;			// Chopping clock frequency = EPWMCLK/N/8£»N =£¨CHP_DIV1~CHP_DIV8£©
	EPwm2Regs.PCCTL.bit.CHPDUTY = CHP4_8TH;			// Chopping clock Duty cycle CHP1_8TH£¨12.5%£©~ CHP7_8TH£¨87.5%£©

	//------------------------------ (TZ) Setup

	//------------------------------ (DC) Setup

	//------------------------------ EPWMx Link
	EPwm2Regs.EPWMXLINK.bit.TBPRDLINK = 0;			// TBPRD   Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12
	EPwm2Regs.EPWMXLINK.bit.GLDCTL2LINK = 0;		// GLDCTL2 Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12

	//------------------------------ Global ReLoad
	EALLOW;

	EPwm2Regs.GLDCTL.bit.GLD = 1;					// the Global Load Mode :     1 -> Enable        0 -> Disable
	EPwm2Regs.GLDCTL.bit.GLDPRD = 1;				// How Many Selected events : 001 -> 1 event  ~  111 -> 7 event
	EPwm2Regs.GLDCTL.bit.GLDMODE = 1;				// Global Load on LoadCounter = Period

	EPwm2Regs.GLDCFG.bit.CMPA_CMPAHR = 1;
	EPwm2Regs.GLDCFG.bit.CMPB_CMPBHR = 1;
	EPwm2Regs.GLDCFG.bit.TBPRD_TBPRDHR = 1;

	EDIS;
}

void usr_ePWM3_Setup(void)
{
	//------------------------------ (TB) Setup TBCLK
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// Pre-Scale of High Speed Time Base Clock
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;			// Pre-Scale of Time Base Clock

	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;		// Count Mode:	 TB_COUNT_UP, TB_COUNT_DOWN, TB_COUNT_UPDOWN, TB_FREEZE
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		// Sync Output:	 TB_SYNC_IN(External Sync),  ZERO,  CMPB,  Disable
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;			// SHADOW Mode:	 0->TB_SHADOW ( Load On TBCTR=0 )   1->TB_IMMEDIATE

	EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;			// TBCTR Load from TBPHS:  0->TB_DISABLE   1->TB_ENABLE
	EPwm3Regs.TBCTL.bit.PHSDIR = 1;					// Count-Up after Synchronization Event.

	EPwm3Regs.TBCTL2.bit.PRDLDSYNC = 1;				// TBPRD Load from Shadow: 0->TBCTR=0  1->TBCTR=0 OR SYNC  2->SYNC

	EPwm3Regs.TBPRD = EPWM_TBPRD_INITIAL;			// Set Count Period
	EPwm3Regs.TBPHS.bit.TBPHS = 0x0000;				// Phase is 0
	EPwm3Regs.TBCTR = 0x0000;						// Clear counter

	//------------------------------ (CC) Setup Shadow Register£¨CMPA¡¢CMPB£©load on ZERO
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;	// CC_SHADOW, CC_IMMEDIATE
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;
/*
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		// CC_SHADOW, CC_IMMEDIATE
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
*/
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	// CC_CTR_ZERO, CC_CTR_PRD, CC_CTR_ZERO_PRD, CC_LD_DISABLE
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	// .LOADBMODE : Hase No Effect in Immediate Mode

	//------------------------------ (CC) Setup Compare values£¨CMPA¡¢CMPB£©
	EPwm3Regs.CMPA.bit.CMPA = EPWM_CMP_START;
	#ifdef	EPWM_TEST_MODE
	EPwm3Regs.CMPB.bit.CMPB = EPWM_CMP_START + 5953;// ePWM Test Mode ( 75% Pulse Width )
	#else
	EPwm3Regs.CMPB.bit.CMPB = EPWM_CMP_START;
	#endif

	//------------------------------ (AQ) Setup Actions	   Set Points£ºZRO, PRD, CAU, CAD, CBU, CBD
	EPwm3Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
	EPwm3Regs.AQCTLA.bit.PRD = AQ_CLEAR;
	EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm3Regs.AQCTLA.bit.CBU = AQ_CLEAR;

	EPwm3Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm3Regs.AQCTLB.bit.PRD = AQ_CLEAR;
	EPwm3Regs.AQCTLB.bit.CAU = AQ_SET;
	EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;

	//------------------------------ (DB) Setup Dead-Band Generator

	//------------------------------ (ET) Setup Interrupt	   Where we will change the Compare Values
	EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;		// INT Event: ET_CTR_ZERO, ET_CTR_PRD, ET_CTR_PRDZERO
													// 			  ET_CTRU_CMPA,ET_CTRD_CMPA,ET_CTRU_CMPB,ET_CTRD_CMPB
	EPwm3Regs.ETSEL.bit.INTEN = 0;					// 1 -> INT Enable,					0 -> INT Disable
	EPwm3Regs.ETSEL.bit.INTSELCMP = 0;				// 0-> INT Trigger by CMPA/CMPB     1-> INT Trigger by CMPC/CMPD
	EPwm3Regs.ETPS.bit.INTPRD = ET_1ST;				// INT Generate on 1st event

	//------------------------------ (PC) Setup PWM Chopper
	EPwm3Regs.PCCTL.bit.CHPEN = 0;					// CHPEN = 1;
	EPwm3Regs.PCCTL.bit.OSHTWTH = 0xF;				// One-shot pulse width = N ¡Á EPWMCLK*8£»N =£¨0x0~0xF£©
	EPwm3Regs.PCCTL.bit.CHPFREQ = CHP_DIV8;			// Chopping clock frequency = EPWMCLK/N/8£»N =£¨CHP_DIV1~CHP_DIV8£©
	EPwm3Regs.PCCTL.bit.CHPDUTY = CHP4_8TH;			// Chopping clock Duty cycle CHP1_8TH£¨12.5%£©~ CHP7_8TH£¨87.5%£©

	//------------------------------ (TZ) Setup

	//------------------------------ (DC) Setup

	//------------------------------ EPWMx Link
	EPwm3Regs.EPWMXLINK.bit.TBPRDLINK = 0;			// TBPRD   Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12
	EPwm3Regs.EPWMXLINK.bit.GLDCTL2LINK = 0;		// GLDCTL2 Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12

	//------------------------------ Global ReLoad
	EALLOW;

	EPwm3Regs.GLDCTL.bit.GLD = 1;					// the Global Load Mode :     1 -> Enable        0 -> Disable
	EPwm3Regs.GLDCTL.bit.GLDPRD = 1;				// How Many Selected events : 001 -> 1 event  ~  111 -> 7 event
	EPwm3Regs.GLDCTL.bit.GLDMODE = 1;				// Global Load on LoadCounter = Period

	EPwm3Regs.GLDCFG.bit.CMPA_CMPAHR = 1;
	EPwm3Regs.GLDCFG.bit.CMPB_CMPBHR = 1;
	EPwm3Regs.GLDCFG.bit.TBPRD_TBPRDHR = 1;

	EDIS;
}

//--------------------------------------------------------------------------------
// Used for ADC Trigger
//--------------------------------------------------------------------------------
void usr_ePWM7_Setup(void)
{
	//------------------------------ (TB) Setup TBCLK
	EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// Pre-Scale of High Speed Time Base Clock
	EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV1;			// Pre-Scale of Time Base Clock

	EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;		// Count Mode:	 TB_COUNT_UP, TB_COUNT_DOWN, TB_COUNT_UPDOWN, TB_FREEZE
	EPwm7Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		// Sync Output:	 TB_SYNC_IN(External Sync),  ZERO,  CMPB,  Disable
	EPwm7Regs.TBCTL.bit.PRDLD = TB_SHADOW;			// SHADOW Mode:	 0->TB_SHADOW ( Load On TBCTR=0 )   1->TB_IMMEDIATE

	EPwm7Regs.TBCTL.bit.PHSEN = TB_DISABLE;			// TBCTR Load from TBPHS:  0->TB_DISABLE   1->TB_ENABLE
	EPwm7Regs.TBCTL.bit.PHSDIR = 1;					// Count-Up after Synchronization Event.

	EPwm7Regs.TBCTL2.bit.PRDLDSYNC = 1;				// TBPRD Load from Shadow: 0->TBCTR=0  1->TBCTR=0 OR SYNC  2->SYNC

	EPwm7Regs.TBPRD = EPWM_TBPRD_INITIAL;			// Set Count Period
	EPwm7Regs.TBPHS.bit.TBPHS = 0x0000;				// Phase is 0
	EPwm7Regs.TBCTR = 0x0000;						// Clear counter

	//------------------------------ (CC) Setup Shadow Register£¨CMPA¡¢CMPB£©load on ZERO
	EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		// CC_SHADOW, CC_IMMEDIATE
	EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	// CC_CTR_ZERO, CC_CTR_PRD, CC_CTR_ZERO_PRD, CC_LD_DISABLE
	EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	// .LOADBMODE : Hase No Effect in Immediate Mode

	//------------------------------ (CC) Setup Compare values£¨CMPA¡¢CMPB£©
	EPwm7Regs.CMPA.bit.CMPA = EPWM7_CMPA_TRIGGER;
	EPwm7Regs.CMPB.bit.CMPB = EPWM7_CMPB_TRIGGER;

	//------------------------------ (AQ) Setup Actions	   Set Points£ºZRO, PRD, CAU, CAD, CBU, CBD
	EPwm7Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
	EPwm7Regs.AQCTLA.bit.PRD = AQ_CLEAR;
	EPwm7Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm7Regs.AQCTLA.bit.CBU = AQ_CLEAR;

	EPwm7Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm7Regs.AQCTLB.bit.PRD = AQ_CLEAR;
	EPwm7Regs.AQCTLB.bit.CAU = AQ_SET;
	EPwm7Regs.AQCTLB.bit.CBU = AQ_CLEAR;

	//------------------------------ (DB) Setup Dead-Band Generator

	//------------------------------ (ET) Setup Interrupt	   Where we will change the Compare Values
	EPwm7Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;		// INT Event: ET_CTR_ZERO, ET_CTR_PRD, ET_CTR_PRDZERO
													// 			  ET_CTRU_CMPA,ET_CTRD_CMPA,ET_CTRU_CMPB,ET_CTRD_CMPB
	EPwm7Regs.ETSEL.bit.INTEN = 0;					// 1 -> INT Enable,					0 -> INT Disable
	EPwm7Regs.ETSEL.bit.INTSELCMP = 0;				// 0-> INT Trigger by CMPA/CMPB     1-> INT Trigger by CMPC/CMPD
	EPwm7Regs.ETPS.bit.INTPRD = ET_1ST;				// INT Generate on 1st event

	//------------------------------ (PC) Setup PWM Chopper
	EPwm7Regs.PCCTL.bit.CHPEN = 0;					// CHPEN = 1;
	EPwm7Regs.PCCTL.bit.OSHTWTH = 0xF;				// One-shot pulse width = N ¡Á EPWMCLK*8£»N =£¨0x0~0xF£©
	EPwm7Regs.PCCTL.bit.CHPFREQ = CHP_DIV8;			// Chopping clock frequency = EPWMCLK/N/8£»N =£¨CHP_DIV1~CHP_DIV8£©
	EPwm7Regs.PCCTL.bit.CHPDUTY = CHP4_8TH;			// Chopping clock Duty cycle CHP1_8TH£¨12.5%£©~ CHP7_8TH£¨87.5%£©

	//------------------------------ (TZ) Setup

	//------------------------------ (DC) Setup

	//------------------------------ EPWMx Link
	EPwm7Regs.EPWMXLINK.bit.TBPRDLINK = 0;			// TBPRD   Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12
//	EPwm7Regs.EPWMXLINK.bit.GLDCTL2LINK = 0;		// GLDCTL2 Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12

	//------------------------------ Global ReLoad
/*	EALLOW;

	EPwm7Regs.GLDCTL.bit.GLD = 1;					// the Global Load Mode :     1 -> Enable        0 -> Disable
	EPwm7Regs.GLDCTL.bit.GLDPRD = 1;				// How Many Selected events : 001 -> 1 event  ~  111 -> 7 event
	EPwm7Regs.GLDCTL.bit.GLDMODE = 1;				// Global Load on LoadCounter = Period

	EPwm7Regs.GLDCFG.bit.CMPA_CMPAHR = 1;
	EPwm7Regs.GLDCFG.bit.CMPB_CMPBHR = 1;
	EPwm7Regs.GLDCFG.bit.TBPRD_TBPRDHR = 1;

	EDIS;
*/
}

void usr_ePWM8_Setup(void)
{
	//------------------------------ (TB) Setup TBCLK
	EPwm8Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// Pre-Scale of High Speed Time Base Clock
	EPwm8Regs.TBCTL.bit.CLKDIV = TB_DIV1;			// Pre-Scale of Time Base Clock

	EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;		// Count Mode:	 TB_COUNT_UP, TB_COUNT_DOWN, TB_COUNT_UPDOWN, TB_FREEZE
	EPwm8Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		// Sync Output:	 TB_SYNC_IN(External Sync),  ZERO,  CMPB,  Disable
	EPwm8Regs.TBCTL.bit.PRDLD = TB_SHADOW;			// SHADOW Mode:	 0->TB_SHADOW ( Load On TBCTR=0 )   1->TB_IMMEDIATE

	EPwm8Regs.TBCTL.bit.PHSEN = TB_DISABLE;			// TBCTR Load from TBPHS:  0->TB_DISABLE   1->TB_ENABLE
	EPwm8Regs.TBCTL.bit.PHSDIR = 1;					// Count-Up after Synchronization Event.

	EPwm8Regs.TBCTL2.bit.PRDLDSYNC = 1;				// TBPRD Load from Shadow: 0->TBCTR=0  1->TBCTR=0 OR SYNC  2->SYNC

	EPwm8Regs.TBPRD = EPWM_TBPRD_INITIAL;			// Set Count Period
	EPwm8Regs.TBPHS.bit.TBPHS = 0x0000;				// Phase is 0
	EPwm8Regs.TBCTR = 0x0000;						// Clear counter

	//------------------------------ (CC) Setup Shadow Register£¨CMPA¡¢CMPB£©load on ZERO
	EPwm8Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		// CC_SHADOW, CC_IMMEDIATE
	EPwm8Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm8Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	// CC_CTR_ZERO, CC_CTR_PRD, CC_CTR_ZERO_PRD, CC_LD_DISABLE
	EPwm8Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	// .LOADBMODE : Hase No Effect in Immediate Mode

	//------------------------------ (CC) Setup Compare values£¨CMPA¡¢CMPB£©
	EPwm8Regs.CMPA.bit.CMPA = EPWM8_CMPA_TRIGGER;
	EPwm8Regs.CMPB.bit.CMPB = EPWM8_CMPB_TRIGGER;

	//------------------------------ (AQ) Setup Actions	   Set Points£ºZRO, PRD, CAU, CAD, CBU, CBD
	EPwm8Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
	EPwm8Regs.AQCTLA.bit.PRD = AQ_CLEAR;
	EPwm8Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm8Regs.AQCTLA.bit.CBU = AQ_CLEAR;

	EPwm8Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm8Regs.AQCTLB.bit.PRD = AQ_CLEAR;
	EPwm8Regs.AQCTLB.bit.CAU = AQ_SET;
	EPwm8Regs.AQCTLB.bit.CBU = AQ_CLEAR;

	//------------------------------ (DB) Setup Dead-Band Generator

	//------------------------------ (ET) Setup Interrupt	   Where we will change the Compare Values
	EPwm8Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;		// INT Event: ET_CTR_ZERO, ET_CTR_PRD, ET_CTR_PRDZERO
													// 			  ET_CTRU_CMPA,ET_CTRD_CMPA,ET_CTRU_CMPB,ET_CTRD_CMPB
	EPwm8Regs.ETSEL.bit.INTEN = 0;					// 1 -> INT Enable,					0 -> INT Disable
	EPwm8Regs.ETSEL.bit.INTSELCMP = 0;				// 0-> INT Trigger by CMPA/CMPB     1-> INT Trigger by CMPC/CMPD
	EPwm8Regs.ETPS.bit.INTPRD = ET_1ST;				// INT Generate on 1st event

	//------------------------------ (PC) Setup PWM Chopper
	EPwm8Regs.PCCTL.bit.CHPEN = 0;					// CHPEN = 1;
	EPwm8Regs.PCCTL.bit.OSHTWTH = 0xF;				// One-shot pulse width = N ¡Á EPWMCLK*8£»N =£¨0x0~0xF£©
	EPwm8Regs.PCCTL.bit.CHPFREQ = CHP_DIV8;			// Chopping clock frequency = EPWMCLK/N/8£»N =£¨CHP_DIV1~CHP_DIV8£©
	EPwm8Regs.PCCTL.bit.CHPDUTY = CHP4_8TH;			// Chopping clock Duty cycle CHP1_8TH£¨12.5%£©~ CHP7_8TH£¨87.5%£©

	//------------------------------ (TZ) Setup

	//------------------------------ (DC) Setup

	//------------------------------ EPWMx Link
	EPwm8Regs.EPWMXLINK.bit.TBPRDLINK = 0;			// TBPRD   Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12
//	EPwm8Regs.EPWMXLINK.bit.GLDCTL2LINK = 0;		// GLDCTL2 Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12

	//------------------------------ Global ReLoad
/*	EALLOW;

	EPwm8Regs.GLDCTL.bit.GLD = 1;					// the Global Load Mode :     1 -> Enable        0 -> Disable
	EPwm8Regs.GLDCTL.bit.GLDPRD = 1;				// How Many Selected events : 001 -> 1 event  ~  111 -> 7 event
	EPwm8Regs.GLDCTL.bit.GLDMODE = 1;				// Global Load on LoadCounter = Period

	EPwm8Regs.GLDCFG.bit.CMPA_CMPAHR = 1;
	EPwm8Regs.GLDCFG.bit.CMPB_CMPBHR = 1;
	EPwm8Regs.GLDCFG.bit.TBPRD_TBPRDHR = 1;

	EDIS;
*/
}

void usr_ePWM9_Setup(void)
{
	//------------------------------ (TB) Setup TBCLK
	EPwm9Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// Pre-Scale of High Speed Time Base Clock
	EPwm9Regs.TBCTL.bit.CLKDIV = TB_DIV1;			// Pre-Scale of Time Base Clock

	EPwm9Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;		// Count Mode:	 TB_COUNT_UP, TB_COUNT_DOWN, TB_COUNT_UPDOWN, TB_FREEZE
	EPwm9Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		// Sync Output:	 TB_SYNC_IN(External Sync),  ZERO,  CMPB,  Disable
	EPwm9Regs.TBCTL.bit.PRDLD = TB_SHADOW;			// SHADOW Mode:	 0->TB_SHADOW ( Load On TBCTR=0 )   1->TB_IMMEDIATE

	EPwm9Regs.TBCTL.bit.PHSEN = TB_DISABLE;			// TBCTR Load from TBPHS:  0->TB_DISABLE   1->TB_ENABLE
	EPwm9Regs.TBCTL.bit.PHSDIR = 1;					// Count-Up after Synchronization Event.

	EPwm9Regs.TBCTL2.bit.PRDLDSYNC = 1;				// TBPRD Load from Shadow: 0->TBCTR=0  1->TBCTR=0 OR SYNC  2->SYNC

	EPwm9Regs.TBPRD = EPWM_TBPRD_INITIAL;			// Set Count Period
	EPwm9Regs.TBPHS.bit.TBPHS = 0x0000;				// Phase is 0
	EPwm9Regs.TBCTR = 0x0000;						// Clear counter

	//------------------------------ (CC) Setup Shadow Register£¨CMPA¡¢CMPB£©load on ZERO
	EPwm9Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		// CC_SHADOW, CC_IMMEDIATE
	EPwm9Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm9Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	// CC_CTR_ZERO, CC_CTR_PRD, CC_CTR_ZERO_PRD, CC_LD_DISABLE
	EPwm9Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	// .LOADBMODE : Hase No Effect in Immediate Mode

	//------------------------------ (CC) Setup Compare values£¨CMPA¡¢CMPB£©
	EPwm9Regs.CMPA.bit.CMPA = EPWM9_CMPA_TRIGGER;
	EPwm9Regs.CMPB.bit.CMPB = EPWM9_CMPB_TRIGGER;

	//------------------------------ (AQ) Setup Actions	   Set Points£ºZRO, PRD, CAU, CAD, CBU, CBD
	EPwm9Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
	EPwm9Regs.AQCTLA.bit.PRD = AQ_CLEAR;
	EPwm9Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm9Regs.AQCTLA.bit.CBU = AQ_CLEAR;

	EPwm9Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm9Regs.AQCTLB.bit.PRD = AQ_CLEAR;
	EPwm9Regs.AQCTLB.bit.CAU = AQ_SET;
	EPwm9Regs.AQCTLB.bit.CBU = AQ_CLEAR;

	//------------------------------ (DB) Setup Dead-Band Generator

	//------------------------------ (ET) Setup Interrupt	   Where we will change the Compare Values
	EPwm9Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;		// INT Event: ET_CTR_ZERO, ET_CTR_PRD, ET_CTR_PRDZERO
													// 			  ET_CTRU_CMPA,ET_CTRD_CMPA,ET_CTRU_CMPB,ET_CTRD_CMPB
	EPwm9Regs.ETSEL.bit.INTEN = 0;					// 1 -> INT Enable,					0 -> INT Disable
	EPwm9Regs.ETSEL.bit.INTSELCMP = 0;				// 0-> INT Trigger by CMPA/CMPB     1-> INT Trigger by CMPC/CMPD
	EPwm9Regs.ETPS.bit.INTPRD = ET_1ST;				// INT Generate on 1st event

	//------------------------------ (PC) Setup PWM Chopper
	EPwm9Regs.PCCTL.bit.CHPEN = 0;					// CHPEN = 1;
	EPwm9Regs.PCCTL.bit.OSHTWTH = 0xF;				// One-shot pulse width = N ¡Á EPWMCLK*8£»N =£¨0x0~0xF£©
	EPwm9Regs.PCCTL.bit.CHPFREQ = CHP_DIV8;			// Chopping clock frequency = EPWMCLK/N/8£»N =£¨CHP_DIV1~CHP_DIV8£©
	EPwm9Regs.PCCTL.bit.CHPDUTY = CHP4_8TH;			// Chopping clock Duty cycle CHP1_8TH£¨12.5%£©~ CHP7_8TH£¨87.5%£©

	//------------------------------ (TZ) Setup

	//------------------------------ (DC) Setup

	//------------------------------ EPWMx Link
	EPwm9Regs.EPWMXLINK.bit.TBPRDLINK = 0;			// TBPRD   Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12
//	EPwm9Regs.EPWMXLINK.bit.GLDCTL2LINK = 0;		// GLDCTL2 Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12

	//------------------------------ Global ReLoad
/*	EALLOW;

	EPwm9Regs.GLDCTL.bit.GLD = 1;					// the Global Load Mode :     1 -> Enable        0 -> Disable
	EPwm9Regs.GLDCTL.bit.GLDPRD = 1;				// How Many Selected events : 001 -> 1 event  ~  111 -> 7 event
	EPwm9Regs.GLDCTL.bit.GLDMODE = 1;				// Global Load on LoadCounter = Period

	EPwm9Regs.GLDCFG.bit.CMPA_CMPAHR = 1;
	EPwm9Regs.GLDCFG.bit.CMPB_CMPBHR = 1;
	EPwm9Regs.GLDCFG.bit.TBPRD_TBPRDHR = 1;

	EDIS;
*/
}

//--------------------------------------------------------------------------------
// Used for Task Trigger
//--------------------------------------------------------------------------------
void usr_ePWM10_Setup(void)
{
	//------------------------------ (TB) Setup TBCLK
	EPwm10Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		// Pre-Scale of High Speed Time Base Clock
	EPwm10Regs.TBCTL.bit.CLKDIV = TB_DIV1;			// Pre-Scale of Time Base Clock

	EPwm10Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;		// Count Mode:	 TB_COUNT_UP, TB_COUNT_DOWN, TB_COUNT_UPDOWN, TB_FREEZE
	EPwm10Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		// Sync Output:	 TB_SYNC_IN(External Sync),  ZERO,  CMPB,  Disable
	EPwm10Regs.TBCTL.bit.PRDLD = TB_SHADOW;			// SHADOW Mode:	 0->TB_SHADOW ( Load On TBCTR=0 )   1->TB_IMMEDIATE

	EPwm10Regs.TBCTL.bit.PHSEN = TB_DISABLE;		// TBCTR Load from TBPHS:  0->TB_DISABLE   1->TB_ENABLE
	EPwm10Regs.TBCTL.bit.PHSDIR = 1;				// Count-Up after Synchronization Event.

	EPwm10Regs.TBCTL2.bit.PRDLDSYNC = 1;			// TBPRD Load from Shadow: 0->TBCTR=0  1->TBCTR=0 OR SYNC  2->SYNC

	EPwm10Regs.TBPRD = EPWM_TBPRD_INITIAL;			// Set Count Period
	EPwm10Regs.TBPHS.bit.TBPHS = 0x0000;			// Phase is 0
	EPwm10Regs.TBCTR = 0x0000;						// Clear counter

	//------------------------------ (CC) Setup Shadow Register£¨CMPA¡¢CMPB£©load on ZERO
	EPwm10Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;	// CC_SHADOW, CC_IMMEDIATE
	EPwm10Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm10Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	// CC_CTR_ZERO, CC_CTR_PRD, CC_CTR_ZERO_PRD, CC_LD_DISABLE
	EPwm10Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	// .LOADBMODE : Hase No Effect in Immediate Mode

	//------------------------------ (CC) Setup Compare values£¨CMPA¡¢CMPB£©
	EPwm10Regs.CMPA.bit.CMPA = EPWM10_CMPA_TRIGGER;
	EPwm10Regs.CMPB.bit.CMPB = EPWM10_CMPA_TRIGGER;

	//------------------------------ (AQ) Setup Actions	   Set Points£ºZRO, PRD, CAU, CAD, CBU, CBD
	EPwm10Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
	EPwm10Regs.AQCTLA.bit.PRD = AQ_CLEAR;
	EPwm10Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm10Regs.AQCTLA.bit.CBU = AQ_CLEAR;

	EPwm10Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm10Regs.AQCTLB.bit.PRD = AQ_CLEAR;
	EPwm10Regs.AQCTLB.bit.CAU = AQ_SET;
	EPwm10Regs.AQCTLB.bit.CBU = AQ_CLEAR;

	//------------------------------ (DB) Setup Dead-Band Generator

	//------------------------------ (ET) Setup Interrupt	   Where we will change the Compare Values
	EPwm10Regs.ETSEL.bit.INTSEL = ET_CTRU_CMPA;		// INT Event: ET_CTR_ZERO, ET_CTR_PRD, ET_CTR_PRDZERO
													// 			  ET_CTRU_CMPA,ET_CTRD_CMPA,ET_CTRU_CMPB,ET_CTRD_CMPB
	EPwm10Regs.ETSEL.bit.INTEN = 1;					// 1 -> INT Enable,					0 -> INT Disable
	EPwm10Regs.ETSEL.bit.INTSELCMP = 0;				// 0-> INT Trigger by CMPA/CMPB     1-> INT Trigger by CMPC/CMPD
	EPwm10Regs.ETPS.bit.INTPRD = ET_1ST;			// INT Generate on 1st event

	//------------------------------ (PC) Setup PWM Chopper
	EPwm10Regs.PCCTL.bit.CHPEN = 0;					// CHPEN = 1;
	EPwm10Regs.PCCTL.bit.OSHTWTH = 0xF;				// One-shot pulse width = N ¡Á EPWMCLK*8£»N =£¨0x0~0xF£©
	EPwm10Regs.PCCTL.bit.CHPFREQ = CHP_DIV8;		// Chopping clock frequency = EPWMCLK/N/8£»N =£¨CHP_DIV1~CHP_DIV8£©
	EPwm10Regs.PCCTL.bit.CHPDUTY = CHP4_8TH;		// Chopping clock Duty cycle CHP1_8TH£¨12.5%£©~ CHP7_8TH£¨87.5%£©

	//------------------------------ (TZ) Setup

	//------------------------------ (DC) Setup

	//------------------------------ EPWMx Link
	EPwm3Regs.EPWMXLINK.bit.TBPRDLINK = 0;			// TBPRD   Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12
//	EPwm3Regs.EPWMXLINK.bit.GLDCTL2LINK = 0;		// GLDCTL2 Link to : 0x0000->ePwm1 ~ 0x1011->ePwm12

	//------------------------------ Global ReLoad
/*	EALLOW;

	EPwm3Regs.GLDCTL.bit.GLD = 1;					// the Global Load Mode :     1 -> Enable        0 -> Disable
	EPwm3Regs.GLDCTL.bit.GLDPRD = 1;				// How Many Selected events : 001 -> 1 event  ~  111 -> 7 event
	EPwm3Regs.GLDCTL.bit.GLDMODE = 1;				// Global Load on LoadCounter = Period

	EPwm3Regs.GLDCFG.bit.CMPA_CMPAHR = 1;
	EPwm3Regs.GLDCFG.bit.CMPB_CMPBHR = 1;
	EPwm3Regs.GLDCFG.bit.TBPRD_TBPRDHR = 1;

	EDIS;
*/
}

//--------------------------------------------------------------------------------
__interrupt void ISR_ePWM1(void)
{


	EPwm1Regs.ETCLR.bit.INT = 1;					// Clear INT flag for this timer
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;			// Acknowledge this interrupt to receive more interrupts from group 3
}

__interrupt void ISR_ePWM2(void)
{


	EPwm2Regs.ETCLR.bit.INT = 1;					// Clear INT flag for this timer
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;			// Acknowledge this interrupt to receive more interrupts from group 3
}

__interrupt void ISR_ePWM3(void)
{


	EPwm3Regs.ETCLR.bit.INT = 1;					// Clear INT flag for this timer
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;			// Acknowledge this interrupt to receive more interrupts from group 3	
}

//--------------------------------------------------------------------------------
__interrupt void ISR_ePWM10(void)
{
	#ifndef	EPWM_TEST_MODE							// Do Not Renew Pulse Width
	ePwm_width_2();
	#endif

	EPwm10Regs.ETCLR.bit.INT = 1;					// Clear INT flag for this timer
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;			// Acknowledge this interrupt to receive more interrupts from group 3
}

