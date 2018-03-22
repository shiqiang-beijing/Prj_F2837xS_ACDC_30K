
//--------------------------------------------------------------------------------
// PTP Package ( 176-Pin )

// PZP Package ( 100-Pin )
#define		SCIB_TXD		22				// GPIO-22	MUX_POSITION = 3
#define		SCIB_RXD		23				// GPIO-23	MUX_POSITION = 3

#define		SCIB_RTS_485	24

//--------------------------------------------------------------------------------
extern	char	* SCIB_msg;
extern	char	TmpArray_3[];
extern	char	TmpArray_6[];
extern	char	TmpArray_8[];

//--------------------------------------------------------------------------------
extern void usr_SCIB_Setup(void);

extern void usr_SCIB_Xmit(int);
extern void usr_SCIB_MsgPut(char *);

extern void usr_SCIB_485_En(void);

extern void usr_SCIB_DebugInfo(void);

