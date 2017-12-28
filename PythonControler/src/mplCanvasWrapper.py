from PyQt5 import  QtGui
from matplotlib.backends.backend_qt5agg import  FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure
import numpy as np
from array import array
import time
import win32api
import random
import threading
from datetime import datetime
from matplotlib.dates import  date2num, MinuteLocator, SecondLocator, DateFormatter
from PyQt5.QtGui import *  
from PyQt5.QtGui import *  
from PyQt5.QtCore import *  
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import  * 
 

X_SECOND = 50
Y_MAX = 6000
Y1_MAX = 120000
Y_MIN = 1

PlotThreadInterval = 0.1
getDataThreadInterval = 0.05
timerInterval = 0.1;
MAXCOUNTER = 40

AutoRange =True
Debug = True
DrawFigure = True
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
        #self.win = pg.GraphicsWindow(title="Basic plotting examples")
        #pg.setConfigOptions(antialias=True)
        #pg.setConfigOption('foreground', 'w')
        #self.p6 = self.win.addPlot(title="Updating plot")
        #self.curve = self.p6.plot(pen='y')
        self.canvas = MplCanvas()
        self.vbl = QVBoxLayout()
        self.ntb = NavigationToolbar(self.canvas, parent)
        self.vbl.addWidget(self.ntb)
        self.vbl.addWidget(self.canvas)
        #self.vbl.addWidget(self.win)
        self.setLayout(self.vbl)
        self.dataX= []
        
        self.ydataVCh0= []
        self.ydataVCh1 = []
        self.ydataVSetPoint = []
        self.ydataVSetPointTemp = []
        
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
        self.startTime = date2num(datetime.now())
        if self.fileHandle is  None:
            #self.fileHandle=open('data/'+time.strftime("%y%m%d",time.localtime())+'/'+time.strftime("%H%M%S",time.localtime())+'.txt', 'a')
            self.fileHandle=open('data/'+time.strftime("%y%m%d%H%M%S",time.localtime())+'.txt', 'a')
        try:     
            if not self.timers[0].isActive():
                self.timers[0].start(20)
            if not self.timers[1].isActive():
                self.timers[1].start(20)#dataThread
        except Exception as e:
            print(e)
    def clearData(self):
        if self.dataX==None:
            return
        self.PlotCounter = 0
        self.dataX.clear()                    
        self.ydataVCh0.clear()                   
        self.ydataVCh1.clear()  
        self.ydataVSetPoint.clear()  
        self.ydataVSetPointTemp.clear()         
        self.ydataPWMOut.clear()      
    def pausePlot(self):
        self.UIAction.thirdUIControl.startPlot.setEnabled(True)
        self.UIAction.thirdUIControl.pausePlot.setEnabled(False)
        if self.fileHandle is not None:
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
        self.datay.append(500+random.randint(1, 100)/100)
        #if self.ptr >20:
        #    self.curve.setData(self.datay)
        #self.ptr += 1
        #return
        try:     
            self.UI_getpoint.display(str(self.getpoint_Value))
            #self.setpoint.display(str(newData[2]))
            getpoint =int((self.getpoint_Value-1000)*100)
            if getpoint<0:
                getpoint = 0
            if getpoint >90:
                getpoint = 90
            #setpoint = int(self.setpoint_Value/50)
            setpoint =int((self.setpoint_Value-1000)*100)
            if setpoint <0:
                setpoint =0;
            if setpoint >90:
                setpoint = 90
            self.UI_getpointbar.setValue(getpoint)# )
            self.UI_setpointbar.setValue(setpoint)#)                
            if self.PlotCounter >= 3:                                                             
                self.canvas.plot(self.dataX,self.ydataVCh0,self.ydataVCh1,self.ydataVSetPoint,self.ydataVSetPointTemp,self.ydataPWMOut)                                       
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
            self.dataX.append(newTime)                        
            self.ydataVCh0.append(newData[0])                   
            self.ydataVCh1.append(newData[1])       
            self.ydataVSetPoint.append(newData[2])
            self.ydataVSetPointTemp.append(newData[3])            
            self.ydataPWMOut.append(newData[4])
            
            self.getpoint_Value = newData[0]
            self.setpoint_Value = newData[2]
            
            if self.PlotCounter >= MAXCOUNTER:
                self.dataX.pop(0)
                
                self.ydataVCh0.pop(0) 
                self.ydataVCh1.pop(0) 
                
                self.ydataVSetPoint.pop(0) 
                self.ydataVSetPointTemp.pop(0) 
                
                self.ydataPWMOut.pop(0) 
            else:
                self.PlotCounter+=1
            
        except Exception as e:
            print('DataGenerateThread'+str(e.args))

                
    # do something here