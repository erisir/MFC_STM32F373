from PyQt5 import  QtGui
from matplotlib.backends.backend_qt5agg import  FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure
import numpy as np
from array import array
import time
import win32api
import random
import os
import threading
from datetime import datetime
from matplotlib.dates import  date2num, MinuteLocator, SecondLocator, DateFormatter
from PyQt5.QtGui import *  
from PyQt5.QtGui import *  
from PyQt5.QtCore import *  
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import  * 
import pyqtgraph as pg
from scipy.fftpack import fft,ifft
import math
X_SECOND = 50
Y_MAX = 6000
Y1_MAX = 120000
Y_MIN = 1

PlotThreadInterval = 50
getDataThreadInterval = 50
 
MAXCOUNTER = 100

AutoRange =True
Debug = True
DrawFigure = True


class DateAxis(pg.AxisItem):
    def __init__(self, *args, **kwds):
        pg.AxisItem.__init__(self, *args, **kwds)
        self.setTickSpacing(8000, 2000)
    def tickStrings(self, values, scale, spacing):
        #print(spacing)
        strings = []
        for x in values:
            try:
                h = x/(60*60*1000)
                x = x%(60*60*1000)
                m = x/(60*1000)
                
                x = x%(60*1000)
                
                s = x/(1000)
                strings.append(str(int(h))+':'+str(int(m))+':'+str(int(s)))
            except ValueError:  ## Windows can't handle dates before 1970
                strings.append('')
        return strings

class CustomViewBox(pg.ViewBox):
    def __init__(self, *args, **kwds):
        pg.ViewBox.__init__(self, *args, **kwds)
        #self.setMouseMode(self.RectMode)
        #self.setMouseEnabled( x=False, y=False)
        #self.setMouseEnabled(self, x=None, y=None):
        
    ## reimplement right-click to zoom out
    def wheelEvent(self, ev):
        ev.ignore()
    def mouseClickEvent(self, ev):
        #self.autoRange()
        ev.ignore()
        pass
        if ev.button() == QtCore.Qt.RightButton:
            pass
        if ev.button() == QtCore.Qt.MiddleButton:
            pass
        if ev.button() == QtCore.Qt.MidButton:
            pass
        if ev.button() == QtCore.Qt.LeftButton:
            pass
            #
    def resizeEvent(self, ev):
        ev.ignore()       
    def mouseDragEvent(self, ev):
        ev.ignore()
        pass
        if ev.button() == QtCore.Qt.RightButton:
            ev.ignore()
        else:
            pg.ViewBox.mouseDragEvent(self, ev)  
class MplCanvas(FigureCanvas):
    curveVCh0= None # draw object
    curveVCh1 = None # draw object
    curveVSetpoint= None # draw object     
    curveVSetpointTemp= None # draw object        
    curvePWMOut= None # draw object
    
    def __init__(self):
        self.fig = Figure()
        self.ax = self.fig.add_subplot(111)
        self.ax1 =  self.ax .twinx()

        FigureCanvas.__init__(self, self.fig)
        FigureCanvas.setSizePolicy(self, QSizePolicy.Expanding, QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)
                    
        self.initFigureUI(self.ax,"time/s",'Vol(mv)')
        self.initFigureUI(self.ax1,"time/s",'PWM')
        self.ax.set_ylim(Y_MIN,Y_MAX)
        self.ax.set_ylim(Y_MIN,Y1_MAX)
        
    def initFigureUI(self,ax,xlabel,ylabel):   
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel)       
        
        ax.xaxis.set_major_locator(SecondLocator(range(0,60,2)))
        ax.xaxis.set_minor_locator(SecondLocator(range(0,60,1)))
        ax.xaxis.set_major_formatter( DateFormatter('%M:%S') )  
        
    def getPlotRange(self,data,stdFactor,stdMin,stdMax):
        mean = int(np.mean(data))
        std = np.std(data)*stdFactor
        if std <stdMin:
            std =stdMin
        if std >stdMax:
            std =stdMax
            
        ystart = mean-std
        if ystart <0:
            ystart = 0
        yend = mean+std*2
        return [ystart,yend]
        
    def plot(self,dataX,ydataVCh0,ydataVCh1,ydataVSetPoint,ydataVSetPointTemp,ydataPWMOut):
        if  not DrawFigure:
            return
        if self.curveVCh0 is None:
            #create draw object once
            self.curveVCh0, = self.ax.plot_date(np.array(dataX), np.array(ydataVCh0),'r-',label=u"Ch0")
            self.curveVCh1, = self.ax.plot_date(np.array(dataX), np.array(ydataVCh0),'b-',label=u"Ch1")
                   
            self.curveVSetpoint, = self.ax.plot_date(np.array(dataX), np.array(ydataVCh0),'k-')#,label=u"Set"
            self.curveVSetpointTemp, = self.ax.plot_date(np.array(dataX), np.array(ydataVCh0),'y-')#,label=u"SetT"
           
            self.curvePWMOut, = self.ax1.plot_date(np.array(dataX), np.array(ydataVCh0),'g-')#label=u"PWM"
            
            self.ax.legend()
            self.ax.grid()
             
        else:
            #update data of draw object
            npx = np.array(dataX)
             
            self.curveVCh0.set_data(npx, np.array(ydataVCh0))
            self.curveVCh1.set_data(npx,np.array(ydataVCh1))
            if Debug:
                self.curveVSetpoint.set_data(npx,np.array(ydataVSetPoint))
                self.curveVSetpointTemp.set_data(npx,np.array(ydataVSetPointTemp))
                
                self.curvePWMOut.set_data(npx, np.array(ydataPWMOut))
                
                ret1 = self.getPlotRange(ydataPWMOut,5,5,3000)  
                self.ax1.set_ylim(ret1[0],ret1[1])
            #update limit of X axis,to make sure it can move           
            self.ax.set_xlim(dataX[0],dataX[-1]+(dataX[-1]-dataX[0])/10)
            
            if AutoRange:
                ret = self.getPlotRange(ydataVCh0,5,5,3000) 
                ret1 = self.getPlotRange(ydataVCh1,5,5,3000)  
                if ret[1]>ret1[1]:                                                           
                    self.ax.set_ylim(ret[0],ret[1])
                else:
                    self.ax.set_ylim(ret1[0],ret1[1])
                            
        ticklabels = self.ax.xaxis.get_ticklabels()
        for tick in ticklabels:
            tick.set_rotation(5)
        self.draw()         
class  MyDynamicMplCanvas(QWidget):
    Interception=655
    Slope=25
    startTime =0
    app = None
    fileHandle = None
 
    UI_getpoint=None
    UI_getpointbar = None
    UI_setpointbar = None
    getpoint_Value = 0
    setpoint_Value = 0
    getDataTimer = None
    PlotCounter=0;
    ptr = 0
    datay = []
        
    def __init__(self , parent =None):
        QWidget.__init__(self, parent)
        self.win = pg.GraphicsWindow()
        pg.setConfigOptions(antialias=True)
        pg.setConfigOption('foreground', 'w')
        
        axis = [DateAxis(orientation='bottom'),DateAxis(orientation='bottom'),DateAxis(orientation='bottom'),DateAxis(orientation='bottom'),DateAxis(orientation='bottom')]
        vb = [CustomViewBox(),CustomViewBox(),CustomViewBox(),CustomViewBox(),CustomViewBox(),CustomViewBox()]
        self.p1 = self.win.addPlot(viewBox=vb[0],axisItems={'bottom': axis[0]})
        self.win.nextRow()
        self.p2 = self.win.addPlot(viewBox=vb[1],axisItems={'bottom': axis[1]})
        self.win.nextRow()
        self.p3 = self.win.addPlot(viewBox=vb[2],axisItems={'bottom': axis[2]})
         
        
        self.canvas = MplCanvas()
        self.vbl = QVBoxLayout()
        #self.ntb = NavigationToolbar(self.canvas, parent)
        #self.vbl.addWidget(self.ntb)
        #self.vbl.addWidget(self.canvas)
        self.vbl.addWidget(self.win)
        self.setLayout(self.vbl)
        self.dataX= []
        
        self.ydataVCh0= []
        self.ydataVCh1 = []
        self.ydataVSetPoint = []
        self.kp = []
        self.ki = []
        self.kd = []
        
        self.ydataPWMOut = []
        
    def InitGUI(self,action,getpoint,getpointbar,setpointbar,appHandle):
        self.app = appHandle
        self.UIAction = action
        self.UI_getpoint=getpoint
        self.UI_getpointbar = getpointbar
        self.UI_setpointbar = setpointbar
    def setTimers(self,timers):
        self.timers = timers
        self.timers[0].timeout.connect(self.plotThread)
        self.timers[1].timeout.connect(self.dataThread)
    def startPlot(self):
        self.UIAction.thirdUIControl.startPlot.setEnabled(False)
        self.UIAction.thirdUIControl.pausePlot.setEnabled(True) 
        
        if self.fileHandle is  None:
            self.startTime = date2num(datetime.now())
            dateStr = time.strftime("20%y-%m-%d",time.localtime())
            if not os.path.isdir('data'):
                os.mkdir('data');
            if not os.path.isdir('data/'+dateStr):
                os.mkdir('data/'+dateStr);
 
            self.fileHandle=open('data/'+dateStr+'/'+time.strftime("%H-%M-%S",time.localtime())+'.txt', 'a')
            self.fileHandle.write('time/s,vCh0,vCh1,Voltage_Set_Point,PWM_Output,kp,ki,kd\r\n')
        try:     
            if not self.timers[0].isActive():
                self.timers[0].start(PlotThreadInterval)
            if not self.timers[1].isActive():
                self.timers[1].start(getDataThreadInterval)#dataThread
                
        except Exception as e:
            print(e)
    def getTimeNum(self):
        timeString = str(datetime.now())
        return int(timeString[11:13])*60*60*1000+int(timeString[14:16])*60*1000+int(timeString[17:19])*1000+int(timeString[20:23])
    def getPlotRange(self,data,stdFactor,stdMin,stdMax):
        mean = int(np.mean(data))
        std = np.std(data)*stdFactor
        if std <stdMin:
            std =stdMin
        if std >stdMax:
            std =stdMax
            
        ystart = mean-std
        if ystart <0:
            ystart = 0
        yend = mean+std*2
        return [ystart,yend]
    def clearData(self):
        if self.dataX==None:
            return
        self.PlotCounter = 0
        self.dataX.clear()                    
        self.ydataVCh0.clear()                   
        self.ydataVCh1.clear()  
        self.ydataVSetPoint.clear()  
        self.ydataPWMOut.clear() 
        self.kp.clear()   
        self.ki.clear()   
        self.kd.clear()      
    def pausePlot(self,useSameFile):
        self.UIAction.thirdUIControl.startPlot.setEnabled(True)
        self.UIAction.thirdUIControl.pausePlot.setEnabled(False)
        if self.fileHandle is not None and not useSameFile:
            self.fileHandle.close()
            self.fileHandle = None
            self.clearData();
        try:     
            if self.timers[0].isActive():
                self.timers[0].stop()
            if self.timers[1].isActive():
                self.timers[1].stop()
        except Exception as e:
            print(e)
 
    def plotThread(self):  
        try:     
            self.UI_getpoint.display(str(self.getpoint_Value))
            if self.getpoint_Value>0:
                getpoint =int((self.getpoint_Value)/50)
            else:
                getpoint=self.getpoint_Value
            if self.setpoint_Value>0:
                setpoint =int((self.setpoint_Value)/50)
            else :
                setpoint = self.setpoint_Value 

            self.UI_getpointbar.setValue(getpoint)# )
            self.UI_setpointbar.setValue(setpoint)#)  
            
            if self.PlotCounter >= 20:   
                #[vCh0,vCh1,Voltage_Set_Point,PWM_Output,kp,ki,kd]                      
                self.p1.plot(x=self.dataX, y=self.ydataVCh0, pen=(255,0,0),clear=True) 
                #self.p1.plot(x=self.dataX, y=self.ydataVSetPoint, pen=(0,0,255))
                #self.p1.plot(x=self.dataX, y=self.ydataVCh1, pen=(0,255,0)) 
                #self.p1.plot(x=self.dataX, y=self.ydataVSetPoint, pen=(0,0,255))  
 
                self.p2.plot(x=self.dataX, y=self.ydataPWMOut, pen=(0,0,255),clear=True) 
                #.p2.plot(x=self.dataX, y=self.ydataVCh1, pen=(0,0,255),clear=True) 
              
                 
                #self.p3.plot(x=self.dataX, y=self.kp, pen=(255,0,0),clear=True)
                self.p3.plot(x=self.dataX, y=self.ki, pen=(0,255,0),clear=True)        
                #self.p3.plot(x=self.dataX, y=self.kd, pen=(0,0,255))    
                    
                #ret = self.getPlotRange(self.ydataVCh1,2,2,3000)  
                #self.p2.vb.setRange(yRange=[ret[0],ret[1]])
                #self.p2                             
                #self.canvas.plot(self.dataX,self.ydataVCh0,self.ydataVCh1,self.ydataVSetPoint,self.ydataVSetPointTemp,self.ydataPWMOut)                                       
        except:
            self.UIAction.errorMessage("绘图出错")
    def dataThread(self): 
        
        newTime= date2num(datetime.now())
        try:
            newData = self.UIAction.GetPlotData()
            if newData == None :
                return
            delta = newTime - self.startTime
            delta = delta*100000#s
            resStr =str(delta)+','+str(newData)+'\r\n'
            resStr = resStr.replace('[', '')
            resStr = resStr.replace(']', '')
            if self.fileHandle is not None:
                self.fileHandle.write(resStr )     
            self.dataX.append(self.getTimeNum())    
            
            # [vCh0,vCh1,Voltage_Set_Point,PWM_Output,kp,ki,kd]                 
            self.ydataVCh0.append(newData[0])                   
            self.ydataVCh1.append(newData[1])       
            self.ydataVSetPoint.append(newData[2])
            self.ydataPWMOut.append(newData[3])
            
            self.kp.append(newData[4])    
            self.ki.append(newData[5])    
            self.kd.append(newData[6])     
            
            self.getpoint_Value = newData[0]
            self.setpoint_Value = newData[2]
            
            if self.PlotCounter >= MAXCOUNTER:
                self.dataX.pop(0)
                
                self.ydataVCh0.pop(0) 
                self.ydataVCh1.pop(0) 
                
                self.ydataVSetPoint.pop(0) 
                self.ydataPWMOut.pop(0) 
                
                self.kp.pop(0) 
                self.ki.pop(0) 
                self.kd.pop(0) 
            else:
                self.PlotCounter+=1
            
        except Exception as e:
            print('DataGenerateThread'+str(e.args))

                
    # do something here