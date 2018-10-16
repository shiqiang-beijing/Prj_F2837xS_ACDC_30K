
//--------------------------------------------------------------------------------

#define		CAP_GPIO_UAC_A			34				// Capture Input for UiA/B/C
#define		CAP_GPIO_UAC_B			35
#define		CAP_GPIO_UAC_C			36				// Older HardWare Version Use : 32,33,34

//--------------------------------------------------------------------------------

//#define		ERROR_DATA_REMOVE						// Remove Error Data

#define		CAP_DATA_UI_MAX			4800000			// 40Hz
#define		CAP_DATA_UI_MIN			3200000			// 60Hz

//#define		CAP_DATA_UI_MAX			4400000			// 45Hz
//#define		CAP_DATA_UI_MIN			3600000			// 55Hz


//--------------------------------------------------------------------------------

extern	void usr_eCAP_Initial(void);

extern	void usr_eCAP1_Setup();
extern	void usr_eCAP2_Setup();
extern	void usr_eCAP3_Setup();

extern __interrupt void ISR_eCAP1(void);
extern __interrupt void ISR_eCAP2(void);
extern __interrupt void ISR_eCAP3(void);
