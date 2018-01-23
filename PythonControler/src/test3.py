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
p2 = win.addPlot(viewBox=vb,axisItems={'bottom': axis})
#curve2 = p2.plot(data1)
count = 0
ydata = []
ydata1 = []
ydata2 = []
xdata = []
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
    global xdata, ydata,ydata1,ydata2, count,curve2,p2
    ydata.append(random.randint(10,100))
    ydata1.append(random.randint(40,60))
    ydata2.append(random.randint(120,150))
    xdata.append(getTimeNum(datetime.now()))#date2num(datetime.now()))
    #print(time.strftime('%H:%M:%S',time.localtime()))
    if count <=100:
        count +=  1
    if count >100:        
        # pw.plot(x=xdata, y=ydata, clear=True)
        ydata.pop(0)
        ydata1.pop(0)
        ydata2.pop(0)
        xdata.pop(0)
        
        #xdata[:-1] = xdata[1:]  # shift data in the array one sample left
        #ydata[:-1] = ydata[1:]  # shift data in the array one sample left
        #curve2.setData(x=xdata, y=ydata)
    #curve2.setPos(ptr1, 0)
    p2.plot(x=xdata, y=ydata, pen=(255,0,0),clear=True)
    p2.plot(x=xdata, y=ydata1, pen=(0,255,0))
    p2.plot(x=xdata, y=ydata2, pen=(0,0,255))
# update all plots
def update():
    update1()

timer = pg.QtCore.QTimer()
timer.timeout.connect(update)
timer.start(50)



## Start Qt event loop unless running in interactive mode or using pyside.
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
