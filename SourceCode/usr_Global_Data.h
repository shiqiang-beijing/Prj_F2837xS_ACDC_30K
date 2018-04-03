
//--------------------------------------------------------------------------------
// Debug Switch
//--------------------------------------------------------------------------------

#ifdef		_RAM_DEBUG									// Debug Set
//
#define		VCM_TEST_MODE								// VCM Test Method :  With 0 input for UiA, Read UiA_Rms, ADC_ZERO_UiA = UiA_Rms
#define		BUZZER_NOT_USE								// No Beeper
//
#endif


//#define		EPWM_TEST_MODE								// 244A OE1 Enable, ePWM_1~3 Pulse Width Fixed Value

//#define		BUZZER_NOT_USE								// No Beeper

//--------------------------------------------------------------------------------
// Compiling Parameter
//--------------------------------------------------------------------------------
#ifdef		_FLASH
//
#define		BUFFER_SIZE_UAC		504
#define		BUFFER_SIZE_IAC		252
//
#define		BUFFER_SIZE_UDC		100
#define		BUFFER_SIZE_IDC		100
//
#define		TABLE_SIZE_SIN		252
//
#endif

#ifdef		_RAM_DEBUG
//
#define		BUFFER_SIZE_UAC		 50
#define		BUFFER_SIZE_IAC		 25
//
#define		BUFFER_SIZE_UDC		 10
#define		BUFFER_SIZE_IDC		 10
//
#define		TABLE_SIZE_SIN		 25
//
#endif

#define		BUFFER_SIZE_CAP		16						// Divide 2^4, Ease Average Value Calculation

#define		RMS_AVG_NUM_UDC		6						// Average Time of UDC Vaule
#define		RMS_AVG_NUM_UAC		6						// Average Time of UAC RMS Vaule
#define		RMS_AVG_NUM_IAC		6						// Average Time of IAC RMS Vaule

#define		NORMAL_VALUE_UAC	30						// Start Working


//--------------------------------------------------------------------------------
// System Control									Function Switch
//--------------------------------------------------------------------------------
extern	volatile	Uint16	En_SciB_Send;

extern	volatile	Uint16	En_Buzzer;
extern	volatile	Uint16	Cnt_Buzzer;

//--------------------------------------------------------------------------------
// System Control									Coefficient
//--------------------------------------------------------------------------------
extern	volatile	float	Rate_Uac_Pwm;

extern	volatile	float	Rate_UiA_Pwm;
extern	volatile	float	Rate_UiB_Pwm;
extern	volatile	float	Rate_UiC_Pwm;

//--------------------------------------------------------------------------------
extern	volatile	float	Rate_RMS_UAB;
extern	volatile	float	Rate_RMS_UBC;
extern	volatile	float	Rate_RMS_UCA;

extern	volatile	float	Rate_RMS_IAB;
extern	volatile	float	Rate_RMS_IBC;
extern	volatile	float	Rate_RMS_ICA;

extern	volatile	float	Rate_RMS_UDC;

extern	volatile	float	Rate_RMS_IDC;

//--------------------------------------------------------------------------------
extern	volatile	Uint16	ePwm_Width_A;
extern	volatile	Uint16	ePwm_Width_B;
extern	volatile	Uint16	ePwm_Width_C;

extern	volatile	Uint16	ePwm_Width_Min;
extern	volatile	Uint16	ePwm_Width_Max;

//--------------------------------------------------------------------------------
// System Parameters								Time
//--------------------------------------------------------------------------------
extern	volatile	Uint16	WorkingTime_S;
extern	volatile	Uint16	WorkingTime_M;
extern	volatile	Uint16	WorkingTime_H;

//--------------------------------------------------------------------------------
// System Parameters								DC
//--------------------------------------------------------------------------------
extern	float	Uo1;
extern	float	Uo2;

extern	float	Uo1_Avg[];
extern	float	Uo2_Avg[];
extern	Uint16	i_Uo1;
extern	Uint16	i_Uo2;

//--------------------------------------------------------------------------------
extern	float	Iout;

extern	float	Iout_Avg[];
extern	Uint16	i_Iout;

//--------------------------------------------------------------------------------
// System Parameters								AC ( Voltage )
//--------------------------------------------------------------------------------
extern	float	UiA_Rms;
extern	float	UiB_Rms;
extern	float	UiC_Rms;

extern	float	UiA_Avg[];
extern	float	UiB_Avg[];
extern	float	UiC_Avg[];
extern	Uint16	i_UiA;
extern	Uint16	i_UiB;
extern	Uint16	i_UiC;

//--------------------------------------------------------------------------------
extern	float	Ui_Rms_Mean;

extern	Uint16	UiA_RmsNew;
extern	Uint16	UiB_RmsNew;
extern	Uint16	UiC_RmsNew;

//--------------------------------------------------------------------------------
// System Parameters								AC ( Current )
//--------------------------------------------------------------------------------
extern	float	IiA_Rms;
extern	float	IiB_Rms;
extern	float	IiC_Rms;

extern	float	IiA_Avg[RMS_AVG_NUM_IAC];
extern	float	IiB_Avg[RMS_AVG_NUM_IAC];
extern	float	IiC_Avg[RMS_AVG_NUM_IAC];
extern	Uint16	i_IiA;
extern	Uint16	i_IiB;
extern	Uint16	i_IiC;

//--------------------------------------------------------------------------------
extern	float	Ii_Rms_Mean;

//--------------------------------------------------------------------------------
// System Parameters								AC ( Frequency )
//--------------------------------------------------------------------------------
extern	float	UiA_Prd;
extern	float	UiB_Prd;
extern	float	UiC_Prd;

extern	float	UiA_Frq;
extern	float	UiB_Frq;
extern	float	UiC_Frq;

//--------------------------------------------------------------------------------
extern	Uint32	CapData_UiA_Mean;
extern	Uint32	CapData_UiB_Mean;
extern	Uint32	CapData_UiC_Mean;

extern	Uint16	CapMean_UiA_Renew;
extern	Uint16	CapMean_UiB_Renew;
extern	Uint16	CapMean_UiC_Renew;

extern	Uint16	CapData_Uin_Mean;

//--------------------------------------------------------------------------------
// System Parameters								Power
//--------------------------------------------------------------------------------
extern	float	Power_In;
extern	float	Power_Out;
extern	float	Power_Efficiency;

//--------------------------------------------------------------------------------
// Sampled Data : Capture
//--------------------------------------------------------------------------------
extern	volatile	Uint32	CapData_UiA[];
extern	volatile	Uint16	CapData_UiA_Index;
extern	volatile	Uint16	CapData_UiA_Full;

extern	volatile	Uint32	CapData_UiB[];
extern	volatile	Uint16	CapData_UiB_Index;
extern	volatile	Uint16	CapData_UiB_Full;

extern	volatile	Uint32	CapData_UiC[];
extern	volatile	Uint16	CapData_UiC_Index;
extern	volatile	Uint16	CapData_UiC_Full;

extern	volatile	Uint32	CapData_UiA_Error;
extern	volatile	Uint32	CapData_UiB_Error;
extern	volatile	Uint32	CapData_UiC_Error;

//--------------------------------------------------------------------------------
// Sampled Data : ADC
//--------------------------------------------------------------------------------
extern	volatile	Uint16	UiA_Trans;
extern	volatile	Uint16	UiB_Trans;
extern	volatile	Uint16	UiC_Trans;

extern	volatile	Uint16	IiA_Trans;
extern	volatile	Uint16	IiB_Trans;
extern	volatile	Uint16	IiC_Trans;

extern	volatile	Uint16	Uo1_Trans;
extern	volatile	Uint16	Uo2_Trans;

extern	volatile	Uint16	Io_Trans;

//--------------------------------------------------------------------------------
extern	volatile	Uint16	AdcResult_UiA[];
extern	volatile	Uint16	AdcResult_UiA_Index;
extern	volatile	Uint16	AdcBuffer_UiA_Full;

extern	volatile	Uint16	AdcResult_UiB[];
extern	volatile	Uint16	AdcResult_UiB_Index;
extern	volatile	Uint16	AdcBuffer_UiB_Full;

extern	volatile	Uint16	AdcResult_UiC[];
extern	volatile	Uint16	AdcResult_UiC_Index;
extern	volatile	Uint16	AdcBuffer_UiC_Full;

//--------------------------------------------------------------------------------
extern	volatile	Uint16	AdcResult_IiA[];
extern	volatile	Uint16	AdcResult_IiA_Index;
extern	volatile	Uint16	AdcBuffer_IiA_Full;

extern	volatile	Uint16	AdcResult_IiB[];
extern	volatile	Uint16	AdcResult_IiB_Index;
extern	volatile	Uint16	AdcBuffer_IiB_Full;

extern	volatile	Uint16	AdcResult_IiC[];
extern	volatile	Uint16	AdcResult_IiC_Index;
extern	volatile	Uint16	AdcBuffer_IiC_Full;

//--------------------------------------------------------------------------------
extern	volatile	Uint16	AdcResult_Uo1[];
extern	volatile	Uint16	AdcResult_Uo1_Index;
extern	volatile	Uint16	AdcBuffer_Uo1_Full;

extern	volatile	Uint16	AdcResult_Uo2[];
extern	volatile	Uint16	AdcResult_Uo2_Index;
extern	volatile	Uint16	AdcBuffer_Uo2_Full;

extern	volatile	Uint16	AdcResult_Io[];
extern	volatile	Uint16	AdcResult_Io_Index;
extern	volatile	Uint16	AdcBuffer_Io_Full;

//--------------------------------------------------------------------------------
// Data Table
//--------------------------------------------------------------------------------
extern	volatile	float	Table_Control[];

extern	const		float	Table_Original[];



