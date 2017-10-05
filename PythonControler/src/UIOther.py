# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UIOther.ui'
#
# Created by: PyQt5 UI code generator 5.7
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(400, 300)
        self.ProfControl = QtWidgets.QPushButton(Dialog)
        self.ProfControl.setGeometry(QtCore.QRect(160, 220, 75, 23))
        self.ProfControl.setObjectName("ProfControl")

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Dialog"))
        self.ProfControl.setText(_translate("Dialog", "高级控制"))

