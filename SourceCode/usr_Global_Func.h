
//--------------------------------------------------------------------------------
extern	void usr_InitSysCtrl(void);

//--------------------------------------------------------------------------------
extern	void	Uint16_to_ASCII_2P(Uint16 , char *);
extern	void	Uint16_to_ASCII_4P(Uint16 , char *);
extern	void	Uint16_to_ASCII_5P(Uint16 , char *);

extern	void	Uint32_to_ASCII_7P(Uint32 , char *);

extern	void	float_to_ASCII_3P1(float , char *);
extern	void	float_to_ASCII_5P1(float , char *);

//--------------------------------------------------------------------------------
extern	float	RMS_IntArray(volatile Uint16 *, Uint16, float);

extern	float	Avg_Float(float *, Uint16);

extern	Uint16	Avg_Uint16(volatile Uint16 *, Uint16);
extern	Uint32	Avg_Uint32(volatile Uint32 *, Uint16);

//--------------------------------------------------------------------------------
extern	void	ePwm_width_1(void);
extern	void	ePwm_width_2(void);
extern	void	ePwm_width_O(void);

