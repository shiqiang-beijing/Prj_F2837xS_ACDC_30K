
//--------------------------------------------------------------------------------
#include	"F28x_Project.h"

//--------------------------------------------------------------------------------
#include	"usr_Global_Data.h"
#include	"usr_ePWM.h"

//--------------------------------------------------------------------------------
// System Control									Function Switch
//--------------------------------------------------------------------------------
volatile	Uint16	En_SciB_Send = 0;

volatile	Uint16	En_Buzzer = 0;
volatile	Uint16	Cnt_Buzzer = 0;

//--------------------------------------------------------------------------------
// System Control									Coefficient
//--------------------------------------------------------------------------------
volatile	float	Rate_Uac_Pwm = 3.50;			// Sampled UAC ( Instant Value ) to Pulse Width

volatile	float	Rate_UiA_Pwm = 3.50;
volatile	float	Rate_UiB_Pwm = 3.50;
volatile	float	Rate_UiC_Pwm = 3.50;

//--------------------------------------------------------------------------------
volatile	float	Rate_RMS_UAB = 0.4005;			// Coefficient of Calculation : RMS_U_AC
volatile	float	Rate_RMS_UBC = 0.4000;
volatile	float	Rate_RMS_UCA = 0.3995;

volatile	float	Rate_RMS_IAB = 0.0365;			// Coefficient of Calculation : RMS_I_AC
volatile	float	Rate_RMS_IBC = 0.0360;
volatile	float	Rate_RMS_ICA = 0.0360;

volatile	float	Rate_RMS_UDC = 0.375;

volatile	float	Rate_RMS_IDC = 0.375;

//--------------------------------------------------------------------------------
volatile	Uint16	ePwm_Width_A = EPWM_CMP_MIN;
volatile	Uint16	ePwm_Width_B = EPWM_CMP_MIN;
volatile	Uint16	ePwm_Width_C = EPWM_CMP_MIN;

volatile	Uint16	ePwm_Width_Min = EPWM_CMP_MIN;
volatile	Uint16	ePwm_Width_Max = EPWM_CMP_MAX;

//--------------------------------------------------------------------------------
// System Parameters								Time
//--------------------------------------------------------------------------------
volatile	Uint16	WorkingTime_S = 0;				// Second
volatile	Uint16	WorkingTime_M = 0;				// Minute
volatile	Uint16	WorkingTime_H = 0;				// Hour

//--------------------------------------------------------------------------------
// System Parameters								DC
//--------------------------------------------------------------------------------
float	Uo1 = 0;									// Voltage of DC Output
float	Uo2 = 0;									// Voltage of DC Output Filter

float	Uo1_Avg[RMS_AVG_NUM_UDC];					// Voltage of DC Output Array ( For Average Calculation )
float	Uo2_Avg[RMS_AVG_NUM_UDC];
Uint16	i_Uo1 = 0;									// Counter for Average Calculation
Uint16	i_Uo2 = 0;

//--------------------------------------------------------------------------------
float	Iout = 0;									// Current of Output

float	Iout_Avg[RMS_AVG_NUM_UDC];					// Current of Output Array ( For Average Calculation )
Uint16	i_Iout = 0;									// Counter for Average Calculation

//--------------------------------------------------------------------------------
// System Parameters								AC ( Voltage )
//--------------------------------------------------------------------------------
float	UiA_Rms = 0;								// Ui RMS
float	UiB_Rms = 0;
float	UiC_Rms = 0;

float	UiA_Avg[RMS_AVG_NUM_UAC];					// Ui RMS Array ( For Average Calculation )
float	UiB_Avg[RMS_AVG_NUM_UAC];
float	UiC_Avg[RMS_AVG_NUM_UAC];
Uint16	i_UiA = 0;									// Counter for Average Calculation
Uint16	i_UiB = 0;
Uint16	i_UiC = 0;

//--------------------------------------------------------------------------------
float	Ui_Rms_Mean = 0;							// Ui RMS -> Mean Value ( 3 Phase )

Uint16	UiA_RmsNew = 0;								// Ui RMS ReNew
Uint16	UiB_RmsNew = 0;
Uint16	UiC_RmsNew = 0;

//--------------------------------------------------------------------------------
// System Parameters								AC ( Current )
//--------------------------------------------------------------------------------
float	IiA_Rms = 0;								// Ii RMS
float	IiB_Rms = 0;
float	IiC_Rms = 0;

float	IiA_Avg[RMS_AVG_NUM_IAC];					// Ii RMS Array ( For Average Calculation )
float	IiB_Avg[RMS_AVG_NUM_IAC];
float	IiC_Avg[RMS_AVG_NUM_IAC];
Uint16	i_IiA = 0;									// Counter for Average Calculation
Uint16	i_IiB = 0;
Uint16	i_IiC = 0;

//--------------------------------------------------------------------------------
float	Ii_Rms_Mean = 0;							// Ii RMS -> Mean Value  ( 3 Phase )

//--------------------------------------------------------------------------------
// System Parameters								AC ( Frequency )
//--------------------------------------------------------------------------------
float	UiA_Prd = 0;								// Ui Period
float	UiB_Prd = 0;
float	UiC_Prd = 0;

float	UiA_Frq = 0;								// Ui Frequency
float	UiB_Frq = 0;
float	UiC_Frq = 0;

//--------------------------------------------------------------------------------
Uint32	CapData_UiA_Mean = 0;						// Mean Value of Captured DataArry
Uint32	CapData_UiB_Mean = 0;
Uint32	CapData_UiC_Mean = 0;

Uint16	CapMean_UiA_Renew = 0;						// Mean Value Calculation Renew
Uint16	CapMean_UiB_Renew = 0;
Uint16	CapMean_UiC_Renew = 0;

Uint16	CapData_Uin_Mean = 0;

//--------------------------------------------------------------------------------
// System Parameters								Power
//--------------------------------------------------------------------------------
float	Power_In  = 0;
float	Power_Out = 0;
float	Power_Efficiency = 1;

//--------------------------------------------------------------------------------
// Sampled Data : Capture
//--------------------------------------------------------------------------------
volatile	Uint32	CapData_UiA[BUFFER_SIZE_CAP];	// Captured DataArry for Ui
volatile	Uint16	CapData_UiA_Index = 0;
volatile	Uint16	CapData_UiA_Full = 0;

volatile	Uint32	CapData_UiB[BUFFER_SIZE_CAP];
volatile	Uint16	CapData_UiB_Index = 0;
volatile	Uint16	CapData_UiB_Full = 0;

volatile	Uint32	CapData_UiC[BUFFER_SIZE_CAP];
volatile	Uint16	CapData_UiC_Index = 0;
volatile	Uint16	CapData_UiC_Full = 0;

volatile	Uint32	CapData_UiA_Error = 0;			// Error Times Accumulator
volatile	Uint32	CapData_UiB_Error = 0;
volatile	Uint32	CapData_UiC_Error = 0;

//--------------------------------------------------------------------------------
// Sampled Data : ADC
//--------------------------------------------------------------------------------
volatile	Uint16	UiA_Trans = 0;					// ADC Sampled Data 
volatile	Uint16	UiB_Trans = 0;
volatile	Uint16	UiC_Trans = 0;

volatile	Uint16	IiA_Trans = 0;
volatile	Uint16	IiB_Trans = 0;
volatile	Uint16	IiC_Trans = 0;

volatile	Uint16	Uo1_Trans = 0;
volatile	Uint16	Uo2_Trans = 0;

volatile	Uint16	Io_Trans = 0;

//--------------------------------------------------------------------------------
volatile	Uint16	AdcResult_UiA[BUFFER_SIZE_UAC];	// Ui Instantaneous Value Array ( For RMS Calculation )
volatile	Uint16	AdcResult_UiA_Index = 0;
volatile	Uint16	AdcBuffer_UiA_Full = 0;

volatile	Uint16	AdcResult_UiB[BUFFER_SIZE_UAC];
volatile	Uint16	AdcResult_UiB_Index = 0;
volatile	Uint16	AdcBuffer_UiB_Full = 0;

volatile	Uint16	AdcResult_UiC[BUFFER_SIZE_UAC];
volatile	Uint16	AdcResult_UiC_Index = 0;
volatile	Uint16	AdcBuffer_UiC_Full = 0;

//--------------------------------------------------------------------------------
volatile	Uint16	AdcResult_IiA[BUFFER_SIZE_IAC];	// Ii Instantaneous Value Array ( For RMS Calculation )
volatile	Uint16	AdcResult_IiA_Index = 0;
volatile	Uint16	AdcBuffer_IiA_Full = 0;

volatile	Uint16	AdcResult_IiB[BUFFER_SIZE_IAC];
volatile	Uint16	AdcResult_IiB_Index = 0;
volatile	Uint16	AdcBuffer_IiB_Full = 0;

volatile	Uint16	AdcResult_IiC[BUFFER_SIZE_IAC];
volatile	Uint16	AdcResult_IiC_Index = 0;
volatile	Uint16	AdcBuffer_IiC_Full = 0;

//--------------------------------------------------------------------------------
volatile	Uint16	AdcResult_Uo1[BUFFER_SIZE_UDC];
volatile	Uint16	AdcResult_Uo1_Index = 0;
volatile	Uint16	AdcBuffer_Uo1_Full = 0;

volatile	Uint16	AdcResult_Uo2[BUFFER_SIZE_UDC];
volatile	Uint16	AdcResult_Uo2_Index = 0;
volatile	Uint16	AdcBuffer_Uo2_Full = 0;

volatile	Uint16	AdcResult_Io[BUFFER_SIZE_IDC];
volatile	Uint16	AdcResult_Io_Index = 0;
volatile	Uint16	AdcBuffer_Io_Full = 0;

//--------------------------------------------------------------------------------
// Data Table
//--------------------------------------------------------------------------------
volatile	float	Table_Control[TABLE_SIZE_SIN];

const		float	Table_Original[TABLE_SIZE_SIN];



