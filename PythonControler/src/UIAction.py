
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import  *
import matplotlib.pyplot as pl
import serial  #pip install pyserial
import sys,os,json
import time
import random #pip install random
import binascii,encodings; 
import numpy as np
from sympy.strategies.core import switch
from struct import pack,unpack
from PyQt5 import QtCore, QtGui, QtWidgets

class ErrorMsg():
    _when = ""
    _why  = ""
    _what = ""
    
class UIAction():     
    firstUIComm = None
    secondUIDetail= None
    thirdUIControl= None
    fourUIOther= None
    proControl = None
    isDeviceReady = False
    comm = None
    commBusy = False
    error = ErrorMsg()
    timeOutCounter = 0
    config = {}
    stopVoltageVsPWMCurse = False
    getRandomMid = 1000
    _CMD_SetRunParam = 0
    _CMD_SetPIDParam = 1
    _CMD_GetRunParam = 2
    _CMD_GetPIDParam = 3
    _CMD_SetVClose   = 4
    _CMD_SetVOpen    = 5
    _CMD_SetVPID     = 6
    _CMD_SetFuzzyMap = 7
    _CMD_ReadFuzzyMap = 8
    
    _DEBUG = 0
    _MSG = 1
    _ERROR = 2
    
 
    Interception = 0
    Slope = 1
    
    def __init__(self,  firstUIComm, secondUIDetail, thirdUIControl, fourUIOther,proControl):
        self.firstUIComm = firstUIComm
        self.secondUIDetail = secondUIDetail
        self.thirdUIControl = thirdUIControl
        self.fourUIOther = fourUIOther
        self.proControl = proControl
        self._translate = QtCore.QCoreApplication.translate
        self.ShowUnit_mv()
        
    def CheckSumCalc(self,buf):
        checkSum = 0;
        return checkSum;
    def SendToComAndRead(self,buf,timeout=10,expectLen=-1):
        self.error._what += '\t->SendToComAndRead'
        if self.isDeviceReady == False  :
            self.error._why='Device not ready'
            return None
        try:    
            while self.commBusy and timeout>0:
                time.sleep(0.01)  
                timeout -=1
            if timeout<=0:
                self.error._why= 'SendToComAndRead timeout,com is busy'
                self.timeOutCounter +=1
                return None 
            
            self.commBusy = True     
            if not self.clearComPort():
                return None
            self.comm.write(bytes(buf))
            res=self.readAnswer(timeout,expectLen)  
            self.commBusy = False
            return res
        except  Exception as e:
            self.error._why ='SendToComAndRead\t'+str(e.args)
            return None
    def SendCommand(self,cmd):
        buf = bytearray( 6 )
        buf[0] = ord('$') 
        buf[1]= ord('N' )
        buf[2]= ord('<' )
        buf[3]= 2
        buf[4]= cmd
        buf[5]= self.CheckSumCalc(buf)  
        return self.SendToComAndRead(buf)
    def SendCommandWithData(self,buf):       
        return self.SendToComAndRead(buf)
    def SendCommandWitAnswer(self,buf,expectLen): 
        return self.SendToComAndRead(buf,1000,expectLen)
    def clearComPort(self):
        self.error._what += '\t->clearComPort'
        try:         
            while self.comm.inWaiting() > 0:
                    bytes(self.comm.read(1))
            return True
        except  Exception as e:
            self.error._why ='clearComPort\t'+str(e.args)
            return False
    def pausePlot(self):
        self.thirdUIControl.startPlot.setEnabled(True)
        self.thirdUIControl.pausePlot.setEnabled(False)
        try:     
            if self.timers[0].isActive():
                self.timers[0].stop()
            if self.timers[1].isActive():
                self.timers[1].stop()
        except Exception as e:
            print(e)
    def startPlot(self):
        self.thirdUIControl.startPlot.setEnabled(False)
        self.thirdUIControl.pausePlot.setEnabled(True) 
        try:     
            if not self.timers[0].isActive():
                self.timers[0].start(self.thirdUIControl.mplCanvas.PlotThreadInterval)
            if not self.timers[1].isActive():
                self.timers[1].start(self.thirdUIControl.mplCanvas.getDataThreadInterval)#dataThread               
        except Exception as e:
            print(e)
    def setTimers(self,timers):
        self.timers = timers
        #self.timer.timeout.connect(self.plotThread)
    def changeSetPoint(self):
        #v=self.proControl.PID_SetPoint.value()
        #self.proControl.PWM_STEP.setProperty("value", self.Bytes2Int32_t(res))
        #self.SetRuningParam(self)
        pass
    def readAnswer(self,timeout,expDataLen):     
        self.error._what+='\t->readAnswer'   
        data = bytearray(256)
        readLen = 0
        try:
            if expDataLen>0:   #read some len
                while(readLen <expDataLen and timeout>0):        
                    while (readLen <expDataLen and self.comm.inWaiting() > 0):
                        res = bytes(self.comm.read(1))
                        data[readLen] = res[0]
                        readLen +=1            
                    timeout = timeout-1  
                    time.sleep(0.001)
                if timeout<=0:
                    self.error._why = 'readAnswer timeout,read'+ str(readLen)+'bit'
                    self.timeOutCounter +=1
                    return None
            else:#read all
                time.sleep(0.1)
                while self.comm.inWaiting() > 0:
                    res = bytes(self.comm.read(1))
                    data[readLen] = res[0]
                    readLen +=1  
                
            ret = bytearray(readLen)
            for index in range(readLen):
                ret[index] = data[index]
            return ret#bytes(ret)
        except  Exception as e:
            self.error._why ='readAnswer\t'+str(e.args)
            return None
   
    def PWMOpen (self):
        self.logMessage(self._MSG,'PWMOpen')
        ret = self.SendCommand(self._CMD_SetVOpen)
        if ret is None:
            self.logMessage(self._ERROR, 'Open False')
        else:
            self.logMessage(self._MSG, str(ret.decode(encoding='utf-8')))
    def PWMClose (self):
        self.logMessage(self._MSG,'PWMClose')
        ret=self.SendCommand(self._CMD_SetVClose)
        if ret is None:
            self.logMessage(self._ERROR, 'Close False')
        else:
            self.logMessage(self._MSG, str(ret.decode(encoding='utf-8')))
    def PWMPID (self):
        self.logMessage(self._MSG,'PWMPID')
        ret=self.SendCommand(self._CMD_SetVPID)
        if ret is None:
            self.logMessage(self._ERROR, 'PID False')
        else:
            self.logMessage(self._MSG, str(ret.decode(encoding='utf-8')))
         
    def CtrlMode_Dig (self):
        pass
    def CtrlMode_Vot (self):
        pass
    def CtrlMode_Cur (self):
        pass
    def CtrlMode_Der (self):
        pass
    def ShowUnit_FS (self):
        self.showUnit = "FS"
        self.thirdUIControl.FlowSetUnit.setText(self._translate("Dialog", "% of F.S."))
        self.thirdUIControl.FlowShowUnit.setText(self._translate("Dialog", "% of F.S."))
    def ShowUnit_sccm (self):
        self.showUnit = "sccm"
        self.thirdUIControl.FlowSetUnit.setText(self._translate("Dialog", "sccm"))
        self.thirdUIControl.FlowShowUnit.setText(self._translate("Dialog", "sccm"))
    def ShowUnit_slm (self):
        self.showUnit = "slm"
        self.thirdUIControl.FlowSetUnit.setText(self._translate("Dialog", "slm"))
        self.thirdUIControl.FlowShowUnit.setText(self._translate("Dialog", "slm"))
    def ShowUnit_V (self):
        self.showUnit = "V"
        self.thirdUIControl.FlowSetUnit.setText(self._translate("Dialog", "V"))
        self.thirdUIControl.FlowShowUnit.setText(self._translate("Dialog", "V"))
    def ShowUnit_mv (self):
        self.showUnit = "mv"
        self.thirdUIControl.FlowSetUnit.setText(self._translate("Dialog", "mv"))
        self.thirdUIControl.FlowShowUnit.setText(self._translate("Dialog", "mv"))
    
    def Set_PID_AutoInc(self):
        pass
    def Set_PID_ManuInc(self):
        pass
        
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
        ret = 0;
        if self.showUnit == "FS":
            ret =votage/50
            if(ret>100):
                ret = 100
            if(ret<0):
                ret = 0
 
        if self.showUnit == "sccm":
            ret =votage/50
            if(ret>100):
                ret = 100
            if(ret<0):
                ret = 0
        if self.showUnit == "slm":
            ret =votage/50
            if(ret>100):
                ret = 100
            if(ret<0):
                ret = 0
        if self.showUnit == "V":
            ret= votage/1000
        if self.showUnit == "mv":
            ret= votage
        return ret

    def GetPlotData(self):
        
        #return int(Voltage_Set_Point,PWM_Output,lastVoltage,currentVol0,currentVol1)
        self.error._what = 'GetPlotData'
        #ret = self.getRandom();
        #ret = [Voltage_Set_Point,PWM_Output,lastVoltage,currentVol0,currentVol1,PID_kp,PID_ki,PID_kd]
        #return [ret[0],ret[1],ret[2],ret[0],ret[1],ret[2],ret[0],ret[1]]
        ret = self.readRunningParam()#self.readVoltage()#
        if ret == None:
            self.logMessage(self._DEBUG)
            return None
       
        Voltage_Set_Point=self.GetShowValue(ret[0])  
        PWM_Output = ret[1]          
        vCh0 = self.GetShowValue(ret[3])
        vCh1 = self.GetShowValue(ret[4])
        kp = ret[5]
        ki = ret[6]
        kd = ret[7]
        
        res = [vCh0,vCh1,Voltage_Set_Point,PWM_Output,kp,ki,kd] 
                        
        return res

    def getRandom(self):
            mid=self.getRandomMid
            start = 1
            end = 100
            return [mid+random.randint(start, end)/100,mid+random.randint(start, end)/100,mid+random.randint(start, end)/100] 
    
    def TryConnect(self):
        os.chdir(sys.path[0])  # 设置脚本所在目录为当前工作目录
        configFile = os.path.join(sys.path[0],"config.txt")
        if os.path.exists(configFile):
            with open(configFile, "r") as f:
                self.config = json.load(f)
            try:
                ret=self.AutoConnect(self.config['COMM'],self.config['Baudrate'])
                return ret[0]
            except:
                return False
        else:
            return False
    def Connect(self):
        commName =self.firstUIComm.CommName.currentText()
        Baudrate = self.firstUIComm.Baudrate.currentText()   
        self.error._what='open\t'+commName+'\tbaudrate\t'+Baudrate
        try:   
            self.comm = serial.Serial(commName,int(Baudrate))                          
        except Exception as e:
            self.error._why = str(e.args)
            self.logMessage(self._DEBUG)
            return False
        try:  
            self.isDeviceReady = True
            ret =  self.readPIDParam()
            
            if ret:
                self.isDeviceReady = True
                self.error._why ="连接成功"
                
            else:
                self.isDeviceReady = False
    
            self.logMessage(self._MSG,self.error._what+'\r\n'+self.error._why);  
            if ret:
                pass  
        except Exception as e:
            self.error._why = 'readPIDParam\t'+str(e.args)
            self.logMessage(self._DEBUG)
            return False       
 
    def AutoConnect(self,comm,Baud):
        commName =comm 
        Baudrate = Baud#self.firstUIComm.Baudrate.currentText()   
        self.error._what='open\t'+commName+'\tbaudrate\t'+Baudrate
        try:   
            self.comm = serial.Serial(str(commName),int(Baudrate))    
        except Exception as e:
            self.error._why = str(e.args)

            return [False,self.error]
        self.isDeviceReady = True
        ret =  self.readPIDParam()
        if ret:       
            self.isDeviceReady = True
            #os.chdir(sys.path[0])  # 设置脚本所在目录为当前工作目录
            #configFile = os.path.join(sys.path[0],"config.txt")
            #self.config['COMM']=comm
            #self.config['Baudrate']=Baud
            #with open(configFile, "w") as output:
            #    json.dump(self.config, output)
            self.error._why ="连接成功"
        else:
            self.isDeviceReady = False
            if  self.comm is not None:
                self.comm.close()
        return [ret,self.error]
    
    def Disconnect(self):
        if  self.comm is not None:
            self.comm.close()
        self.isDeviceReady = False  
    def Bytes2Int32_t(self,buf):
        offset = buf[3]
        buf[3] +=4
        return buf[offset+3]*256*256*256+buf[offset+2]*256*256+buf[offset+1]*256+buf[offset]
    def Bytes2Int16_t(self,buf):
        offset = buf[3]
        buf[3] +=2
        return buf[offset+1]*256+buf[offset]
    def Int16_t2Bytes(self,buf,value):
        offset = buf[3]
        buf[offset+2] = int(value/256)
        buf[offset+1] = int(value%256)
        buf[3] = offset+2
    def Int32_t2Bytes(self,buf,value):
        offset = buf[3]
        buf[3] = offset+4
        buf[offset+4] = int(value/(256*256*256))
        value = value%(256*256*256)
        buf[offset+3] = int(value/(256*256))
        value = value%(256*256)
        buf[offset+2] = int(value/256)
        buf[offset+1] = int(value%256)
        
    
    def SetPIDParam(self):#  $N< totalLen,CMD,[data]
        self.error._what = 'SetPIDParam'
        self.logMessage(self._MSG)
        buf = bytearray( 64 )
        offset = 4
        buf[0] = ord('$') 
        buf[1]=ord('N' )
        buf[2]= ord('<' )
        buf[3]= offset
        buf[4]= self._CMD_SetPIDParam
         
        self.Int16_t2Bytes(buf,self.proControl.PID_Kp.value()*100)
        self.Int16_t2Bytes(buf,self.proControl.PID_Ki.value()*100)
        self.Int16_t2Bytes(buf,self.proControl.PID_Kd.value()*100)
        
        self.Int16_t2Bytes(buf,self.proControl.PID_KpF.value()*100)
        self.Int16_t2Bytes(buf,self.proControl.PID_KiF.value()*100)
        self.Int16_t2Bytes(buf,self.proControl.PID_KdF.value()*100)
        
        self.Int16_t2Bytes(buf,self.proControl.PID_eL.value())
        self.Int16_t2Bytes(buf,self.proControl.PID_eM.value())
        self.Int16_t2Bytes(buf,self.proControl.PID_eS.value())
        
        self.Int16_t2Bytes(buf,self.proControl.PID_ecL.value())
        self.Int16_t2Bytes(buf,self.proControl.PID_ecM.value())
        self.Int16_t2Bytes(buf,self.proControl.PID_ecS.value())
        
        self.Int16_t2Bytes(buf,self.proControl.PID_Cutoff.value()*10)
        self.Int16_t2Bytes(buf,self.proControl.PID_SampleCycle.value())
        self.Int16_t2Bytes(buf,self.proControl.PID_DeadZone.value()*10)
        
        self.Int32_t2Bytes(buf,self.proControl.PWM_MAX.value())
        self.Int32_t2Bytes(buf,self.proControl.PWM_MIN.value())
        self.Int32_t2Bytes(buf,self.proControl.PWM_STEP.value())
        a = 10
         
        cmdLen = buf[3]+1
        dataLen = cmdLen -4#4 bytes head 
        buf[3]=dataLen                   
        sendBuf =  bytearray(cmdLen)  
        for x in range(0,cmdLen):
            sendBuf[x]=  buf[x]       
        self.thirdUIControl.mplCanvas.pausePlot(True); 
        time.sleep(0.05)  
        ret= self.SendCommandWithData(bytes(sendBuf))
        self.thirdUIControl.mplCanvas.startPlot();
        if ret is None:
            self.logMessage(self._ERROR)
        else:
            self.logMessage(self._MSG, str(ret.decode(encoding='utf-8')))
        
    def readPIDParam(self):
        self.error._what+='\t->readPIDParam'
        buf = bytearray( 6 )
        buf[0] = ord('$') 
        buf[1]=ord('N' )
        buf[2]= ord('<' )
        buf[3]= 6-4
        buf[4]= self._CMD_GetPIDParam
        buf[5]= ord('X' )
        ret = self.SendCommandWitAnswer(buf,45)
        
        if ret is  None:
            return False
        if True:
            if ret[0] != ord('$') :
                return False
            if ret[1] != ord('N' ):
                return False
            if ret[2] != ord('>' ):
                return False
            if ret[4] != self._CMD_GetPIDParam:
                return False
        offset = 5
        res = bytearray( 64 )
         
        for ii in range(0,len(ret)):
            res[ii]= ret[ii]
        res[3] = offset
        #32byte
        self.proControl.PID_Kp.setProperty("value", float(self.Bytes2Int16_t(res)/100))
        self.proControl.PID_Ki.setProperty("value", float(self.Bytes2Int16_t(res)/100))
        self.proControl.PID_Kd.setProperty("value", float(self.Bytes2Int16_t(res)/100))

        self.proControl.PID_KpF.setProperty("value", float(self.Bytes2Int16_t(res)/100))
        self.proControl.PID_KiF.setProperty("value", float(self.Bytes2Int16_t(res)/100))
        self.proControl.PID_KdF.setProperty("value", float(self.Bytes2Int16_t(res)/100))
     
        self.proControl.PID_eL.setProperty("value", self.Bytes2Int16_t(res))
        self.proControl.PID_eM.setProperty("value", self.Bytes2Int16_t(res))
        self.proControl.PID_eS.setProperty("value", self.Bytes2Int16_t(res))
                
        self.proControl.PID_ecL.setProperty("value", self.Bytes2Int16_t(res))
        self.proControl.PID_ecM.setProperty("value", self.Bytes2Int16_t(res))
        self.proControl.PID_ecS.setProperty("value", self.Bytes2Int16_t(res))
 
        self.proControl.PID_Cutoff.setProperty("value", float(self.Bytes2Int16_t(res)/10))
        self.proControl.PID_SampleCycle.setProperty("value", float(self.Bytes2Int16_t(res)))
        self.proControl.PID_DeadZone.setProperty("value", float(self.Bytes2Int16_t(res)/10))
 
        
        self.proControl.PWM_MAX.setProperty("value", self.Bytes2Int32_t(res))
        self.proControl.PWM_MIN.setProperty("value", self.Bytes2Int32_t(res))
        self.proControl.PWM_STEP.setProperty("value", self.Bytes2Int32_t(res))
        
        return True
   
    def SetRuningParam(self):
        self.error._what = 'SetRuningParam'
        self.logMessage(self._MSG)
        buf = bytearray( 16 )
        offset = 4
        buf[0] = ord('$') 
        buf[1]=ord('N' )
        buf[2]= ord('<' )
        buf[3]= offset
        buf[4]=  self._CMD_SetRunParam 
        self.getRandomMid = self.proControl.PID_SetPoint.value()
        self.Int16_t2Bytes(buf,self.proControl.PID_SetPoint.value())
        self.Int16_t2Bytes(buf,self.proControl.BackForward_End.value())
        
        cmdLen = buf[3]+1
        dataLen = cmdLen -4#4 bytes head 
        buf[3]=dataLen                   
        sendBuf =  bytearray(cmdLen)  
        for x in range(0,cmdLen):
            sendBuf[x]=  buf[x]       
        self.thirdUIControl.mplCanvas.pausePlot(True);
        time.sleep(0.05) 
        ret= self.SendCommandWithData(bytes(sendBuf))
        self.thirdUIControl.mplCanvas.startPlot();
        if ret is None:
            self.logMessage(self._ERROR)
        else:
            self.logMessage(self._MSG, str(ret.decode(encoding='utf-8')))
  
    def FuzzySave(self):
        pass
    def FuzzySet(self):
        self.error._what = 'SetFuzzyMap'
        self.logMessage(self._MSG)
        buf = bytearray( 27 )
        offset = 4
        buf[0] = ord('$') 
        buf[1]=ord('N' )
        buf[2]= ord('<' )
        buf[3]= offset
        buf[4]= self._CMD_SetFuzzyMap
        table = self.proControl.FuzzyMap;
        dic = {'NL':0,'NM':1,'NS':2,'ZE':3,'PS':4,'PM':5,'PL':6}

        self.thirdUIControl.mplCanvas.pausePlot(True);
        time.sleep(0.05) 
        for rows_index in range(7):#ec
            buf[5] = rows_index
            cmdLen = 5;
            for cols_index in range(7):#e
                v = table.item(rows_index,cols_index).text()
                v=v.replace(' ','')
                kpkikd = v.split(',')
                buf[6+3*cols_index]=dic[kpkikd[0]]
                buf[6+3*cols_index+1]=dic[kpkikd[1]]
                buf[6+3*cols_index+2]=dic[kpkikd[2]]
                cmdLen +=3
            cmdLen+=1
            dataLen = cmdLen -4#4 bytes head 
            buf[3]=dataLen                   
            sendBuf =  bytearray(cmdLen)  
            for x in range(0,cmdLen):
                sendBuf[x]=  buf[x]             
            ret= self.SendCommandWithData(bytes(sendBuf))
            
            if ret is None:
                self.logMessage(self._ERROR)
            else:
                self.logMessage(self._MSG, str(ret.decode(encoding='utf-8')))
            time.sleep(0.05) 
        self.thirdUIControl.mplCanvas.startPlot();
    def FuzzyRead(self):
        buf = bytearray( 6 )
        buf[0] = ord('$') 
        buf[1]=ord('N' )
        buf[2]= ord('<' )
        buf[3]= 2
        buf[4]= self._CMD_ReadFuzzyMap
        buf[5]= ord('X' )
        res = self.SendCommandWitAnswer(buf,196) 
        if res is  None:
            return None    
        if True:
            if res[0] != ord('$') :
                return None
            if res[1] != ord('N' ):
                return None
            if res[2] != ord('>' ):
                return None
            if res[4] != self._CMD_ReadFuzzyMap:
                return None    
       
        offset = 5
        res[3]= offset
        table = self.proControl.FuzzyMap;
        dic = {0:'NL',1:'NM',2:'NS' ,3:'ZE' ,4:'PS' ,5:'PM' ,6:'PL' }
        tableValue  =np.array(res).reshape(7,-1)
        for rows_index in range(7):#ec
            buf[5] = rows_index
            v = tableValue[rows_index][5:]
            #06 05 04 06 06 04 05 06 03 05 06 03 04 06 03 03 06 04 03 05 04 68
            for cols_index in range(7):#e
                t = str(dic[v[cols_index*3 ]])+','+str(dic[v[cols_index*3+1 ]])+','+str(dic[v[cols_index*3 +2]])
                print(t)
                newItem = QTableWidgetItem(t)
                table.setItem(rows_index, cols_index, newItem)
                
 
 
    def readRunningParam(self):
        buf = bytearray( 6 )
        buf[0] = ord('$') 
        buf[1]=ord('N' )
        buf[2]= ord('<' )
        buf[3]= 2
        buf[4]= self._CMD_GetRunParam
        buf[5]= ord('X' )
        res = self.SendCommandWitAnswer(buf,39) 
        
        if res is  None:
            return None    
        if True:
            if res[0] != ord('$') :
                return None
            if res[1] != ord('N' ):
                return None
            if res[2] != ord('>' ):
                return None
            if res[4] != self._CMD_GetRunParam:
                return None    
       
        offset = 5
        res[3]= offset
        Voltage_Set_Point =self.Bytes2Int16_t(res); 
        PWM_Output = self.Bytes2Int32_t(res); 
        lastVoltage = self.Bytes2Int16_t(res); 
        
        currentVol0 = self.Bytes2Int16_t(res)/10; 
        currentVol1 = self.Bytes2Int16_t(res)/10+self.thirdUIControl.SetPoint.value();
        
        PID_kp = self.Bytes2Int16_t(res)/100; 
        PID_ki = self.Bytes2Int16_t(res)/100; 
        PID_kd = self.Bytes2Int16_t(res)/100; 
              
        ret = [Voltage_Set_Point,PWM_Output,lastVoltage,currentVol0,currentVol1,PID_kp,PID_ki,PID_kd]
        timersInfo  =[self.Bytes2Int32_t(res),self.Bytes2Int32_t(res),self.Bytes2Int32_t(res),self.Bytes2Int32_t(res)]; 
        print(timersInfo)
        return ret
     
    def logMessage(self,flag,msg=None):# debug
        if msg is None:
            msg = self.error._what+'\r\n'+self.error._why
        msg = msg.replace('\n','')
        print(msg)
        msg +=str(self.timeOutCounter)
        if flag ==self._DEBUG:
            self.fourUIOther.LogMsg.setText(msg)
        if flag ==self._ERROR:
            self.thirdUIControl.DebugMsg.setText(msg)
            self.fourUIOther.LogMsg.setText(msg)
        if flag == self._MSG:
            self.thirdUIControl.DebugMsg.setText(msg)
            self.fourUIOther.LogMsg.setText(msg)
        
