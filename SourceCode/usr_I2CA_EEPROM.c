//--------------------------------------------------------------------------------
#include "F28x_Project.h"

//--------------------------------------------------------------------------------
#include "usr_I2CA_EEPROM.h"
#include "usr_Global_Data.h"

//--------------------------------------------------------------------------------
struct I2CMSG I2cMsg_EE_Out = { I2C_MSGSTAT_SEND_WITHSTOP,      // Status
                                EEPROM_I2C_ADDR_WRITE,          // I2C Address
                                EEPROM_NUM_BYTE,                // Data Number of Bytes
                                EEPROM_DATA_ADDR_HIGH,          // Data Address High
                                EEPROM_DATA_ADDR_LOW,           // Data Address Low
                                0x12,                           // Data Byte 1
                                0x34};                          // Data Byte 2

struct I2CMSG I2cMsg_EE_In  = { I2C_MSGSTAT_SEND_NOSTOP,
                                EEPROM_I2C_ADDR_READ,
                                EEPROM_NUM_BYTE,
                                EEPROM_DATA_ADDR_HIGH,
                                EEPROM_DATA_ADDR_LOW};

struct I2CMSG *CurrentMsgPtr;
Uint16 PassCount;
Uint16 FailCount;

//--------------------------------------------------------------------------------
void usr_I2CA_Setup(void)   {
    int16   i = 0;

    // Setup GPIO
    GPIO_SetupPinMux(32, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinMux(33, GPIO_MUX_CPU1, 1);

    // Setup Registers
    I2caRegs.I2CSAR.all = 0x00A0;       // Slave address - EEPROM control code

    I2caRegs.I2CPSC.all = 19;           // Prescaler: Need 7-12 Mhz,    Module-Clk = SYSCLKOUT / (IPSC+1) = 200MHz / 20
    I2caRegs.I2CCLKL = 20;              // NOTE: must be non zero       I2C-Clk = Module-Clk / ( ICCH+5 + ICCL+5 )
    I2caRegs.I2CCLKH = 20;              // NOTE: must be non zero       I2C-Clk = 10MHz / 50 = 200KHz

    I2caRegs.I2CIER.all = 0x24;         // Enable SCD & ARDY __interrupts
    I2caRegs.I2CMDR.all = 0x0020;       // Take I2C out of reset
                                        // Stop I2C when suspended

    I2caRegs.I2CFFTX.all = 0x6000;      // Enable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x2040;      // Enable RXFIFO, clear RXFFINT,

    // Clear incoming message buffer
    for (i = 0; i < I2C_MAX_BUFFER_SIZE; i++)
    {
        I2cMsg_EE_In.MsgBuffer[i] = 0x0000;
    }
}

//--------------------------------------------------------------------------------
Uint16 usr_I2CA__Write(struct I2CMSG *msg)  {
    Uint16 i;

    // Wait until the STP bit is cleared from any previous master communication.
    // Clearing of this bit by the module is delayed until after the SCD bit is
    // set. If this bit is not checked prior to initiating a new message, the
    // I2C could get confused.

    if(I2caRegs.I2CMDR.bit.STP == 1)
    {
       return I2C_STP_NOT_READY_ERROR;
    }

    // Setup slave address
    //
    I2caRegs.I2CSAR.all = msg->SlaveAddress;

    // Check if bus busy
    //
    if(I2caRegs.I2CSTR.bit.BB == 1)
    {
       return I2C_BUS_BUSY_ERROR;
    }

    // Setup number of bytes to send
    // MsgBuffer + Address
    //
    I2caRegs.I2CCNT = msg->NumOfBytes+2;

    // Setup data to send
    //
    I2caRegs.I2CDXR.all = msg->MemoryHighAddr;
    I2caRegs.I2CDXR.all = msg->MemoryLowAddr;

    for (i=0; i < msg->NumOfBytes; i++)
    {
       I2caRegs.I2CDXR.all = *(msg->MsgBuffer+i);
    }

    // Send start as master transmitter
    //
    I2caRegs.I2CMDR.all = 0x6E20;

    return I2C_SUCCESS;
}

Uint16 usr_I2CA__Read(struct I2CMSG *msg)   {

    // Wait until the STP bit is cleared from any previous master communication.
    // Clearing of this bit by the module is delayed until after the SCD bit is
    // set. If this bit is not checked prior to initiating a new message, the
    // I2C could get confused.

    if(I2caRegs.I2CMDR.bit.STP == 1)
    {
       return I2C_STP_NOT_READY_ERROR;
    }

    I2caRegs.I2CSAR.all = msg->SlaveAddress;

    if(msg->MsgStatus == I2C_MSGSTAT_SEND_NOSTOP)
    {
       // Check if bus busy
       //
       if(I2caRegs.I2CSTR.bit.BB == 1)
       {
          return I2C_BUS_BUSY_ERROR;
       }
       I2caRegs.I2CCNT = 2;
       I2caRegs.I2CDXR.all = msg->MemoryHighAddr;
       I2caRegs.I2CDXR.all = msg->MemoryLowAddr;
       I2caRegs.I2CMDR.all = 0x2620; // Send data to setup EEPROM address
    }
    else if(msg->MsgStatus == I2C_MSGSTAT_RESTART)
    {
       I2caRegs.I2CCNT = msg->NumOfBytes;    // Setup how many bytes to expect
       I2caRegs.I2CMDR.all = 0x2C20;         // Send restart as master receiver
    }

    return I2C_SUCCESS;
}

//--------------------------------------------------------------------------------
// pass - Halt debugger and signify pass
//
void pass()
{
   asm("   ESTOP0");
   for(;;);
}

// fail - Halt debugger and signify fail
//
void fail()
{
   asm("   ESTOP0");
   for(;;);
}

__interrupt void ISR_I2CA_INT(void)     {

    Uint16 IntSource, i;

    // Read __interrupt source
    //
    IntSource = I2caRegs.I2CISRC.all;

    // Interrupt source = stop condition detected
    //
    if(IntSource == I2C_SCD_ISRC)
    {
        // If completed message was writing data, reset msg to inactive state
        //
        if(CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_WRITE_BUSY)
        {
            CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_INACTIVE;
        }
        else
        {
            // If a message receives a NACK during the address setup portion of
            // the EEPROM read, the code further below included in the register
            // access ready __interrupt source code will generate a stop
            // condition. After the stop condition is received (here), set the
            // message status to try again. User may want to limit the number of
            // retries before generating an error.
            if(CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
            {
                CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_SEND_NOSTOP;
            }
            // If completed message was reading EEPROM data, reset msg to inactive
            // state and read data from FIFO.
            else if(CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_READ_BUSY)
            {
                CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_INACTIVE;

                for(i=0; i < EEPROM_NUM_BYTE; i++)
                {
                    CurrentMsgPtr->MsgBuffer[i] = I2caRegs.I2CDRR.all;
                }
                {
                    // Check received data
                    //
                    for(i=0; i < EEPROM_NUM_BYTE; i++)
                    {
                        if(I2cMsg_EE_In.MsgBuffer[i] == I2cMsg_EE_Out.MsgBuffer[i])
                        {
                            PassCount++;
                        }
                        else
                        {
                            FailCount++;
                        }
                    }
                    if(PassCount == EEPROM_NUM_BYTE)
                    {
                        pass();
                    }
                    else
                    {
                        fail();
                    }
                }
            }
        }
    }

    //
    // Interrupt source = Register Access Ready
    // This __interrupt is used to determine when the EEPROM address setup
    // portion of the read data communication is complete. Since no stop bit is
    // commanded, this flag tells us when the message has been sent instead of
    // the SCD flag. If a NACK is received, clear the NACK bit and command a
    // stop. Otherwise, move on to the read data portion of the communication.
    else if(IntSource == I2C_ARDY_ISRC)
    {
        if(I2caRegs.I2CSTR.bit.NACK == 1)
        {
            I2caRegs.I2CMDR.bit.STP = 1;
            I2caRegs.I2CSTR.all = I2C_CLR_NACK_BIT;
        }
        else if(CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
        {
            CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_RESTART;
        }
    }
    else
    {
        // Generate some error due to invalid __interrupt source
        asm("   ESTOP0");
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}
