
//------------------------------ ADCCLK		// ADC Input Clock:      5 ~ 50 MHz
											// ADC Conversion Cycle: 12Bit -> 10~11 ADCCLK,  16Bit -> 30~31 ADCCLK
											// ADCCTL2.PRESCALE:
#define		ADC_PRESCALE		6			// 0->SYSCLK/1    2->SYSCLK/2  3->SYSCLK/2.5  4->SYSCLK/3
											// 5->SYSCLK/3.5  6->SYSCLK/4  7->SYSCLK/4.5  8->SYSCLK/5

//------------------------------ Power Up
#define		ADC_POWERUP_US		500			// Power-Up Time = 500uS

//------------------------------ ADC-SOC ( Sample Window )
#define		ADC_ACQPS_Uac		19			// ( ACQPS + 1) * ( SYSCLK ) = 100 nS
											// Conversion_Time = 11 * ADC_Clock = 220 nS

//--------------------------------------------------------------------------------
// ADC-SOC ( Trigger Source ) ( for bit.TRIGSEL )
//--------------------------------------------------------------------------------
// 0x00->Software Only
// 0x01->CPU1_TIMER0_INT   0x02->CPU1_TIMER1_INT   0x03->CPU1_TIMER2_INT
// 0x04->GPIO_ADCEXTSOC
// 0x05/06->ePWM1_SOCA/B   0x07/08->ePWM2_SOCA/B   0x09/0A->ePWM3_SOCA/B
// 0x0B/0C->ePWM4_SOCA/B   0x0D/0E->ePWM5_SOCA/B   0x0F/10->ePWM6_SOCA/B
// 0X11/12->ePWM7_SOCA/B   0X13/14->ePWM8_SOCA/B   0X15/16->ePWM9_SOCA/B
// 0X17/18->ePWM10_SOCA/B  0X19/1A->ePWM11_SOCA/B  0X1B/1C->ePWM12_SOCA/B

#define		ADC_TRIGGER_UiA1	0x05		// ePWM1_SOCA
#define		ADC_TRIGGER_UiB1	0x07		// ePWM2_SOCA
#define		ADC_TRIGGER_UiC1	0x09		// ePWM3_SOCA

#define		ADC_TRIGGER_UiA2	0x21
#define		ADC_TRIGGER_UiB2	0x22
#define		ADC_TRIGGER_UiC2	0x23

#define		ADC_TRIGGER_IiA		0x11		// ePWM7_SOCA
#define		ADC_TRIGGER_IiB		0x13		// ePWM8_SOCA
#define		ADC_TRIGGER_IiC		0x15		// ePWM9_SOCA

#define		ADC_TRIGGER_Uo1		0x12		// ePWM7_SOCB
#define		ADC_TRIGGER_Uo2		0x14		// ePWM8_SOCB
#define		ADC_TRIGGER_Io		0x16		// ePWM9_SOCB

//--------------------------------------------------------------------------------
// ADC-SOC ( Channle : Uac )	Single-Ended ADC Channle: 0->ADCIN-0 ~ 15->ADCIN-15
//--------------------------------------------------------------------------------
#define		ADC_CHANNEL_UiA1	1			// UiA1 -> Channle A1 -> INT_A1
#define		ADC_CHANNEL_UiB1	2			// UiB1 -> Channle B2 -> INT_B1
#define		ADC_CHANNEL_UiC1	2			// UiC1 -> Channle C2 -> INT_C1

#define		ADC_CHANNEL_UiA2	2			// A2
#define		ADC_CHANNEL_UiB2	3			// B3
#define		ADC_CHANNEL_UiC2	3			// C3

#define		ADC_CHANNEL_IiA		3			// IiA  -> Channle A3 -> INT_A4
#define		ADC_CHANNEL_IiB		4			// IiB  -> Channle C4 -> INT_C2
#define		ADC_CHANNEL_IiC		0			// IiC  -> Channle D0 -> INT_D2

#define		ADC_CHANNEL_Uo1		4			// Uo1  -> Channle A4 -> INT_A2	 ( in the Front of  Lo )
#define		ADC_CHANNEL_Uo2		1			// Uo2  -> Channle D1 -> INT_D1  ( at the Behind of Lo )
#define		ADC_CHANNEL_Io		0			// Io   -> Channle A0 -> INT_A3

//------------------------------ ADC-SOC ( Channle : Temperature )
#define		ADC_CHANNEL_Tmp1	3			// D3
#define		ADC_CHANNEL_Tmp2	4			// D4

//--------------------------------------------------------------------------------
// ADC-ZERO
//--------------------------------------------------------------------------------
#define		ADC_ZERO_UiA		2050		// VCM=1.5V -> 2047		Theoretical VCM Value
#define		ADC_ZERO_UiB		2051
#define		ADC_ZERO_UiC		2047

#define		ADC_ZERO_IiA		2051		// VCM=1.5V -> 2047		Theoretical VCM Value
#define		ADC_ZERO_IiB		2051
#define		ADC_ZERO_IiC		2050

#define		ADC_ZERO_Uo1		 683		// OP_0=0.5V -> 683		Theoretical Zero_Vin Value ( Vin=0, Out_Op=0.5 )
#define		ADC_ZERO_Uo2		 683
#define		ADC_ZERO_Io			 683		// OP_0=0.5V -> 683		Theoretical Zero_Vin Value

//--------------------------------------------------------------------------------
extern void usr_AdcA_Initial(void);
extern void usr_AdcB_Initial(void);
extern void usr_AdcC_Initial(void);
extern void usr_AdcD_Initial(void);

extern void usr_AdcA_Setup(void);
extern void usr_AdcB_Setup(void);
extern void usr_AdcC_Setup(void);
extern void usr_AdcD_Setup(void);

extern void usr_Adc_Trigger_On(void);
extern void usr_Adc_Trigger_Off(void);

extern __interrupt void ISR_AdcA_INT1(void);
extern __interrupt void ISR_AdcA_INT2(void);
extern __interrupt void ISR_AdcA_INT3(void);
extern __interrupt void ISR_AdcA_INT4(void);

extern __interrupt void ISR_AdcB_INT1(void);
extern __interrupt void ISR_AdcB_INT2(void);
extern __interrupt void ISR_AdcB_INT3(void);
extern __interrupt void ISR_AdcB_INT4(void);

extern __interrupt void ISR_AdcC_INT1(void);
extern __interrupt void ISR_AdcC_INT2(void);
extern __interrupt void ISR_AdcC_INT3(void);
extern __interrupt void ISR_AdcC_INT4(void);

extern __interrupt void ISR_AdcD_INT1(void);
extern __interrupt void ISR_AdcD_INT2(void);
extern __interrupt void ISR_AdcD_INT3(void);
extern __interrupt void ISR_AdcD_INT4(void);

