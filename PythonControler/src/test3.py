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
comm = serial.Serial("COM5",int(115200))
buf = bytearray( 6 )
buf[0] = ord('$') 
buf[1]= ord('N' )
buf[2]= ord('<' )
buf[3]= 2
buf[4]= ord('8')
buf[5]= 0 
res = bytearray( 4096 )
fileHandle=open('data/Ch0'+time.strftime("%y%m%d%H%M%S",time.localtime())+'.txt', 'a')
def getData():
    global comm, buf,res
    comm.write(bytes(buf))    
    readLen=0
    time.sleep(2)
    while (comm.inWaiting() > 0):
        ret = bytes(comm.read(1))
        res[readLen] = ret[0]
        readLen +=1
 
    ch0 = []
    ch1 = []
    print(readLen)
    for x in range(0,512,4):
        t = struct.unpack_from("h", res, x)[0]
        v = 2.0* (((t + 32768) * 2862) / (1 * 65535))
        ch0.append(v)
        t = struct.unpack_from("h", res, x+2)[0]
        v = 2.0* (((t + 32768) * 2862) / (1 * 65535))
        ch1.append(v)
    
    
    return [ch0,ch1]

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
    ret = getData()
    xdata = []
    for x in range(0,len(ret[0])):
        xdata.append(x)
        fileHandle.write(str(x)+','+str(ret[0][x])+','+str(ret[1][x])+'\r\n')
    p2.plot(x=xdata, y=ret[0], pen=(255,0,0),clear=True)     
    #p2.plot(x=xdata, y=ret[1], pen=(0,255,0),clear=True)
 
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