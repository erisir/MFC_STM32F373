
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
 

class UIAction():     
    firstUIComm = None
    secondUIDetail= None
    thirdUIControl= None
    fourUIOther= None
    proControl = None
    isDeviceReady = False
    comm = None
    commBusy = False
    lastError = 'no error'
    
    _CMD_SetRunParam = '0';
    _CMD_SetPIDParam = '1';
    _CMD_GetRunParam = '2';
    _CMD_GetPIDParam = '3';
    _CMD_SetVClose  = '4';
    _CMD_SetVOpen    = '5';
    _CMD_SetVPID    = '6'
    
    showUnit = "mv"
    
    Interception = 0
    Slope = 1
    
 
    
    def __init__(self,  firstUIComm, secondUIDetail, thirdUIControl, fourUIOther,proControl):
        self.firstUIComm = firstUIComm
        self.secondUIDetail = secondUIDetail
        self.thirdUIControl = thirdUIControl
        self.fourUIOther = fourUIOther
        self.proControl = proControl
 
    def RecaculatePIDParam(self):           
        if self.proControl.PID_AutoIncMode1.isChecked():#根据Kp获得Ki,Kd
            KpH = self.proControl.PID_KpH.value()
            KpM = self.proControl.PID_KpM.value()
            KpL = self.proControl.PID_KpL.value()
            
            self.proControl.PID_KiH.setProperty("value", float(KpH*3.5/2.45))
            self.proControl.PID_KdH.setProperty("value", float(KpH*1.25/2.45))
            
            self.proControl.PID_KiM.setProperty("value", float(KpM*3.5/2.45))
            self.proControl.PID_KdM.setProperty("value", float(KpM*1.25/2.45))
            
            self.proControl.PID_KiL.setProperty("value", float(KpL*3.5/2.45))
            self.proControl.PID_KdL.setProperty("value", float(KpL*1.25/2.45))
             
        if self.proControl.PID_AutoIncMode2.isChecked():  
            response_time = self.proControl.Responce_Time.value() 
            delay_time = self.proControl.Delay_Time.value()  
            PID_factor = int(self.proControl.PID_Factor.value()) 
            T = 0
            Kp = 0
            Ti = 0
            Td = 0
            if PID_factor==1:
                T = 0.05*delay_time
                Kp = 0.15*response_time/delay_time
                Ti = 2*delay_time
                Td = 0.45*delay_time
            if PID_factor==2:
                T = 0.16*delay_time
                Kp = 1*response_time/delay_time
                Ti = 1.9*delay_time
                Td = 0.55*delay_time
            if PID_factor==3:
                T = 0.34*delay_time
                Kp = 0.85*response_time/delay_time
                Ti = 1.62*delay_time
                Td = 0.65*delay_time
            if PID_factor==4:
                T = 0.6*delay_time
                Kp = 0.6*response_time/delay_time
                Ti = 1.5*delay_time
                Td = 0.82*delay_time
            Ki = Kp*T/Ti
            Kd = Kp*Td/T
            
            A = Kp+Ki+Kd
            B = Kp+2*Kd
            C = Kd    
            if A>65.535:
                A = 65.535
            if B>65.535:
                B=65.535
            if C>65.535:
                C = 65.535
            
            self.proControl.PID_KpH.setProperty("value", float(A))
            self.proControl.PID_KiH.setProperty("value", float(B))
            self.proControl.PID_KdH.setProperty("value", float(C))
            
            self.proControl.PID_KpM.setProperty("value", float(A))
            self.proControl.PID_KiM.setProperty("value", float(B))
            self.proControl.PID_KdM.setProperty("value", float(C))
            
            self.proControl.PID_KpL.setProperty("value", float(A))
            self.proControl.PID_KiL.setProperty("value", float(B))
            self.proControl.PID_KdL.setProperty("value", float(C))
    def CheckSumCalc(self,buf):
        checkSum = 0;
        return checkSum;
    def SendToComAndRead(self,buf,timeout):
        if self.isDeviceReady == False  :
            self.lastError = 'Device not ready'
            return None
        try:    
            while self.commBusy and timeout>0:
                time.sleep(0.001)  
                timeout -=1
            if timeout<=0:
                self.lastError = 'SendToComAndRead timeout,com is busy'
                return None 
            
            self.commBusy = True     
            self.comm.write(bytes(buf))
            res=self.readAnswer(timeout)  
            self.commBusy = False
            self.log(str(res))
            return res
        except:
            self.errorMessage("发送命令失败\t"+self.lastError)
            return None
    def SendCommand(self,cmd):
        
        buf = bytearray( 6 )
        buf[0] = ord('$') 
        buf[1]= ord('N' )
        buf[2]= ord('<' )
        buf[3]= 6
        buf[4]= ord(cmd)
        buf[5]= self.CheckSumCalc(buf)  
        ret = self.SendToComAndRead(buf,10)
        if ret is None:
            self.errorMessage(self.lastError)
            return False
        self.logMessage(ret.decode('utf-8'))
        return True
 
    def SendCommandWithData(self,buf):       
        ret = self.SendToComAndRead(buf,20)
        if ret is None:
            self.errorMessage(self.lastError)
            return False
        self.logMessage(ret.decode('utf-8'))
        return True
    
    def SendCommandWitAnswer(self,buf,expectLen): 
        res= self.SendToComAndRead(buf,20)
        if res is None:
            return None
        if(len(res) !=expectLen):
            self.lastError ='com return buffer len not equal'
            return None
        return res 
    
    def readAnswer(self,timeout):        
        data = bytearray(64)
        readLen = 0
        try:   
            while(readLen ==0 and timeout>0):        
                while self.comm.inWaiting() > 0:
                    res = bytes(self.comm.read(1))
                    data[readLen] = res[0]
                    readLen +=1
                timeout = timeout-1       
                time.sleep(0.001)
            if timeout<=0:
                self.lastError = 'readAnswer timeout'
                return None
            if  readLen != 0:
                ret = bytearray(readLen)
                for index in range(readLen):
                    ret[index] = data[index]
                return bytes(ret)
            else:
                self.lastError = 'readAnswer with nothing'
                return None                
        except:
            self.lastError ='readAnswer error,exception occurs' 
            return None
   
    def PWMOpen (self):
        self.SendCommand(self._CMD_SetVOpen)
        
    def PWMClose (self):
        self.SendCommand(self._CMD_SetVClose)
        
    def PWMPID (self):
        self.SendCommand(self._CMD_SetVPID)
         
    def CtrlMode_Dig (self):
        self.log("CtrlMode_Dig")
    def CtrlMode_Vot (self):
        self.log("CtrlMode_Vot")
    def CtrlMode_Cur (self):
        self.log("CtrlMode_Cur")
    def CtrlMode_Der (self):
        self.log("CtrlMode_Der")
    def ShowUnit_FS (self):
        self.showUnit = "FS"
        self.log("ShowUnit_FS")
    def ShowUnit_sccm (self):
        self.showUnit = "sccm"
    def ShowUnit_slm (self):
        self.showUnit = "slm"
    def ShowUnit_V (self):
        self.showUnit = "V"
    def ShowUnit_mv (self):
        self.showUnit = "mv"
    
    def Set_PID_AutoInc(self):
        pass
        #self.SendDataCommand(self._U_SetPIDMode,0)
    
    def Set_PID_ManuInc(self):
        pass
        #self.SendDataCommand(self._U_SetPIDMode,1)    
        
    def VotageToFlow(self,votage):
        Flow = (votage-self.Interception)/self.Slope
        if Flow <0:
            Flow = 0
        return Flow
    def FlowToVotage(self,flow):
        Votage = self.Slope*flow+self.Interception
        if Votage<0:
            Votage = 0
        return Votage
    
    def GetShowValue(self,votage):
        if self.showUnit == "FS":
            return votage/3200
        if self.showUnit == "sccm":
            return votage/320
        if self.showUnit == "slm":
            return votage/32
        if self.showUnit == "V":
            return votage/1000
        if self.showUnit == "mv":
            return votage
   
   
    def GetPlotData(self):
        #return int(Voltage_Set_Point,PWM_Output,lastVoltage,LastError,PrevError,SumError,currentVol0,currentVol1)
        ret = self.readRunningParam()
        if ret == None:
            print(self.lastError)
            return None
        try:
                       
            vCh0 = self.GetShowValue(ret[6])
            vCh1 = self.GetShowValue(ret[7])
            vsetpoint_temp = self.GetShowValue(ret[8])
            PWM_Output = ret[1]
            Voltage_Set_Point=self.GetShowValue(ret[0])                            
            return [Voltage_Set_Point,PWM_Output,vCh0,vsetpoint_temp]
        except:
            self.errorMessage("GetPlotData error")
            print(self.lastError)
            return None
         
    def stopVoltageVsPWMCurse(self):
        self.stopVoltageVsPWMCurse = True                  
         
    def getRandom(self):
            mid=1084
            start = 1
            end = 100
            return [mid+random.randint(start, end)/100,mid+random.randint(start, end)/100,mid+random.randint(start, end)/100] 
    
        
    def Connect(self):
        commName = 'com13'#self.firstUIComm.CommName.currentText()
        Baudrate = self.firstUIComm.Baudrate.currentText()   
        try:   
            self.comm = serial.Serial(commName,int(Baudrate),timeout=2)              
        except:
            self.errorMessage("串口"+commName+"被其他程序占用")
            return self.lastError
        self.isDeviceReady = True
        ret = self.readPIDParam()
        
        if ret:
            self.isDeviceReady = True
            self.logMessage("连接成功")
        else:
            self.isDeviceReady = False
            self.errorMessage(self.lastError)
            self.errorMessage("连接失败，请检查串口参数")
            
       

        return self.lastError
    def Disconnect(self):
        if  self.comm is not None:
            self.comm.close()
        self.isDeviceReady = False  
    def SetPIDParam(self):
        
        buf = bytearray( 35 )
        buf[0] = ord('$') 
        buf[1]=ord('N' )
        buf[2]= ord('<' )
        buf[3]= 35
        buf[4]= ord(self._CMD_SetPIDParam )
        buf[5]= ord('X' )
        offset = 5
        
        self.RecaculatePIDParam()
        x=self.proControl.PID_KpH.value()*1000
        y=self.proControl.PID_KiH.value()*1000
        z=self.proControl.PID_KdH.value()*1000
       
        buf[offset+1] = int(x/256)
        buf[offset] = int(x%256)
        offset = offset+ 2
        buf[offset+1] = int(y/256)
        buf[offset] = int(y%256)
        offset = offset+ 2
        buf[offset+1] = int(z/256)
        buf[offset] = int(z%256)
        offset = offset+ 2
        x=self.proControl.PID_KpM.value()*1000
        y=self.proControl.PID_KiM.value()*1000
        z=self.proControl.PID_KdM.value()*1000
       
        buf[offset+1] = int(x/256)
        buf[offset] = int(x%256)
        offset = offset+ 2
        buf[offset+1] = int(y/256)
        buf[offset] = int(y%256)
        offset = offset+ 2
        buf[offset+1] = int(z/256)
        buf[offset] = int(z%256)
        offset = offset+ 2
        x=self.proControl.PID_KpL.value()*1000
        y=self.proControl.PID_KiL.value()*1000
        z=self.proControl.PID_KdL.value()*1000
       
        buf[offset+1] = int(x/256)
        buf[offset] = int(x%256)
        offset = offset+ 2
        buf[offset+1] = int(y/256)
        buf[offset] = int(y%256)
        offset = offset+ 2
        buf[offset+1] = int(z/256)
        buf[offset] = int(z%256)
        offset = offset+ 2
        x=self.proControl.PID_ThredHold_High.value()
        y=self.proControl.PID_ThredHold_Low.value()
        z=self.proControl.PID_ThredHold_PWM.value()
       
        buf[offset+1] = int(x/256)
        buf[offset] = int(x%256)
        offset = offset+ 2
        buf[offset+1] = int(y/256)
        buf[offset] = int(y%256)
        offset = offset+ 2
        buf[offset+1] = int(z/256)
        buf[offset] = int(z%256)
        offset = offset+ 2
        
        x=self.proControl.DeadZone.value()
        y=self.proControl.Responce_Time.value()*1000
        z=self.proControl.F_Cutoff.value()*1000
        
      
        buf[offset+1] = int(x/256)
        buf[offset] = int(x%256)
        offset = offset+ 2
        buf[offset+1] = int(y/256)
        buf[offset] = int(y%256)
        offset = offset+ 2
        buf[offset+1] = int(z/256)
        buf[offset] = int(z%256)
                                                  
        return self.SendCommandWithData(bytes(buf))
    
   
        
    def readPIDParam(self):
        buf = bytearray( 6 )
        buf[0] = ord('$') 
        buf[1]=ord('N' )
        buf[2]= ord('<' )
        buf[3]= 6
        buf[4]= ord(self._CMD_GetPIDParam )
        buf[5]= ord('X' )
        res = self.SendCommandWitAnswer(buf,35)
        if res is  None:
            return False
        if res[0] != ord('$') :
            return False
        if res[1] != ord('N' ):
            return False
        if res[2] != ord('>' ):
            return False
        if res[4] != ord(self._CMD_GetPIDParam ):
            return False
        offset = 5
        x = res[offset+1]*256+res[offset]
        offset  =offset+2
        y = res[offset+1]*256+res[offset]
        offset  =offset+2
        z = res[offset+1]*256+res[offset]
        self.proControl.PID_KpH.setProperty("value", float(x/1000))
        self.proControl.PID_KiH.setProperty("value", float(y/1000))
        self.proControl.PID_KdH.setProperty("value", float(z/1000))
        
        offset  =offset+2
        x = res[offset+1]*256+res[offset]
        offset  =offset+2
        y = res[offset+1]*256+res[offset]
        offset  =offset+2
        z = res[offset+1]*256+res[offset]
        self.proControl.PID_KpM.setProperty("value", float(x/1000))
        self.proControl.PID_KiM.setProperty("value", float(y/1000))
        self.proControl.PID_KdM.setProperty("value", float(z/1000))
        offset  =offset+2
        x = res[offset+1]*256+res[offset]
        offset  =offset+2
        y = res[offset+1]*256+res[offset]
        offset  =offset+2
        z = res[offset+1]*256+res[offset]
        self.proControl.PID_KpL.setProperty("value", float(x/1000))
        self.proControl.PID_KiL.setProperty("value", float(y/1000))
        self.proControl.PID_KdL.setProperty("value", float(z/1000))
        offset  =offset+2
        x = res[offset+1]*256+res[offset]
        offset  =offset+2
        y = res[offset+1]*256+res[offset]
        offset  =offset+2
        z = res[offset+1]*256+res[offset]
        self.proControl.PID_ThredHold_High.setProperty("value", x)
        self.proControl.PID_ThredHold_Low.setProperty("value", y)
        self.proControl.PID_ThredHold_PWM.setProperty("value", z)
       
        offset  =offset+2
        x = res[offset+1]*256+res[offset]               
        offset  =offset+2
        y = (res[offset+1]*256+res[offset])
        offset  =offset+2
        z = (res[offset+1]*256+res[offset])
        
         
        self.proControl.DeadZone.setProperty("value", x)
        self.proControl.Responce_Time.setProperty("value", float(y/1000))
        self.proControl.F_Cutoff.setProperty("value", float(z/1000))
        return True

    
    def SetRuningParam(self):
        buf = bytearray( 9 )
        buf[0] = ord('$') 
        buf[1]=ord('N' )
        buf[2]= ord('<' )
        buf[3]= 9
        buf[4]= ord(self._CMD_SetRunParam )
        buf[5]= ord('X' )
        offset = 5
        
        x=self.proControl.PWM_SET.value()
        y=self.proControl.PID_SetPoint.value()
       
       
        buf[offset+1] = int(x/256)
        buf[offset] = int(x%256)
        offset = offset+ 2
        buf[offset+1] = int(y/256)
        buf[offset] = int(y%256)
 
        return self.SendCommandWithData(bytes(buf))
    def readRunningParam(self):
        #return int(Voltage_Set_Point,PWM_Output,lastVoltage,LastError,PrevError,SumError,currentVol0,currentVol1)
        buf = bytearray( 6 )
        buf[0] = ord('$') 
        buf[1]=ord('N' )
        buf[2]= ord('<' )
        buf[3]= 6
        buf[4]= ord(self._CMD_GetRunParam )
        buf[5]= ord('X' )
        res = self.SendCommandWitAnswer(buf,23)
        if res is  None:
            return None      
        if res[0] != ord('$') :
            return None
        if res[1] != ord('N' ):
            return None
        if res[2] != ord('>' ):
            return None
        if res[4] != ord(self._CMD_GetRunParam ):
            return None    
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
        #print(ret)
        return ret
           

    def log(self,str):
        pass
        #print('-'*10+str)
    def errorMessage(self,str):
        print('!'*10+str)
    def warnningMessage(self,str):
        print('?'*10+str)
    def logMessage(self,str):
        self.lastError = str
        print('-'*10+str)       
         
