
from PyQt5.QtGui import *  
from PyQt5.QtCore import *  
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import  *
import UIComm,UIControl,UIDetail,UIOther,UIControlProf
import time
import sys  
from UIAction import   UIAction

class UIMainWindow(QDialog):  
    def __init__(self,appHandle,parent=None):  
        super(UIMainWindow,self).__init__(parent)  
        self.app = appHandle
        self.firstUIComm=UIComm.Ui_Dialog()  
        self.secondUIDetail=UIDetail.Ui_Dialog()  
        self.thirdUIControl=UIControl.Ui_Dialog()
        self.fourUIOther=UIOther.Ui_Dialog()  
        
        self.UIControlProf=UIControlProf.Ui_Dialog()
        
        QtWidgets.QWidget
 
        tabWidget=QtWidgets.QTabWidget(self)  
        w1=QWidget()  
        self.firstUIComm.setupUi(w1)  
        w2=QWidget()  
        self.secondUIDetail.setupUi(w2)  
        w3=QWidget()  
        self.thirdUIControl.setupUi(w3)  
        w4=QWidget()  
        self.fourUIOther.setupUi(w4)  
      
        self.ProfControldlg = QDialog()  
        self.UIControlProf.setupUi(self.ProfControldlg )  
  
        tabWidget.addTab(w3,"通讯信息")  
        tabWidget.addTab(w2,"产品信息")  
        tabWidget.addTab(w1,"控制信息")  
        tabWidget.addTab(w4,"其他项目")  
        tabWidget.resize(520,560)
        
        self.uiAction = UIAction(self.firstUIComm,self.secondUIDetail,self.thirdUIControl,self.fourUIOther,self.UIControlProf)
        self.ConnectEvent()
              
        
    def ConnectEvent(self):
        self.thirdUIControl.PWMOpen.clicked.connect(self.uiAction.PWMOpen)
        self.thirdUIControl.PWMClose.clicked.connect(self.uiAction.PWMClose)
        self.thirdUIControl.PWMPID.clicked.connect(self.uiAction.PWMPID)
        self.thirdUIControl.CtrlMode_Dig.clicked.connect(self.uiAction.CtrlMode_Dig)
        self.thirdUIControl.CtrlMode_Vot.clicked.connect(self.uiAction.CtrlMode_Vot)
        self.thirdUIControl.CtrlMode_Cur.clicked.connect(self.uiAction.CtrlMode_Cur)
        self.thirdUIControl.CtrlMode_Der.clicked.connect(self.uiAction.CtrlMode_Der)
        self.thirdUIControl.ShowUnit_FS.clicked.connect(self.uiAction.ShowUnit_FS)
        self.thirdUIControl.ShowUnit_sccm.clicked.connect(self.uiAction.ShowUnit_sccm)
        self.thirdUIControl.ShowUnit_slm.clicked.connect(self.uiAction.ShowUnit_slm)
        self.thirdUIControl.ShowUnit_V.clicked.connect(self.uiAction.ShowUnit_V)
        self.thirdUIControl.ShowUnit_mv.clicked.connect(self.uiAction.ShowUnit_mv)
        self.thirdUIControl.mplCanvas.InitGUI(self.uiAction,self.thirdUIControl.GetPoint,self.thirdUIControl.GetPointBar,self.app)
        self.thirdUIControl.startPlot.clicked.connect(self.thirdUIControl.mplCanvas.startPlot)
        self.thirdUIControl.pausePlot.setEnabled(False)
        self.thirdUIControl.pausePlot.clicked.connect(self.thirdUIControl.mplCanvas.pausePlot)
        self.thirdUIControl.Clear.clicked.connect(self.Clear)
        self.thirdUIControl.Exit.clicked.connect(self.Exit)
        
    
   
        self.UIControlProf.SetPIDParam.clicked.connect(self.uiAction.SetPIDParam)
       
        self.UIControlProf.PID_SetPoint.valueChanged.connect(self.uiAction.SetRuningParam) 
        self.UIControlProf.PWM_SET.valueChanged.connect(self.uiAction.SetRuningParam)        
        
        self.firstUIComm.connect.clicked.connect(self.uiAction.Connect)
        self.firstUIComm.Disconnect.clicked.connect(self.uiAction.Disconnect)
        
        self.fourUIOther.ProfControl.clicked.connect(self.showProfControlDlg)
    def Start(self):
        return self.uiAction.Connect()
    def Exit(self):
        self.thirdUIControl.mplCanvas.releasePlot()
        #exit()
        sys.exit("goodbye!");
    def Clear(self):
        self.showProfControlDlg()
        pass
     
    def showProfControlDlg(self):
        #self.ProfControldlg.exec_()  #遮挡
        self.ProfControldlg.show()  #不遮挡
          
app=QApplication (sys.argv)
# Create and display the splash screen
splash_pix = QPixmap('../image/logo.png')
splash = QSplashScreen(splash_pix, Qt.WindowStaysOnTopHint)
# adding progress bar
height= splash_pix.height()*0.85
width = splash_pix.width()*0.8
progressBar = QProgressBar(splash)
progressBar.setGeometry(QtCore.QRect(60, height, width, 10))
progressBar.setProperty("value", 24)
progressBar.setObjectName("progressBar")

tipLabel = QtWidgets.QLabel(splash)
tipLabel.setGeometry(QtCore.QRect(60, height-30, width, 20))
tipLabel.setObjectName("label")
splash.setMask(splash_pix.mask())
splash.show()
time.sleep(0.1)
tipLabel.setText( "程序正在启动...")
time.sleep(1)
tipLabel.setText( "正在尝试连接下位机...")
mainApp=UIMainWindow(app) 
conectRes = mainApp.Start()
progressBar.setValue(50)
app.processEvents()
tipLabel.setText( conectRes)
time.sleep(0.5)
progressBar.setValue(99)
app.processEvents()
time.sleep(0.5)

mainApp.show()  
splash.finish(mainApp)  
app.exec_()