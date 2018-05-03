# -*- coding: utf-8 -*-
"""
Various methods of drawing scrolling plots.
"""
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
import numpy as np
import time
import random
from datetime import datetime
from matplotlib.dates import  date2num

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
import struct
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
import numpy as np
import time
import random
import math
from datetime import datetime
from matplotlib.dates import date2num, num2date

class DateAxis(pg.AxisItem):
    def __init__(self, *args, **kwds):
        pg.AxisItem.__init__(self, *args, **kwds)
        self.setTickSpacing(4000, 1000)
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
                strings.append(str(int(h))+'-'+str(int(m))+'-'+str(int(s)))
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
        self.autoRange()
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
win = pg.GraphicsWindow()

win.setWindowTitle('pyqtgraph example: Scrolling Plots')
axis = DateAxis(orientation='bottom')
vb = CustomViewBox()
p2 = win.addPlot()#viewBox=vb,axisItems={'bottom': axis})
#curve2 = p2.plot(data1)
count = 0
ydata = []
ydata1 = []
ydata2 = []
xdata = []
def getHysData():
    f=40;
    dt=1/(f*1000);
    NTo=2;
    Hmax=10000;
    a=1000;
    alpha=0.001;
    c=0.1;
    b=1000;
    Ms=1.7E+6;
    k=2000;
    len1=round(NTo/(f*dt));
    T=[];#range(0,len*f/NTo)
    hs = []
    for x in range(0,int(len1*f/NTo)):
        T.append(x)
        hs.append(math.sin(2*3.14*f*x))
 
    print('******************');
    print('Setting up Simulation');
    print('******************');
   
    s=len(hs);
    len2=s;
    col=0;
    print('************');
    print('Simulation set up & runing......');
    print('************');
    print('');
    
    #Main JA Algorithm
    mirr=0;
    m=0;
    OUT1=[];
    OUT2=[];
    OUT3=[];
    lasthe=0;
    lastm=0;
    for i in range(2,len2):
        dm=1;n=0;
        h=hs[i];     
        if  i < len2:  
            dhs=hs[i+1]-hs[i];
            if(dhs>0):
                delta=1;
            else:
                delta=-1;
    
        while(n<1) and (dm>1E-10):
            print(n)
            print('-')
            print(dm)
            H=h*Hmax;
            M=m*Ms;
            He=H+alpha*M;      
            he=He/Hmax;
            dhe=he-lasthe;    
            a=(((He)/a)-a/(He));
            man = math.cosh(a)/math.sinh(a)
            dmirr=(Hmax/k)*(man-mirr)*delta*dhe;
            mirr=mirr+dmirr;    
            m=c*man+(1-c)*mirr;
            dm=lastm-m;
            lasthe=he;
            lastm=m;
            n=n+1;
 
        OUT1.append(M); 
        OUT2.append(man*Ms); 
        OUT3.append(he*Hmax);
    return [hs,OUT1]
 
def getData():   
    return [13,12,13,12,13,12,13,12,13,12,13,12,13,12,13,12]

def getMinute(date=datetime.now()):
    return str(date)[14:16]
def getHour(date=datetime.now()):
    return str(date)[11:13]

def getSecond(date=datetime.now()):
    return str(date)[17:19]
def getMiliSecond(date=datetime.now()):
    return str(date)[20:23]
def getTimeNum(date):
    timeString = str(date)
    return int(timeString[11:13])*60*60*1000+int(timeString[14:16])*60*1000+int(timeString[17:19])*1000+int(timeString[20:23])
def update1():
    ret = getHysData()
    #xdata = []
    #for x in range(0,len(ret)):
    #    xdata.append(x)
    p2.plot(x=ret[0], y=ret[1], pen=(255,0,0),clear=True)     

 
# update all plots
def update():
    update1()

timer = pg.QtCore.QTimer()
timer.timeout.connect(update)
timer.start(100)



## Start Qt event loop unless running in interactive mode or using pyside.
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
    comm.close()
    fileHandle.close()