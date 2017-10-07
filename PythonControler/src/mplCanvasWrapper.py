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

X_MINUTES = 0.15
Y_MAX = 3200
Y_MIN = 1
INTERVAL = 0.05
MAXCOUNTER = 50

class MplCanvas(FigureCanvas):
    def __init__(self):
        self.fig = Figure()
        self.ax = self.fig.add_subplot(111)
        self.ax1 =  self.ax .twinx()
        #self.ax1 = self.fig.add_subplot(212)
        FigureCanvas.__init__(self, self.fig)
        FigureCanvas.setSizePolicy(self, QSizePolicy.Expanding, QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)
        self.curveVRef = None # draw object
        self.curveVSetpoint= None # draw object
        self.curveVSensor= None # draw object
        self.curvePWMOut= None # draw object
            
        self.initFigureUI(self.ax,"time (sec)",'Voltage(mv)')
        self.initFigureUI(self.ax1,"time (sec)",'OutPut(pwm)')
        
    def initFigureUI(self,ax,xlabel,ylabel):   
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel)
        ax.set_ylim(Y_MIN,Y_MAX)
        #self.ax.xaxis.set_major_locator(MinuteLocator())  # every minute is a major locator
        ax.xaxis.set_major_locator(SecondLocator([0,5,10,15,20,25,30,35,40,45,50,55]))
        ax.xaxis.set_minor_locator(SecondLocator())
        #self.ax.xaxis.set_minor_locator(SecondLocator([10,20,30,40,50])) # every 10 second is a minor locator
        ax.xaxis.set_major_formatter( DateFormatter('%M:%S') ) #tick label formatter
        
    
    def plot(self,dataX,ydataVCh0,ydataVSetPoint,ydataVCh1,ydataPWMOut):
        if self.curveVRef is None:
            #create draw object once
            self.curveVRef, = self.ax.plot_date(np.array(dataX), np.array(ydataVCh0),'b-',label=u"VRef")
            self.curveVSetpoint, = self.ax.plot_date(np.array(dataX), np.array(ydataVCh0),'k-',label=u"VSetpoint")
            self.curveVSensor, = self.ax.plot_date(np.array(dataX), np.array(ydataVCh0),'r-',label=u"VSensor")
            self.curvePWMOut, = self.ax1.plot_date(np.array(dataX), np.array(ydataVCh0),'g-',label=u"PWM")
            #self.ax.legend()
            self.ax.grid()
             
        else:
            #update data of draw object
            npx = np.array(dataX)
            npy =  np.array(ydataVCh0)
            npy0 =  np.array(ydataVSetPoint)
            npy1 =  np.array(ydataVCh1)
            npy2 =  np.array(ydataPWMOut)
            meany = int(np.mean(ydataVCh0))
            std = np.std(ydataVCh0)*10
            if std <5:
                std =5
            if std >2500:
                std =2500
            ystart = meany-std
            if ystart <0:
                ystart = 0
            yend = meany+std
            meany = int(np.mean(ydataPWMOut))
            std = np.std(ydataPWMOut)*10
            if std <5:
                std =5
            ystart1 = meany-std
            if ystart1 <0:
                ystart1 = 0
            yend1 = meany+std

            self.curveVRef.set_data(npx,npy)
            self.curveVSetpoint.set_data(npx,npy0)
            self.curveVSensor.set_data(npx, npy1)
            self.curvePWMOut.set_data(npx, npy2)
            #update limit of X axis,to make sure it can move
            
            self.ax.set_xlim(dataX[0],dataX[-1]+(dataX[-1]-dataX[0])/10)
            
            #ystart = 0
            #yend = 6000
            
            #ystart1 = 0
            #yend1 = 60000
            self.ax.set_ylim(ystart,yend)
            self.ax1.set_ylim(ystart1,yend1)
            
        ticklabels = self.ax.xaxis.get_ticklabels()
        for tick in ticklabels:
            tick.set_rotation(5)
        self.draw()   
    
       
class  MyDynamicMplCanvas(QWidget):
    Interception=655
    Slope=25
    startTime =0
    app = None
    def on_close(self):
        print("close")
        self.tData.Close()
    def __init__(self , parent =None):
        QWidget.__init__(self, parent)
        self.canvas = MplCanvas()
        self.vbl = QVBoxLayout()
        self.ntb = NavigationToolbar(self.canvas, parent)
        self.vbl.addWidget(self.ntb)
        self.vbl.addWidget(self.canvas)
        self.setLayout(self.vbl)
        self.dataX= []
        self.ydataVCh0= []
        self.ydataVSetPoint = []
        self.ydataVCh1 = []
        self.ydataPWMOut = []
        self.initDataGenerator()
        win32api.SetConsoleCtrlHandler(self.on_close, True)

        #self.startTime = date2num(datetime.now())
        
    def InitGUI(self,action,getpoint,getpointbar):
        #self.app = appHandle
        self.UIAction = action
        self.getpoint=getpoint
        self.getpointbar = getpointbar
        
    def startPlot(self):
        self.UIAction.thirdUIControl.startPlot.setEnabled(False)
        self.UIAction.thirdUIControl.pausePlot.setEnabled(True)
        #self.startTime = date2num(datetime.now())
        self.__generating = True

    def pausePlot(self):
        self.UIAction.thirdUIControl.startPlot.setEnabled(True)
        self.UIAction.thirdUIControl.pausePlot.setEnabled(False)
        self.__generating = False
        #self.UIAction.CloseComm()
    def isPlotRunning(self):
        return self.__generating;
    def initDataGenerator(self):
        self.__generating=False
        self.__exit = False
        self.tData = threading.Thread(name = "dataGenerator",target = self.generateData)
        self.tData.start()
        
    def releasePlot(self):
        self.__exit  = True
        self.tData.join()
    
    def generateData(self):
        counter=0
        while(True):
            if self.__exit:
                break
            if self.__generating:
                newTime= date2num(datetime.now())
                #return [Voltage_Set_Point,PWM_Output,vCh0,vCh1]
                newData = self.UIAction.GetPlotData()
                if newData is None :
                    continue
                try:                            
                    self.dataX.append(newTime)
                    
                    self.ydataVCh0.append(newData[2])
                    self.ydataVSetPoint.append(newData[0])
                    self.ydataVCh1.append(newData[3])
                    self.ydataPWMOut.append(newData[1])
                    
                    #self.getpoint.setProperty("value", newData[2])
                    self.getpoint.display(str(newData[2]))
                    #self.getpointbar.setValue(int(newData[2]/2500))
                    self.canvas.plot(self.dataX,self.ydataVCh0,self.ydataVSetPoint,self.ydataVCh1,self.ydataPWMOut)   
                    #self.app.processEvents()
                    if counter >= MAXCOUNTER:
                        self.dataX.pop(0)
                        self.ydataVCh0.pop(0) 
                        self.ydataVSetPoint.pop(0) 
                        self.ydataVCh1.pop(0) 
                        self.ydataPWMOut.pop(0) 
                    else:
                        counter+=1
                except:
                    self.UIAction.errorMessage("绘图出错")
                    continue 
            time.sleep(INTERVAL)

    # do something here