
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import  *
import UIComm,UIControl,UIDetail,UIOther
import matplotlib.pyplot as pl
import serial  #pip install pyserial
import sys
import time
import random #pip install random
import binascii,encodings; 
import numpy as np
from sympy.strategies.core import switch
from struct import pack,unpack
#define _CMD_SetRunParam     '0'
#define _CMD_SetPIDParam     '1'

#define _CMD_GetRunParam     '2'
#define _CMD_GetPIDParam     '3'

#define _CMD_SetVClose         '4'
#define _U_SetVOpen                 '5'
#define _U_SetVPID                 '6'
   

def readRunningParam(comm):
    #return int(Voltage_Set_Point,PWM_Output,lastVoltage,LastError,PrevError,SumError,currentVol0,currentVol1)
    buf = bytearray( 6 )
    buf[0] = ord('$') 
    buf[1]=ord('N' )
    buf[2]= ord('<' )
    buf[3]= 5
    buf[4]= ord('2' )
    buf[5]= ord('X' )
    try:           
        comm.write(bytes(buf))
        res=comm.read(23)
        offset = 5
        Voltage_Set_Point = res[offset+1]*256+res[offset]
        offset  =offset+2
        PWM_Output = res[offset+1]*256+res[offset]
        offset  =offset+2
        lastVoltage = res[offset+1]*256+res[offset]
        
        
        offset  =offset+2
        LastError = res[offset+1]*256+res[offset]
        offset  =offset+2
        PrevError = res[offset+1]*256+res[offset]
        offset  =offset+2
        SumError = res[offset+1]*256+res[offset]
        
  
        
        offset  =offset+2
        currentVol0 = (res[offset+1]*256+res[offset])
        offset  =offset+2
        currentVol1 = (res[offset+1]*256+res[offset])
        offset  =offset+2
        Voltage_Set_Point_temp = (res[offset+1]*256+res[offset])
        ret = [Voltage_Set_Point,PWM_Output,lastVoltage,LastError,PrevError,SumError,currentVol0,currentVol1,Voltage_Set_Point_temp]
        print(ret)
   
        return ret
    except:
        print('readRunningParam error')
        return None
def SendCommand(comm,cmd):       
        buf = bytearray( 6 )
        buf[0] = ord('$') 
        buf[1]= ord('N' )
        buf[2]= ord('<' )
        buf[3]= 6
        buf[4]= ord(cmd)
        buf[5]= ord('X' )           
        try:
            comm.write(bytes(buf))
        except:
            pass
def readAnswer(comm,readLen,timeout):        
        data = bytearray(64)
        try:   
            while(readLen[0]==0 and timeout>0):        
                while comm.inWaiting() > 0:
                    res = bytes(comm.read(1))
                    data[readLen[0]] = res[0]
                    readLen[0] =readLen[0]+1
                timeout = timeout-1       
                time.sleep(0.001)
                
            if  readLen[0] != 0:
                ret = bytearray(readLen[0])
                for index in range(readLen[0]):
                    ret[index] = data[index]
                return bytes(ret)
            else:
                return None                
        except:
            print('readAnswer error')
            return False
comm = serial.Serial('com13',115200,timeout=2)  
SendCommand(comm,'5') 
readLen = [0] 
ret = readAnswer(comm,readLen,10)
print(ret)
comm.close()