
//--------------------------------------------------------------------------------
#include	"F28x_Project.h"

#include	"usr_Global_Data.h"

//--------------------------------------------------------------------------------
__interrupt void ISR_CpuTimer0(void)
{
//	CpuTimer0.InterruptCount++;

	// Acknowledge this interrupt to receive other interrupts of group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;						// Clear Group flag
}

//--------------------------------------------------------------------------------
// Timer : 10 mS
//--------------------------------------------------------------------------------
__interrupt void ISR_CpuTimer1(void)
{
	//------------------------------ WorkingTime Counter
	CpuTimer1.InterruptCount++;

	if( CpuTimer1.InterruptCount > 100 )						// 1 Second
	{
		WorkingTime_S += 1;
		CpuTimer1.InterruptCount = 0;

		if( WorkingTime_S >= 60 )								// 1 Minute
		{
			WorkingTime_S = 0;
			WorkingTime_M += 1;
			if( WorkingTime_M >= 60 )							// 1 Hour
			{
				WorkingTime_M = 0;
				WorkingTime_H += 1;
			}
		}
	}

	//------------------------------ Buzzer
	if( En_Buzzer != 0 )										// If Buzzer Enabled Count the Buzzer Time
	{
		Cnt_Buzzer += 1;
		if( Cnt_Buzzer > 10 )
		{
			En_Buzzer = 0;
			Cnt_Buzzer = 0;
		}
	}


}

//--------------------------------------------------------------------------------
// Timer : 3.0 S
//--------------------------------------------------------------------------------
__interrupt void ISR_CpuTimer2(void)
{
//	CpuTimer2.InterruptCount++;

	//------------------------------ SciB
	if( En_SciB_Send == 0)
	{
		En_SciB_Send = 1;
	}

	//------------------------------ Buzzer
	#ifndef	BUZZER_NOT_USE

	if( En_Buzzer == 0 )										// If Buzzer Closed Enable It
	{
		En_Buzzer = 1;
	}

	#endif

}

