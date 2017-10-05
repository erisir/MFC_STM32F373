
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
#PWMOpen,PWMClose,PWMPID,CtrlMode_Dig,CtrlMode_Vot,CtrlMode_Cur,CtrlMode_Der,ShowUnit_FS,ShowUnit_sccm,ShowUnit_slm,ShowUnit_V,ShowUnit_mv

class Pid:
    def __init__(self):
        self.Kp =  4123.0
        self.Ki =  4123.0
        self.Kd =  4123.0 
        self.inteval = 200   
        self.setPoint = 1200
        self.PWMOut = 0.0
        self.PIDByPCEnable = False
        self.currVotage = 0.0
        self.LastError = 0.0
        self.PrevError = 0.0
        
class PWMVotageFitPara:
    def __init__(self):
        self.SetForwardA = 0
        self.SetForwardB = 0
        self.SetForwardC = 0
        self.SetBackwardA = 0
        self.SetBackwardB = 0
        self.SetBackwardC = 0


class UIAction():     
    firstUIComm = None
    secondUIDetail= None
    thirdUIControl= None
    fourUIOther= None
    isDeviceReady = False
    
    lastError = ""
    spid = Pid()
    sPVFD = PWMVotageFitPara()

    
    _U_SetVotage     = '0';
    _U_SetPTerm      = '1';
    _U_SetITerm      = '2';
    _U_SetDTerm      = '3';
    _U_SetDura       = '4';
    _U_SetPWMVal     = '5';
    _U_GetVotage     = '6';
    _U_SetTClose     = '7';
    _U_SetTOpen      = '8';
    _U_SetTPID       = '9';
    _U_SetVotageTimes= 'a';
    _U_SetPIDMode= 'b';
    _U_SetPIDPeriod = 'c';
    _U_SetTIM4Prescaler = 'd';
    _U_SetPIDVotageChanel =  'e'
    _U_SetForwardA = 'f';
    _U_SetForwardB = 'g';
    _U_SetForwardC = 'h';
    _U_SetBackwardA = 'i';
    _U_SetBackwardB = 'j';
    _U_SetBackwardC = 'k';
    _U_SetPIDThredHold='l';
    
    showUnit = "mv"
    
 
    
    def __init__(self,  firstUIComm, secondUIDetail, thirdUIControl, fourUIOther,proControl):
        self.firstUIComm = firstUIComm
        self.secondUIDetail = secondUIDetail
        self.thirdUIControl = thirdUIControl
        self.fourUIOther = fourUIOther
        self.proControl = proControl
        self.pidPara =["Kp","Ki","Kd","DeadZone","SetPoint","Output","LastError","PrevError","voltage","SumErr"];
        self.comm = None
        self.SetVotage = 0
        
        self.sPVFD.SetForwardA = -0.006
        self.sPVFD.SetForwardB = 38.712
        self.sPVFD.SetForwardC = 4770
        
        self.sPVFD.SetBackwardA = -0.002
        self.sPVFD.SetBackwardB = 30.283
        self.sPVFD.SetBackwardC = 254.478
        

    def SendDataCommand(self,cmd,value):
        byteArr = pack('f',value)
        checkSum = 'X'
        buf = bytes([ord('@'),ord(cmd),byteArr[0],byteArr[1],byteArr[2],byteArr[3],ord(checkSum)])
        #print(buf)
        #self.log("发送字符串\t"buf)  
        #print("发送字符串")      
        #print(buf)
        try:
            self.comm.write(buf) 
            time.sleep(0.1)
            res = self.comm.read_until()
            print(res)  
        except:
            self.errorMessage("发送命令失败")
        
    def SendDataCommandWithAnswer(self,cmd,value):
        byteArr = pack('f',value)
        checkSum = 'X'
        buf = bytes([ord('@'),ord(cmd),byteArr[0],byteArr[1],byteArr[2],byteArr[3],ord(checkSum)])
        #print(buf)
        #self.log("发送字符串\t"buf)  
        #print("发送字符串")      
        #print(buf)
        try:
            self.comm.write(buf) 
        except:
            self.errorMessage("发送命令失败")

    def PWMOpen (self):
        self.SendDataCommand(self._U_SetTOpen,0)
        self.spid.PIDByPCEnable = False
        self.spid.PWMOut = 65530
        self.log("PWMOpen")
    def PWMClose (self):
        self.SendDataCommand(self._U_SetTClose,0)
        self.spid.PIDByPCEnable = False
        self.spid.PWMOut = 30
        self.log("PWMClose")
    def PWMPID (self):
        if self.proControl.PID_ByPC.isChecked():
            self.spid.PIDByPCEnable = True
        else:
            self.spid.PIDByPCEnable = False
            self.SendDataCommand(self._U_SetTPID,0)
        self.log("PWMPID")
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
        self.log("ShowUnit_sccm")
    def ShowUnit_slm (self):
        self.showUnit = "slm"
        self.log("ShowUnit_slm")
    def ShowUnit_V (self):
        self.showUnit = "V"
        self.log("ShowUnit_V")
    def ShowUnit_mv (self):
        self.showUnit = "mv"
        self.log("ShowUnit_mv")
    
    def Set_PID_AutoInc(self):
        self.SendDataCommand(self._U_SetPIDMode,0)
    
    def Set_PID_ManuInc(self):
        self.SendDataCommand(self._U_SetPIDMode,1)
    
    def PID_Kp_valueChanged(self,value):
        self.spid.Kp = value/1000
        self.SendDataCommand(self._U_SetPTerm,value)
    
    def PID_Ki_valueChanged(self,value):
        self.spid.Ki = value/1000
        self.SendDataCommand(self._U_SetITerm,value)
        
    def PID_Kd_valueChanged(self,value):
        self.spid.Kd = value/1000
        self.SendDataCommand(self._U_SetDTerm,value)
        
    def PID_Inteval_valueChanged(self,value):
        self.spid.inteval = value
        self.SendDataCommand(self._U_SetPIDPeriod,value)
        
    def PID_SetPoint_valueChanged(self,value):
        self.SendDataCommand(self._U_SetVotage,value)
        self.spid.setPoint = value
    
    def PID_ThredHold_valueChanged(self,value):
        self.SendDataCommand(self._U_SetPIDThredHold,value)
    
    def PID_VotageChanel_valueChanged(self,value):
        self.SendDataCommand(self._U_SetPIDVotageChanel,value)
        
        
    def SmoothWindow_valueChanged(self,value):
        self.SendDataCommand(self._U_SetVotageTimes,value)
        
    def Prescaler_valueChanged(self,value):
        self.SendDataCommand(self._U_SetTIM4Prescaler,value)
        
    def PWMValue_valueChanged(self,value):
        self.SendDataCommand(self._U_SetPWMVal,value)
        
    def PWMRate_valueChanged(self,value):
        self.PWMRate = value
    def Slope_valueChanged(self,value):
        self.Slope=value
    def Interception_valueChanged(self,value):
        self.Interception=value

        
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
   
    def IncPIDCalc(self,NextPoint):  
        #当前误差
        iError = self.spid.setPoint - NextPoint;
        #增量计算
        iIncpid = self.spid.Kp * iError #E[k]项
        - self.spid.Ki * self.spid.LastError #E[k－1]项
        + self.spid.kd * self.spid.PrevError; #E[k－2]项
        #存储误差，用于下次计算
        self.spid.PrevError = self.spid.LastError;
        self.spid.LastError = iError;
        #返回增量值
        return(iIncpid);
       
    #增量式自适应PID控制设计
    def IncAutoPIDCalc(self, NextPoint):
        
        #当前误差
        iError = self.spid.setPoint - NextPoint;
        if iError>500:
            return self.getPWMAtVotage(self.spid.setPoint)
        #增量计算
        iIncpid = self.spid.Kp * (2.45*iError #E[k]项
        - 3.5*self.spid.LastError #E[k－1]项
        + 1.25*self.spid.PrevError) #E[k－2]项
        #存储误差，用于下次计算
        self.spid.PrevError = self.spid.LastError;
        self.spid.LastError = iError;
        #返回增量值
        return(iIncpid);
    def getPWMAtVotage(self,votage):
        return 65535*votage/3
        pass
    def GetPlotData(self):
        votage = self.GetVotage()
        if votage is None:
            return None
        
        vCh0 = self.GetShowValue(votage[0])
        vCh1 = self.GetShowValue(votage[1])
        
        vSetPoint = self.GetShowValue(self.spid.setPoint)
        pwmOut = votage[2]
        
        if self.spid.PIDByPCEnable:
            self.Set_PID_ControlByPC(votage[1])
            pwmOut = self.spid.PWMOut
            print(str(votage[1])+"|"+str(pwmOut))
        return [vCh0,vSetPoint,vCh1,pwmOut]
    
    def Set_PID_ControlByPC(self,votage): 
        out = self.IncAutoPIDCalc(votage)
        self.spid.PWMOut += out
        if self.spid.PWMOut<20:
            self.spid.PWMOut = 20
        if self.spid.PWMOut>65530:
            self.spid.PWMOut = 65530
        self.spid.currVotage = votage
        
        
        self.SendDataCommand(self._U_SetPWMVal,self.spid.PWMOut)
        #time.sleep(self.spid.inteval/1000)

    def getPWMVSVotage(self):
        self.stopVoltageVsPWMCurse = False
        self.ConnectTest()
        pwmForward = []
        votageForward = []
        pwmBackward = []
        votageBackward = []
        
        pl.cla()
        pl.grid() #开启网格
        ax = pl.gca()
        pl.xlabel("PWM")
        pl.ylabel("Votage")
        pl.title("PWM => Votage")
        pl.legend()
        pl.show()
        start = int(self.proControl.BackForward_Start.value())
        end = int(self.proControl.BackForward_End.value())
        stepsize = int(self.proControl.BackForward_StepSize.value())
 
        for x in range(start,end,stepsize):
            if self.stopVoltageVsPWMCurse :
                return   
            self.SendDataCommand(self._U_SetPWMVal,x)
            pl.pause(0.1)
            ret = self.GetVotage()
 
            pwmForward.append(x)
            votageForward.append(ret[1])
            pl.plot(pwmForward, votageForward, 'r*')
        
        
        
        for x in range(end,start,-1*stepsize):
            if self.stopVoltageVsPWMCurse :
                return 
            self.SendDataCommand(self._U_SetPWMVal,x)
            pl.pause(0.02)
            ret = self.GetVotage()
 
            pwmBackward.append(x)
            votageBackward.append(ret[1])
            pl.plot(pwmBackward, votageBackward, 'b*')
        pwmForwardFit = []
        votageForwardFit = []
        pwmBackwardFit = []
        votageBackwardFit = []
        vmax =2000
        vmin= 500
        ind= 0

        for x in votageForward:
            if x >vmin and x<vmax:
                pwmForwardFit.append(pwmForward[ind])
                votageForwardFit.append(votageForward[ind])
            ind= ind+1
        ind= 0 
        for x in votageBackward:
            if x >vmin and x<vmax:
                pwmBackwardFit.append(pwmBackward[ind])
                votageBackwardFit.append(votageBackward[ind])
            ind= ind+1
        pl.plot(pwmForwardFit, votageForwardFit, 'k-')
        pl.plot(pwmBackwardFit, votageBackwardFit, 'k-')

        ForwardFunc = np.polyfit(np.array(votageForwardFit),np.array(pwmForwardFit) , 2)#用2次多项式拟合
        BackwardFunc = np.polyfit(np.array(votageBackwardFit), np.array(pwmBackwardFit), 2)#用2次多项式拟合     
        
        print(ForwardFunc)
        print(BackwardFunc)
        
        self.sPVFD.SetForwardA = ForwardFunc[0]
        self.sPVFD.SetForwardB = ForwardFunc[1]
        self.sPVFD.SetForwardC = ForwardFunc[2]

        
        self.sPVFD.SetBackwardA = BackwardFunc[0]
        self.sPVFD.SetBackwardB = BackwardFunc[1]
        self.sPVFD.SetBackwardC = BackwardFunc[2]
  
        fitX = range(20,2700,200)  
              
        fitBackwardPWM=np.polyval(BackwardFunc,fitX)
        fitForwardPWM=np.polyval(ForwardFunc,fitX)
        
        pl.plot(fitForwardPWM,fitX,'r-')
        pl.plot(fitBackwardPWM,fitX,'b-')

        pl.pause(1)
            
    def savePVFDtoMCU(self):
        self.SendDataCommand(self._U_SetForwardA,self.sPVFD.SetForwardA)
        time.sleep(0.1)
        self.SendDataCommand(self._U_SetForwardB,self.sPVFD.SetForwardB)
        time.sleep(0.1)
        self.SendDataCommand(self._U_SetForwardC,self.sPVFD.SetForwardC)
        time.sleep(0.1)
        self.SendDataCommand(self._U_SetBackwardA,self.sPVFD.SetBackwardA)
        time.sleep(0.1)
        self.SendDataCommand(self._U_SetBackwardB,self.sPVFD.SetBackwardB)
        time.sleep(0.1)
        self.SendDataCommand(self._U_SetBackwardC,self.sPVFD.SetBackwardC)
        time.sleep(0.1)
 
    def stopVoltageVsPWMCurse(self):
        self.stopVoltageVsPWMCurse = True                  
    def GetVotage(self):
        try:
            ouput = 0
            if self.thirdUIControl.PWMPID.isChecked() and False:
                self.SendDataCommandWithAnswer(self._U_SetDura,0)
                time.sleep(0.1)
                res=self.read()
                ret = res.split(",")
                str = ""
                i=0                
                for x in ret:                                    
                    str += self.pidPara[i]+":"+x+" , "
                    if i == 5:
                        ouput = int(x)
                    i = i+1
                print(str)
                res=self.read()
                print(res)
                
            self.SendDataCommandWithAnswer(self._U_GetVotage,0)
            time.sleep(0.1)
            res=self.read()
            #return self.getRandom() 
            if res is None or len(res)<3:
                self.errorMessage("读取位置信息返回长度出错")
                return self.getRandom() 
            temp = res[2:].split(',')
            if len(temp)==3:
                return [float(temp[0]),float(temp[1]),float(temp[2])]
            else:
                self.errorMessage("读取位置信息返回数据出错")
                return self.getRandom()
        except:
            self.logMessage("读取串口出错")            
            return self.getRandom()
        
    def getRandom(self):
            mid=1084
            start = 1
            end = 100
            return [mid+random.randint(start, end)/100,mid+random.randint(start, end)/100,mid+random.randint(start, end)/100] 
    
    def read(self,terminator='\n', size=None):
        try:
            res=self.comm.read_until()
            if(len(res)>0):
                return res.decode('ascii').replace('\n', '', 1)
            else:
                return None
        except:
            self.errorMessage("读取串口信息失败")
        
    def ConnectTest(self):
        commName = self.firstUIComm.CommName.currentText()
        Baudrate = self.firstUIComm.Baudrate.currentText()   
        try:   
            self.comm = serial.Serial(commName,int(Baudrate),timeout=1)              
        except:
            self.errorMessage("串口"+commName+"被其他程序占用")
            return self.lastError
         
        self.SendDataCommandWithAnswer(self._U_GetVotage,0)
        res=self.read() 
        if res is not None and len(res)>2 and str(res)[:2] == "@P" :
            self.isDeviceReady = True
            self.logMessage("连接成功")
        else:
            self.isDeviceReady = False
            self.errorMessage("连接失败，请检查串口参数")

        return self.lastError
        #self.comm.close()
    def CloseComm(self):
        if  self.comm is None:
            pass
        else:
            self.comm.close()
    def logMessage(self,str):
        self.lastError = str
        print('-'*10+str)
    def log(self,str):
        pass
        #print('-'*10+str)
    def errorMessage(self,str):
        self.lastError = str
        print('!'*10+str)
    def warnningMessage(self,str):
        print('?'*10+str)
         
