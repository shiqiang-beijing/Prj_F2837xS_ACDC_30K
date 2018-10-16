
//--------------------------------------------------------------------------------
//  24C02   2K Bit      256 Byte      32 Page     8 Byte/Page      Internal Address : 0x00~0xFF

//--------------------------------------------------------------------------------
#define     EEPROM_I2C_ADDR_WRITE   0xA0        // I2C Slave Chip Address for Write : 1010 + 000 + 0
#define     EEPROM_I2C_ADDR_READ    0xA1        // I2C Slave Chip Address for Read  : 1010 + 000 + 1

#define     EEPROM_DATA_ADDR_HIGH   0x00        // Data Address High
#define     EEPROM_DATA_ADDR_LOW    0x30        // Data Address Low

#define     EEPROM_NUM_BYTE         2

//--------------------------------------------------------------------------------
extern      struct I2CMSG I2cMsg_EE_Out;
extern      struct I2CMSG I2cMsg_EE_In;

//--------------------------------------------------------------------------------
extern      void usr_I2CA_Setup(void);

extern      Uint16 usr_I2CA__Write(struct I2CMSG *msg);
extern      Uint16 usr_I2CA__Read(struct I2CMSG *msg);

extern      __interrupt void ISR_I2CA_INT(void);
