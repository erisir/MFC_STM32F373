# -*- coding: utf-8 -*-
"""
This example demonstrates many of the 2D plotting capabilities
in pyqtgraph. All of the plots may be panned/scaled by dragging with 
the left/right mouse buttons. Right click on any plot to show a context menu.
"""

#import initExample ## Add path to library (just for examples; you do not need this)


from pyqtgraph.Qt import QtGui, QtCore
import numpy as np
import pyqtgraph as pg
import random
import time

class DateAxis(pg.AxisItem):
    def tickStrings(self, values, scale, spacing):
        strns = []
        rng = max(values)-min(values)
        #if rng < 120:
        #    return pg.AxisItem.tickStrings(self, values, scale, spacing)
        if rng < 3600*24:
            string = '%H:%M:%S'
            label1 = '%b %d -'
            label2 = ' %b %d, %Y'
        elif rng >= 3600*24 and rng < 3600*24*30:
            string = '%d'
            label1 = '%b - '
            label2 = '%b, %Y'
        elif rng >= 3600*24*30 and rng < 3600*24*30*24:
            string = '%b'
            label1 = '%Y -'
            label2 = ' %Y'
        elif rng >=3600*24*30*24:
            string = '%Y'
            label1 = ''
            label2 = ''
        for x in values:
            try:
                strns.append(time.strftime(string, time.localtime(x)))
            except ValueError:  ## Windows can't handle dates before 1970
                strns.append('')
        try:
            label = time.strftime(label1, time.localtime(min(values)))+time.strftime(label2, time.localtime(max(values)))
        except ValueError:
            label = ''
        #self.setLabel(text=label)
        return strns
 

    #ptr =ptr+ 1
    

 
app = pg.mkQApp()

axis = DateAxis(orientation='bottom')
 

pw = pg.PlotWidget( axisItems={'bottom': axis}, enableMenu=False, title="PlotItem with custom axis and ViewBox<br>Menu disabled, mouse behavior changed: left-drag to zoom, right-click to reset zoom")
 
 
 

datay= []
datax = []
ptr = 0
dates = np.arange(8) * (24)
curve1 = pw.plot(x=[0], y=[0], symbol='o') 
#curve1 = pw.plot(x=dates, y=datay, symbol='o')
pw.show()
def update():
    global ptr,datay,datax,pw,dates,curve1
    datay.append(500+random.randint(1, 100)/100)
    ptr =ptr+ 1
    
    if ptr >7:
        datay[:-1] = datay[1:]
         
        #curve1 = pw.plot(x=dates, y=datay, symbol='o')
    #   curve1.setData(dates,datay)
    #   pw.show()
    #   datay.pop(0)
        
#timer = QtCore.QTimer()
#timer.timeout.connect(update)
#timer.start(10000)
## Start Qt event loop unless running in interactive mode or using pyside.
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
