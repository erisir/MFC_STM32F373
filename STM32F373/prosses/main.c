/**
  ******************************************************************************
  * @file    main.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   main
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#include "tasks.h" 
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
/*
// ----------------------- Defines ------------------------------------------
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 10

// ----------------------- Static variables ---------------------------------
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];*/

extern uint16_t Counters[CounterNum];
extern uint8_t commERROR;
extern uint32_t runningCounter;
uint8_t ticFlag[4];
uint32_t ticTimers[4];
uint32_t tocTimers[4];
void tic(uint8_t index)
{
	ticFlag[index]=5;
	ticTimers[index]=runningCounter;

}
void toc(uint8_t index)
{
	ticFlag[index]=0;
	tocTimers[index]=runningCounter-ticTimers[index];
	REG_INPUTsAddr->DEBUG32[index]=tocTimers[index]; 
}
void toggle(uint8_t index)
{
	if(ticFlag[index]<3)
			tic(index);
	else
			toc(index);
}
int main(void)
{ 
	 // uint16_t a=10;
		
		Bsp_Int();		 
	 
		while(1)
		{		
			  	
				if(Counters[0]>=50)//1000Hz
				{							
						toggle(0);	
						Counters[0] = 0;							
						VOL_IIR_Filter();//40us									

				}				
				if(Counters[1]>=50)//200Hz
				{		
						toggle(1);	
						eMBPoll();					
						Counters[1] = 0;								
				}
				if(Counters[2]>=Get_ControlCycle()*10)//5Hz
				{
						toggle(2);
						Counters[2] = 0;						
						if (PID_isRunning()) PID_Start();	//80us											 
				} 
				if(Counters[3]>=5000)
				{			
					 
					Counters[3] = 0;
					if(IsFlowReset()){
								OnResetFlowDown();
						}	
				}
		}
}
/*
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
	static u16 i;	///

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
						usRegInputBuf[iRegIndex] = i++;	 ///
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
 
    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNRegs;
    ( void )eMode;
		 
    return MB_ENOERR;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNCoils;
    ( void )eMode;
		 
    return MB_ENOERR;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
		 
    return MB_ENOERR;
}*/

