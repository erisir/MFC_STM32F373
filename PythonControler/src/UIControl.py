# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UIControl.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(531, 568)
        self.BoxCtrl = QtWidgets.QGroupBox(Dialog)
        self.BoxCtrl.setGeometry(QtCore.QRect(13, 10, 111, 80))
        self.BoxCtrl.setObjectName("BoxCtrl")
        self.verticalLayoutWidget_2 = QtWidgets.QWidget(self.BoxCtrl)
        self.verticalLayoutWidget_2.setGeometry(QtCore.QRect(10, 18, 71, 62))
        self.verticalLayoutWidget_2.setObjectName("verticalLayoutWidget_2")
        self.verticalLayout_2 = QtWidgets.QVBoxLayout(self.verticalLayoutWidget_2)
        self.verticalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.PWMOpen = QtWidgets.QRadioButton(self.verticalLayoutWidget_2)
        self.PWMOpen.setObjectName("PWMOpen")
        self.verticalLayout_2.addWidget(self.PWMOpen)
        self.PWMClose = QtWidgets.QRadioButton(self.verticalLayoutWidget_2)
        self.PWMClose.setObjectName("PWMClose")
        self.verticalLayout_2.addWidget(self.PWMClose)
        self.PWMPID = QtWidgets.QRadioButton(self.verticalLayoutWidget_2)
        self.PWMPID.setObjectName("PWMPID")
        self.verticalLayout_2.addWidget(self.PWMPID)
        self.BoxMode = QtWidgets.QGroupBox(Dialog)
        self.BoxMode.setGeometry(QtCore.QRect(13, 94, 111, 111))
        self.BoxMode.setObjectName("BoxMode")
        self.verticalLayoutWidget = QtWidgets.QWidget(self.BoxMode)
        self.verticalLayoutWidget.setGeometry(QtCore.QRect(10, 20, 81, 84))
        self.verticalLayoutWidget.setObjectName("verticalLayoutWidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.verticalLayoutWidget)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setObjectName("verticalLayout")
        self.CtrlMode_Dig = QtWidgets.QRadioButton(self.verticalLayoutWidget)
        self.CtrlMode_Dig.setObjectName("CtrlMode_Dig")
        self.verticalLayout.addWidget(self.CtrlMode_Dig)
        self.CtrlMode_Vot = QtWidgets.QRadioButton(self.verticalLayoutWidget)
        self.CtrlMode_Vot.setObjectName("CtrlMode_Vot")
        self.verticalLayout.addWidget(self.CtrlMode_Vot)
        self.CtrlMode_Cur = QtWidgets.QRadioButton(self.verticalLayoutWidget)
        self.CtrlMode_Cur.setObjectName("CtrlMode_Cur")
        self.verticalLayout.addWidget(self.CtrlMode_Cur)
        self.groupBox_3 = QtWidgets.QGroupBox(Dialog)
        self.groupBox_3.setGeometry(QtCore.QRect(130, 14, 381, 191))
        self.groupBox_3.setTitle("")
        self.groupBox_3.setObjectName("groupBox_3")
        self.groupBox_4 = QtWidgets.QGroupBox(self.groupBox_3)
        self.groupBox_4.setGeometry(QtCore.QRect(73, 10, 151, 51))
        self.groupBox_4.setObjectName("groupBox_4")
        self.horizontalLayoutWidget_2 = QtWidgets.QWidget(self.groupBox_4)
        self.horizontalLayoutWidget_2.setGeometry(QtCore.QRect(5, 12, 141, 31))
        self.horizontalLayoutWidget_2.setObjectName("horizontalLayoutWidget_2")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(self.horizontalLayoutWidget_2)
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.SetPoint = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.SetPoint.setKeyboardTracking(False)
        self.SetPoint.setObjectName("SetPoint")
        self.horizontalLayout_2.addWidget(self.SetPoint)
        self.label_3 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_3.setObjectName("label_3")
        self.horizontalLayout_2.addWidget(self.label_3)
        self.groupBox_5 = QtWidgets.QGroupBox(self.groupBox_3)
        self.groupBox_5.setGeometry(QtCore.QRect(73, 70, 151, 51))
        self.groupBox_5.setObjectName("groupBox_5")
        self.horizontalLayoutWidget_3 = QtWidgets.QWidget(self.groupBox_5)
        self.horizontalLayoutWidget_3.setGeometry(QtCore.QRect(5, 13, 141, 31))
        self.horizontalLayoutWidget_3.setObjectName("horizontalLayoutWidget_3")
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout(self.horizontalLayoutWidget_3)
        self.horizontalLayout_3.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.GetPoint = QtWidgets.QLCDNumber(self.horizontalLayoutWidget_3)
        self.GetPoint.setAutoFillBackground(False)
        self.GetPoint.setStyleSheet("color: rgb(85, 255, 0);\n"
"selection-color: rgb(255, 0, 0);\n"
"font: 75 9pt \"Aharoni\";\n"
"gridline-color: rgb(85, 0, 0);\n"
"border-color: rgb(0, 0, 255);\n"
"background-color: rgb(170, 255, 0);")
        self.GetPoint.setSmallDecimalPoint(False)
        self.GetPoint.setObjectName("GetPoint")
        self.horizontalLayout_3.addWidget(self.GetPoint)
        self.label_4 = QtWidgets.QLabel(self.horizontalLayoutWidget_3)
        self.label_4.setObjectName("label_4")
        self.horizontalLayout_3.addWidget(self.label_4)
        self.BoxUnit = QtWidgets.QGroupBox(self.groupBox_3)
        self.BoxUnit.setGeometry(QtCore.QRect(230, 10, 120, 121))
        self.BoxUnit.setObjectName("BoxUnit")
        self.verticalLayoutWidget_3 = QtWidgets.QWidget(self.BoxUnit)
        self.verticalLayoutWidget_3.setGeometry(QtCore.QRect(10, 10, 61, 106))
        self.verticalLayoutWidget_3.setObjectName("verticalLayoutWidget_3")
        self.verticalLayout_3 = QtWidgets.QVBoxLayout(self.verticalLayoutWidget_3)
        self.verticalLayout_3.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.ShowUnit_FS = QtWidgets.QRadioButton(self.verticalLayoutWidget_3)
        self.ShowUnit_FS.setObjectName("ShowUnit_FS")
        self.verticalLayout_3.addWidget(self.ShowUnit_FS)
        self.ShowUnit_sccm = QtWidgets.QRadioButton(self.verticalLayoutWidget_3)
        self.ShowUnit_sccm.setObjectName("ShowUnit_sccm")
        self.verticalLayout_3.addWidget(self.ShowUnit_sccm)
        self.ShowUnit_slm = QtWidgets.QRadioButton(self.verticalLayoutWidget_3)
        self.ShowUnit_slm.setObjectName("ShowUnit_slm")
        self.verticalLayout_3.addWidget(self.ShowUnit_slm)
        self.ShowUnit_V = QtWidgets.QRadioButton(self.verticalLayoutWidget_3)
        self.ShowUnit_V.setObjectName("ShowUnit_V")
        self.verticalLayout_3.addWidget(self.ShowUnit_V)
        self.ShowUnit_mv = QtWidgets.QRadioButton(self.verticalLayoutWidget_3)
        self.ShowUnit_mv.setObjectName("ShowUnit_mv")
        self.verticalLayout_3.addWidget(self.ShowUnit_mv)
        self.horizontalLayoutWidget = QtWidgets.QWidget(self.groupBox_3)
        self.horizontalLayoutWidget.setGeometry(QtCore.QRect(60, 141, 320, 31))
        self.horizontalLayoutWidget.setObjectName("horizontalLayoutWidget")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.horizontalLayoutWidget)
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.startPlot = QtWidgets.QPushButton(self.horizontalLayoutWidget)
        self.startPlot.setObjectName("startPlot")
        self.horizontalLayout.addWidget(self.startPlot)
        self.pausePlot = QtWidgets.QPushButton(self.horizontalLayoutWidget)
        self.pausePlot.setEnabled(True)
        self.pausePlot.setObjectName("pausePlot")
        self.horizontalLayout.addWidget(self.pausePlot)
        self.Clear = QtWidgets.QPushButton(self.horizontalLayoutWidget)
        self.Clear.setObjectName("Clear")
        self.horizontalLayout.addWidget(self.Clear)
        self.Exit = QtWidgets.QPushButton(self.horizontalLayoutWidget)
        self.Exit.setObjectName("Exit")
        self.horizontalLayout.addWidget(self.Exit)
        self.gridLayoutWidget = QtWidgets.QWidget(self.groupBox_3)
        self.gridLayoutWidget.setGeometry(QtCore.QRect(10, 10, 51, 161))
        self.gridLayoutWidget.setObjectName("gridLayoutWidget")
        self.gridLayout = QtWidgets.QGridLayout(self.gridLayoutWidget)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.SetPointBar = QtWidgets.QProgressBar(self.gridLayoutWidget)
        self.SetPointBar.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.SetPointBar.setStyleSheet("\n"
" QProgressBar::chunk {\n"
"   background-color: #000000;\n"
"}\n"
"QProgressBar {\n"
"   text-align: top;\n"
"}")
        self.SetPointBar.setProperty("value", 0)
        self.SetPointBar.setOrientation(QtCore.Qt.Vertical)
        self.SetPointBar.setObjectName("SetPointBar")
        self.gridLayout.addWidget(self.SetPointBar, 0, 0, 1, 1)
        self.label = QtWidgets.QLabel(self.gridLayoutWidget)
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 1, 0, 1, 1)
        self.GetPointBar = QtWidgets.QProgressBar(self.gridLayoutWidget)
        self.GetPointBar.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.GetPointBar.setStyleSheet("\n"
" QProgressBar::chunk {\n"
"   background-color: #FF0000;\n"
"}\n"
"QProgressBar {\n"
"   text-align: top;\n"
"    color:#FF0000\n"
"}")
        self.GetPointBar.setProperty("value", 50)
        self.GetPointBar.setOrientation(QtCore.Qt.Vertical)
        self.GetPointBar.setObjectName("GetPointBar")
        self.gridLayout.addWidget(self.GetPointBar, 0, 1, 1, 1)
        self.label_2 = QtWidgets.QLabel(self.gridLayoutWidget)
        self.label_2.setObjectName("label_2")
        self.gridLayout.addWidget(self.label_2, 1, 1, 1, 1)
        self.DebugMsg = QtWidgets.QLabel(self.groupBox_3)
        self.DebugMsg.setGeometry(QtCore.QRect(10, 170, 331, 16))
        self.DebugMsg.setText("")
        self.DebugMsg.setScaledContents(True)
        self.DebugMsg.setWordWrap(True)
        self.DebugMsg.setObjectName("DebugMsg")
        self.mplCanvas = MyDynamicMplCanvas(Dialog)
        self.mplCanvas.setGeometry(QtCore.QRect(10, 209, 491, 341))
        self.mplCanvas.setObjectName("mplCanvas")

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Dialog"))
        self.BoxCtrl.setTitle(_translate("Dialog", "阀控制"))
        self.PWMOpen.setText(_translate("Dialog", "全开"))
        self.PWMClose.setText(_translate("Dialog", "关闭"))
        self.PWMPID.setText(_translate("Dialog", "常规"))
        self.BoxMode.setTitle(_translate("Dialog", "控制模式"))
        self.CtrlMode_Dig.setText(_translate("Dialog", "数字"))
        self.CtrlMode_Vot.setText(_translate("Dialog", "模拟电压"))
        self.CtrlMode_Cur.setText(_translate("Dialog", "模拟电流"))
        self.groupBox_4.setTitle(_translate("Dialog", "设定值"))
        self.label_3.setText(_translate("Dialog", "% of F.S."))
        self.groupBox_5.setTitle(_translate("Dialog", "瞬时流量"))
        self.label_4.setText(_translate("Dialog", "% of F.S."))
        self.BoxUnit.setTitle(_translate("Dialog", "单位"))
        self.ShowUnit_FS.setText(_translate("Dialog", "% F.S."))
        self.ShowUnit_sccm.setText(_translate("Dialog", "sccm"))
        self.ShowUnit_slm.setText(_translate("Dialog", "slm"))
        self.ShowUnit_V.setText(_translate("Dialog", "V"))
        self.ShowUnit_mv.setText(_translate("Dialog", "mV"))
        self.startPlot.setText(_translate("Dialog", "开始"))
        self.pausePlot.setText(_translate("Dialog", "暂停"))
        self.Clear.setText(_translate("Dialog", "设置"))
        self.Exit.setText(_translate("Dialog", "退出"))
        self.label.setText(_translate("Dialog", "SP."))
        self.label_2.setText(_translate("Dialog", "RO."))

from mplCanvasWrapper import MyDynamicMplCanvas
