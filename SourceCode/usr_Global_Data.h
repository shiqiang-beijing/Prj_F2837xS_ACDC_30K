
//--------------------------------------------------------------------------------
// Debug Switch
//--------------------------------------------------------------------------------

//---------- For Debug Configration ----------

#ifdef		_RAM_DEBUG							// Debug Mode

#define		VCM_TEST_MODE						// With 0 input for UiA_B_C, if Read UiA_Rms, UiA_Rms = ADC_ZERO_UiA
												// With 0 input for Uo1/Uo2, if Read Uo1/Uo2, Uo1/Uo2 = ADC_ZERO_Uo1/Uo2
#define		BUZZER_NOT_USE						// No Beeper

#endif

//---------- For Release Configration ----------

//#define		VCM_TEST_MODE                   // the Same as above

//#define		EPWM_TEST_MODE					// 1: 244A OE1 Enable; 2: ePWM_1~ePWM_3 Pulse Width Fixed Value

//#define		BUZZER_NOT_USE						// No Beeper

#define     POWER_MODE_PARALLEL                 // Parallel Mode ( Serial Mode -> Cancel the Defination )

#define     SCIB_MONITOR                        // SCIB Communicate wite Monitor

//--------------------------------------------------------------------------------
// Compiling Parameter
//--------------------------------------------------------------------------------
#ifdef		_FLASH
//
#define		BUFFER_SIZE_UAC		504
#define		BUFFER_SIZE_IAC		252
#define		BUFFER_SIZE_UDC		100
#define		BUFFER_SIZE_IDC		100
//
#endif

#ifdef		_RAM_DEBUG
//
#define		BUFFER_SIZE_UAC		 50
#define		BUFFER_SIZE_IAC		 25
#define		BUFFER_SIZE_UDC		 10
#define		BUFFER_SIZE_IDC		 10
//
#endif

#define		BUFFER_SIZE_CAP		16						// Divide 2^4, Ease Average Value Calculation

#define		WDH_AVG_NUM_PWM		20
#define		RMS_AVG_NUM_UDC		6						// Average Time of UDC Vaule
#define		RMS_AVG_NUM_UAC		6						// Average Time of UAC RMS Vaule
#define		RMS_AVG_NUM_IAC		6						// Average Time of IAC RMS Vaule

#define		WORKING_VALUE_UAC	30						// Start Working

//--------------------------------------------------------------------------------
// ADC VCM Limitation
//--------------------------------------------------------------------------------
#define		ADC_MIN_UAC_ZERO	1987					// VCM=1.5V  -> 2047		Theoretical VCM Value
#define		ADC_MAX_UAC_ZERO	2107					// Tolerance -> 3.0%

#define		ADC_MIN_IAC_ZERO	1987					// VCM=1.5V  -> 2047		Theoretical VCM Value
#define		ADC_MAX_IAC_ZERO	2107					// Tolerance -> 3.0%

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

extern	volatile	float	Rate_SymA_Pwm;
extern	volatile	float	Rate_SymB_Pwm;
extern	volatile	float	Rate_SymC_Pwm;

extern	volatile	float	Rate_All_A_Pwm;
extern	volatile	float	Rate_All_B_Pwm;
extern	volatile	float	Rate_All_C_Pwm;

//--------------------------------------------------------------------------------
extern	volatile	Uint16	Bias_ADC_UAB;
extern	volatile	Uint16	Bias_ADC_UBC;
extern	volatile	Uint16	Bias_ADC_UCA;

extern	volatile	float	Rate_RMS_UAB;
extern	volatile	float	Rate_RMS_UBC;
extern	volatile	float	Rate_RMS_UCA;

extern	volatile	Uint16	Bias_ADC_IAB;
extern	volatile	Uint16	Bias_ADC_IBC;
extern	volatile	Uint16	Bias_ADC_ICA;

extern	volatile	float	Rate_RMS_IAB;
extern	volatile	float	Rate_RMS_IBC;
extern	volatile	float	Rate_RMS_ICA;

extern	volatile	float	Slope_RMS_UDC;
extern	volatile	float	Apart_RMS_UDC;

extern	volatile	float	Slope_RMS_IDC;
extern	volatile	float	Apart_RMS_IDC;

//--------------------------------------------------------------------------------
extern	volatile	Uint16	ePwm_Width_A;
extern	volatile	Uint16	ePwm_Width_B;
extern	volatile	Uint16	ePwm_Width_C;

extern	volatile	Uint16	ePwm_Width_Min;
extern	volatile	Uint16	ePwm_Width_Max;

extern				float	ePwm_Width_Arry[];
extern	volatile	Uint16	ePwm_Width_Index;
extern	volatile	Uint16	ePwm_Width_Full;

extern	volatile	Uint16	ePwm_DutyCycle;

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
extern	Uint16	Uo2_ADC;

extern	float	Uo1_Avg[];
extern	float	Uo2_Avg[];
extern	Uint16	i_Uo1;
extern	Uint16	i_Uo2;

//--------------------------------------------------------------------------------
extern	float	Iout;
extern	Uint16	Iout_ADC;

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
// Data Buffer SCI-B
//--------------------------------------------------------------------------------
extern  volatile	char	SCIB_RxBuf[];
extern  volatile    Uint16  SCIB_RxCnt;
extern  volatile    Uint16  SCIB_RxGet;
extern  volatile    Uint16  SCIB_SetupGet;
