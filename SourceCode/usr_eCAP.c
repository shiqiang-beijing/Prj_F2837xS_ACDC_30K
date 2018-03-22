
//--------------------------------------------------------------------------------
#include	"F28x_Project.h"

//--------------------------------------------------------------------------------
#include	"usr_Gpio.h"
#include	"usr_eCAP.h"

#include	"usr_Global_Data.h"

//--------------------------------------------------------------------------------
#ifdef	_FLASH
#pragma CODE_SECTION(ISR_eCAP1, ".TI.ramfunc");
#pragma CODE_SECTION(ISR_eCAP2, ".TI.ramfunc");
#pragma CODE_SECTION(ISR_eCAP3, ".TI.ramfunc");
#endif

//--------------------------------------------------------------------------------
void usr_eCAP_Initial(void)
{
	InitECap1Gpio(CAP_GPIO_UAC_A);										// eCAP1
	GPIO_SetupPinOptions(CAP_GPIO_UAC_A, GPIO_INPUT, GPIO_ASYNC);

	InitECap2Gpio(CAP_GPIO_UAC_B);										// eCAP2
	GPIO_SetupPinOptions(CAP_GPIO_UAC_B, GPIO_INPUT, GPIO_ASYNC);

	InitECap3Gpio(CAP_GPIO_UAC_C);										// eCAP3
	GPIO_SetupPinOptions(CAP_GPIO_UAC_C, GPIO_INPUT, GPIO_ASYNC);
}

//--------------------------------------------------------------------------------
void usr_eCAP1_Setup(void)
{
	//------------------------------ Working Stop
	ECap1Regs.ECEINT.all = 0x0000;					// Disable All CAP _interrupt
	ECap1Regs.ECCLR.all = 0xFFFF;					// Clear   All CAP _interrupt Flag

	ECap1Regs.ECCTL1.bit.CAPLDEN = 0;				// Disable CAP Register Load
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;				// Stop TSCTR Counting

	//------------------------------ Working Mode
	ECap1Regs.ECCTL1.bit.PRESCALE = 0;				// 0h -> Divide by 1;  1h -> Divide by 2;  2h -> Divide by 4

	ECap1Regs.ECCTL1.bit.CAP1POL = 0;       		// Capture Event Triggered on :   0->Rising Edge   1->Falling Edge
	ECap1Regs.ECCTL1.bit.CAP2POL = 0;
	ECap1Regs.ECCTL1.bit.CAP3POL = 0;
	ECap1Regs.ECCTL1.bit.CAP4POL = 0;

	ECap1Regs.ECCTL1.bit.CTRRST1 = 1;      			 // Capture Event Reset the Counter : 0->No  1->Yes
	ECap1Regs.ECCTL1.bit.CTRRST2 = 1;
	ECap1Regs.ECCTL1.bit.CTRRST3 = 1;
	ECap1Regs.ECCTL1.bit.CTRRST4 = 1;

	ECap1Regs.ECCTL2.bit.CAP_APWM = 0;				// Work Mode Select : 0->eCAP        1->APWM
	ECap1Regs.ECCTL2.bit.CONT_ONESHT = 0;			// Work Mode Select : 0->Continuous  1->One-Shot

	ECap1Regs.ECCTL2.bit.SYNCI_EN = 0;				// Sync-In : Enable   0->Disable     1->Enable
	ECap1Regs.ECCTL2.bit.SYNCO_SEL = 2;				// Sync-Out  Select : 0->Sync-In     1->CTR = PRD    2/3->Disable Sync-Out

	//------------------------------ INT Trigger
	ECap1Regs.ECEINT.bit.CEVT1 = 1;					// INI Triggered By Capture Event 1~4 :  1->Enable  0->Disable
	ECap1Regs.ECEINT.bit.CEVT2 = 0;
	ECap1Regs.ECEINT.bit.CEVT3 = 0;
	ECap1Regs.ECEINT.bit.CEVT4 = 0;

	//------------------------------ Working Start
	ECap1Regs.ECCTL2.bit.REARM = 1;
	ECap1Regs.ECCTL1.bit.CAPLDEN = 1;				// Enable Capture
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;				// Start TSCTR Counting
}

void usr_eCAP2_Setup(void)
{
	//------------------------------ Working Stop
	ECap2Regs.ECEINT.all = 0x0000;					// Disable All CAP _interrupt
	ECap2Regs.ECCLR.all = 0xFFFF;					// Clear   All CAP _interrupt Flag

	ECap2Regs.ECCTL1.bit.CAPLDEN = 0;				// Disable CAP Register Load
	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0;				// Stop TSCTR Counting

	//------------------------------ Working Mode
	ECap2Regs.ECCTL1.bit.PRESCALE = 0;				// 0h -> Divide by 1;  1h -> Divide by 2;  2h -> Divide by 4

	ECap2Regs.ECCTL1.bit.CAP1POL = 0;       		// Capture Event Triggered on :   0->Rising Edge   1->Falling Edge
	ECap2Regs.ECCTL1.bit.CAP2POL = 0;
	ECap2Regs.ECCTL1.bit.CAP3POL = 0;
	ECap2Regs.ECCTL1.bit.CAP4POL = 0;

	ECap2Regs.ECCTL1.bit.CTRRST1 = 1;      			 // Capture Event Reset the Counter : 0->No  1->Yes
	ECap2Regs.ECCTL1.bit.CTRRST2 = 1;
	ECap2Regs.ECCTL1.bit.CTRRST3 = 1;
	ECap2Regs.ECCTL1.bit.CTRRST4 = 1;

	ECap2Regs.ECCTL2.bit.CAP_APWM = 0;				// Work Mode Select : 0->eCAP        1->APWM
	ECap2Regs.ECCTL2.bit.CONT_ONESHT = 0;			// Work Mode Select : 0->Continuous  1->One-Shot

	ECap2Regs.ECCTL2.bit.SYNCI_EN = 0;				// Sync-In : Enable   0->Disable     1->Enable
	ECap2Regs.ECCTL2.bit.SYNCO_SEL = 2;				// Sync-Out  Select : 0->Sync-In     1->CTR = PRD    2/3->Disable Sync-Out

	//------------------------------ INT Trigger
	ECap2Regs.ECEINT.bit.CEVT1 = 1;					// INI Triggered By Capture Event 1~4 :  1->Enable  0->Disable
	ECap2Regs.ECEINT.bit.CEVT2 = 0;
	ECap2Regs.ECEINT.bit.CEVT3 = 0;
	ECap2Regs.ECEINT.bit.CEVT4 = 0;

	//------------------------------ Working Start
	ECap2Regs.ECCTL2.bit.REARM = 1;
	ECap2Regs.ECCTL1.bit.CAPLDEN = 1;				// Enable Capture
	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;				// Start TSCTR Counting
}

void usr_eCAP3_Setup(void)
{
	//------------------------------ Working Stop
	ECap3Regs.ECEINT.all = 0x0000;					// Disable All CAP _interrupt
	ECap3Regs.ECCLR.all = 0xFFFF;					// Clear   All CAP _interrupt Flag

	ECap3Regs.ECCTL1.bit.CAPLDEN = 0;				// Disable CAP Register Load
	ECap3Regs.ECCTL2.bit.TSCTRSTOP = 0;				// Stop TSCTR Counting

	//------------------------------ Working Mode
	ECap3Regs.ECCTL1.bit.PRESCALE = 0;				// 0h -> Divide by 1;  1h -> Divide by 2;  2h -> Divide by 4

	ECap3Regs.ECCTL1.bit.CAP1POL = 0;       		// Capture Event Triggered on :   0->Rising Edge   1->Falling Edge
	ECap3Regs.ECCTL1.bit.CAP2POL = 0;
	ECap3Regs.ECCTL1.bit.CAP3POL = 0;
	ECap3Regs.ECCTL1.bit.CAP4POL = 0;

	ECap3Regs.ECCTL1.bit.CTRRST1 = 1;      			 // Capture Event Reset the Counter : 0->No  1->Yes
	ECap3Regs.ECCTL1.bit.CTRRST2 = 1;
	ECap3Regs.ECCTL1.bit.CTRRST3 = 1;
	ECap3Regs.ECCTL1.bit.CTRRST4 = 1;

	ECap3Regs.ECCTL2.bit.CAP_APWM = 0;				// Work Mode Select : 0->eCAP        1->APWM
	ECap3Regs.ECCTL2.bit.CONT_ONESHT = 0;			// Work Mode Select : 0->Continuous  1->One-Shot

	ECap3Regs.ECCTL2.bit.SYNCI_EN = 0;				// Sync-In : Enable   0->Disable     1->Enable
	ECap3Regs.ECCTL2.bit.SYNCO_SEL = 2;				// Sync-Out  Select : 0->Sync-In     1->CTR = PRD    2/3->Disable Sync-Out

	//------------------------------ INT Trigger
	ECap3Regs.ECEINT.bit.CEVT1 = 1;					// INI Triggered By Capture Event 1~4 :  1->Enable  0->Disable
	ECap3Regs.ECEINT.bit.CEVT2 = 0;
	ECap3Regs.ECEINT.bit.CEVT3 = 0;
	ECap3Regs.ECEINT.bit.CEVT4 = 0;

	//------------------------------ Working Start
	ECap3Regs.ECCTL2.bit.REARM = 1;
	ECap3Regs.ECCTL1.bit.CAPLDEN = 1;				// Enable Capture
	ECap3Regs.ECCTL2.bit.TSCTRSTOP = 1;				// Start TSCTR Counting
}

//--------------------------------------------------------------------------------
__interrupt void ISR_eCAP1(void)					// UiA Period Capture
{
	// ------ Get the Data
	if( (ECap1Regs.CAP1<CAP_DATA_UI_MIN)||(ECap1Regs.CAP1>CAP_DATA_UI_MAX) )	// Error Data
	{
		CapData_UiA_Error += 1;

		#ifndef	ERROR_DATA_REMOVE
		CapData_UiA[CapData_UiA_Index++] = ECap1Regs.CAP1;						// Error Data into Array
		#endif
	}
	else
	{
		CapData_UiA[CapData_UiA_Index++] = ECap1Regs.CAP1;						// Right Data into Array
	}

	// ------ Data into Array
	if( CapData_UiA_Index >= BUFFER_SIZE_CAP )
	{
		CapData_UiA_Index = 0;
		CapData_UiA_Full = 1;
	}

	ECap1Regs.ECCLR.bit.CEVT1 = 1;
	ECap1Regs.ECCLR.bit.INT = 1;					// Global Interrupt Clear for eCAP1
	ECap1Regs.ECCTL2.bit.REARM = 1;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;			// Acknowledge this interrupt to receive more interrupts from group 4
}

__interrupt void ISR_eCAP2(void)					// UiB Period Capture
{
	// ------ Get the Data
	if( (ECap2Regs.CAP1<CAP_DATA_UI_MIN)||(ECap2Regs.CAP1>CAP_DATA_UI_MAX) )
	{
		CapData_UiB_Error += 1;

		#ifndef	ERROR_DATA_REMOVE
		CapData_UiB[CapData_UiB_Index++] = ECap2Regs.CAP1;
		#endif
	}
	else
	{
		CapData_UiB[CapData_UiB_Index++] = ECap2Regs.CAP1;
	}

	// ------ Data into Array
	if( CapData_UiB_Index >= BUFFER_SIZE_CAP )
	{
		CapData_UiB_Index = 0;
		CapData_UiB_Full = 1;
	}

	ECap2Regs.ECCLR.bit.CEVT1 = 1;
	ECap2Regs.ECCLR.bit.INT = 1;
	ECap2Regs.ECCTL2.bit.REARM = 1;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}

__interrupt void ISR_eCAP3(void)					// UiC Period Capture
{
	// ------ Get the Data
	if( (ECap3Regs.CAP1<CAP_DATA_UI_MIN)||(ECap3Regs.CAP1>CAP_DATA_UI_MAX) )
	{
		CapData_UiC_Error += 1;

		#ifndef	ERROR_DATA_REMOVE
		CapData_UiC[CapData_UiC_Index++] = ECap3Regs.CAP1;
		#endif
	}
	else
	{
		CapData_UiC[CapData_UiC_Index++] = ECap3Regs.CAP1;
	}

	// ------ Data into Array
	if( CapData_UiC_Index >= BUFFER_SIZE_CAP )
	{
		CapData_UiC_Index = 0;
		CapData_UiC_Full = 1;
	}

	ECap3Regs.ECCLR.bit.CEVT1 = 1;
	ECap3Regs.ECCLR.bit.INT = 1;
	ECap3Regs.ECCTL2.bit.REARM = 1;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}

