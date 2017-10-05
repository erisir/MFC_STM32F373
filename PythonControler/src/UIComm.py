# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UIComm.ui'
#
# Created by: PyQt5 UI code generator 5.7
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(520, 440)
        self.horizontalLayoutWidget = QtWidgets.QWidget(Dialog)
        self.horizontalLayoutWidget.setGeometry(QtCore.QRect(30, 390, 471, 25))
        self.horizontalLayoutWidget.setObjectName("horizontalLayoutWidget")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.horizontalLayoutWidget)
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.label = QtWidgets.QLabel(self.horizontalLayoutWidget)
        self.label.setObjectName("label")
        self.horizontalLayout.addWidget(self.label)
        self.CommName = QtWidgets.QComboBox(self.horizontalLayoutWidget)
        self.CommName.setObjectName("CommName")
        self.CommName.addItem("")
        self.CommName.addItem("")
        self.CommName.addItem("")
        self.CommName.addItem("")
        self.CommName.addItem("")
        self.CommName.addItem("")
        self.CommName.addItem("")
        self.CommName.addItem("")
        self.horizontalLayout.addWidget(self.CommName)
        self.label_2 = QtWidgets.QLabel(self.horizontalLayoutWidget)
        self.label_2.setObjectName("label_2")
        self.horizontalLayout.addWidget(self.label_2)
        self.Baudrate = QtWidgets.QComboBox(self.horizontalLayoutWidget)
        self.Baudrate.setObjectName("Baudrate")
        self.Baudrate.addItem("")
        self.Baudrate.addItem("")
        self.Baudrate.addItem("")
        self.Baudrate.addItem("")
        self.Baudrate.addItem("")
        self.Baudrate.addItem("")
        self.Baudrate.addItem("")
        self.Baudrate.addItem("")
        self.horizontalLayout.addWidget(self.Baudrate)
        self.connect = QtWidgets.QPushButton(self.horizontalLayoutWidget)
        self.connect.setObjectName("connect")
        self.horizontalLayout.addWidget(self.connect)
        self.Disconnect = QtWidgets.QPushButton(self.horizontalLayoutWidget)
        self.Disconnect.setObjectName("Disconnect")
        self.horizontalLayout.addWidget(self.Disconnect)
        self.graphicsView = QtWidgets.QGraphicsView(Dialog)
        self.graphicsView.setGeometry(QtCore.QRect(30, 20, 471, 361))
        self.graphicsView.setStyleSheet("background-color: rgb(85, 255, 0);")
        self.graphicsView.setObjectName("graphicsView")

        self.retranslateUi(Dialog)
        self.CommName.setCurrentIndex(3)
        self.Baudrate.setCurrentIndex(7)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Dialog"))
        self.label.setText(_translate("Dialog", "串口"))
        self.CommName.setItemText(0, _translate("Dialog", "COM1"))
        self.CommName.setItemText(1, _translate("Dialog", "COM2"))
        self.CommName.setItemText(2, _translate("Dialog", "COM3"))
        self.CommName.setItemText(3, _translate("Dialog", "COM4"))
        self.CommName.setItemText(4, _translate("Dialog", "COM5"))
        self.CommName.setItemText(5, _translate("Dialog", "COM6"))
        self.CommName.setItemText(6, _translate("Dialog", "COM7"))
        self.CommName.setItemText(7, _translate("Dialog", "COM8"))
        self.label_2.setText(_translate("Dialog", "波特率"))
        self.Baudrate.setItemText(0, _translate("Dialog", "4800"))
        self.Baudrate.setItemText(1, _translate("Dialog", "9600"))
        self.Baudrate.setItemText(2, _translate("Dialog", "14400"))
        self.Baudrate.setItemText(3, _translate("Dialog", "19200"))
        self.Baudrate.setItemText(4, _translate("Dialog", "28800"))
        self.Baudrate.setItemText(5, _translate("Dialog", "38400"))
        self.Baudrate.setItemText(6, _translate("Dialog", "57600"))
        self.Baudrate.setItemText(7, _translate("Dialog", "115200"))
        self.connect.setText(_translate("Dialog", "连接"))
        self.Disconnect.setText(_translate("Dialog", "断开"))

