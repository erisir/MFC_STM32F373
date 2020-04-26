//file user_mb_app.h

#ifndef    USER_APP
#define USER_APP

#include "main.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbutils.h"

/* -----------------------Slave Defines -------------------------------------*/
#define S_DISCRETE_INPUT_START        0 //40000 等
#define S_DISCRETE_INPUT_NDISCRETES   7
#define S_COIL_START                  0 //40000 等
#define S_COIL_NCOILS                 3 // ON OFF pid
#define S_REG_INPUT_START             0 //40000 等
#define S_REG_INPUT_NREGS             100 // Ch0 Ch1
#define S_REG_HOLDING_START           0 //40000 等
#define S_REG_HOLDING_NREGS           260
 
#endif
