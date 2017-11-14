# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UIOther.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(476, 510)
        self.LogMsg = QtWidgets.QLabel(Dialog)
        self.LogMsg.setGeometry(QtCore.QRect(10, 10, 451, 461))
        self.LogMsg.setText("")
        self.LogMsg.setScaledContents(True)
        self.LogMsg.setWordWrap(True)
        self.LogMsg.setObjectName("LogMsg")

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Dialog"))

