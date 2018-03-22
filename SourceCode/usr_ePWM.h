
//--------------------------------------------------------------------------------
#define		EPWM_TBPRD_INITIAL			7937		// Initial Period
													// 5nS * 8000 = 40uS     -> Fs = 25.0 KHz 	( CPU@200MHz )
													// 5nS * 7937 = 39.685uS -> Fs = 25.2 KHz


// 1 Time ADC Sample + Convertion :	100nS + 220nS(11 ADC-Cycle) = 320nS 	=>	65  ( CPU-Cycle )	Sample  Proces Parallel
// ADC_ISR Run Time(Single Sample): 40 + Redundancy							=>	45  ( CPU-Cycle )	ADC_ISR Proces Serial

// UiA,UiB,UiC,ADC Sample_Convertion and ISR Proces Time:  65 + ( 45 * 3 )	=>  200 ( CPU-Cycle )	ePwm_width() Calling_Time

// ePwm_width() Running Time + ISR:	185 + Redundancy				 		=>	200 ( CPU-Cycle )

// ePwm Start Time Should Delay   :	200 + 200								=>	400

#define		EPWM10_CMPA_TRIGGER			210			// ePwm_width() Calling_Time ( ADC_Sample + ADC_ISR )

//------------------------------ For ePWM6 ~ ePWM8
#define		EPWM_CMP_START				420			// For Single Sample
#define		EPWM_CMP_OVERLAY			10			// * 5ns = 50ns			EPWM_CMP_OVERLAY < EPWM_CMP_MIN
#define		EPWM_CMP_MIN				50			// * 5ns = 250ns
#define		EPWM_CMP_MAX				7515		// EPWM_TBPRD - EPWM_CMP_START

//------------------------------ For ADC Trigger & Convertion

#define		EPWM7_CMPA_TRIGGER			7000		// Trigger for ADC_IiA
#define		EPWM8_CMPA_TRIGGER			7000		// Trigger for ADC_IiB
#define		EPWM9_CMPA_TRIGGER			7000		// Trigger for ADC_IiC

#define		EPWM7_CMPB_TRIGGER			7500		// Trigger for ADC_Uo1
#define		EPWM8_CMPB_TRIGGER			7500		// Trigger for ADC_Uo2
#define		EPWM9_CMPB_TRIGGER			7500		// Trigger for ADC_Io

//--------------------------------------------------------------------------------
// 周期扇区：12个
// 半周扇区：6 个
// 扇区跨度：30度
//
// 开关次数：  42/扇区    252/半周    504/周期
// 输出频率：    40       45       50       55       60		   ( Hz )
// 输出周期：    25       22.22    20       18.18    16.67     ( mS )
// 开关频率：    20.2              25.2              30.2      ( KHz )
// CPU 定时：    9920.6   8818.3   7936.5   7215.0   6613.7    ( CPU-Cycle )

//--------------------------------------------------------------------------------
extern void usr_ePWM_Initial(void);

extern void usr_ePWM1_Setup(void);
extern void usr_ePWM2_Setup(void);
extern void usr_ePWM3_Setup(void);

extern void usr_ePWM7_Setup(void);
extern void usr_ePWM8_Setup(void);
extern void usr_ePWM9_Setup(void);

extern void usr_ePWM10_Setup(void);

//--------------------------------------------------------------------------------
extern __interrupt void ISR_ePWM1(void);
extern __interrupt void ISR_ePWM2(void);
extern __interrupt void ISR_ePWM3(void);

extern __interrupt void ISR_ePWM10(void);


