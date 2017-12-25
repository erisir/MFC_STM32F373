# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UIControlProf.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(553, 549)
        self.groupBox = QtWidgets.QGroupBox(Dialog)
        self.groupBox.setGeometry(QtCore.QRect(10, 10, 521, 231))
        self.groupBox.setObjectName("groupBox")
        self.SetPIDParam = QtWidgets.QPushButton(self.groupBox)
        self.SetPIDParam.setGeometry(QtCore.QRect(270, 200, 75, 23))
        self.SetPIDParam.setObjectName("SetPIDParam")
        self.horizontalLayoutWidget_2 = QtWidgets.QWidget(self.groupBox)
        self.horizontalLayoutWidget_2.setGeometry(QtCore.QRect(10, 20, 506, 81))
        self.horizontalLayoutWidget_2.setObjectName("horizontalLayoutWidget_2")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(self.horizontalLayoutWidget_2)
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.formLayout_3 = QtWidgets.QFormLayout()
        self.formLayout_3.setObjectName("formLayout_3")
        self.label = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label.setObjectName("label")
        self.formLayout_3.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label)
        self.PID_KpH = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_KpH.setKeyboardTracking(False)
        self.PID_KpH.setDecimals(1)
        self.PID_KpH.setMaximum(65535.0)
        self.PID_KpH.setSingleStep(0.1)
        self.PID_KpH.setProperty("value", 6.3)
        self.PID_KpH.setObjectName("PID_KpH")
        self.formLayout_3.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.PID_KpH)
        self.label_2 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_2.setObjectName("label_2")
        self.formLayout_3.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.label_2)
        self.PID_KiH = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_KiH.setKeyboardTracking(False)
        self.PID_KiH.setDecimals(1)
        self.PID_KiH.setMaximum(65535.0)
        self.PID_KiH.setSingleStep(0.1)
        self.PID_KiH.setProperty("value", 6.5)
        self.PID_KiH.setObjectName("PID_KiH")
        self.formLayout_3.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.PID_KiH)
        self.label_3 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_3.setObjectName("label_3")
        self.formLayout_3.setWidget(2, QtWidgets.QFormLayout.LabelRole, self.label_3)
        self.PID_KdH = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_KdH.setKeyboardTracking(False)
        self.PID_KdH.setDecimals(1)
        self.PID_KdH.setMaximum(65535.0)
        self.PID_KdH.setSingleStep(0.1)
        self.PID_KdH.setProperty("value", 2.2)
        self.PID_KdH.setObjectName("PID_KdH")
        self.formLayout_3.setWidget(2, QtWidgets.QFormLayout.FieldRole, self.PID_KdH)
        self.horizontalLayout_2.addLayout(self.formLayout_3)
        self.formLayout_6 = QtWidgets.QFormLayout()
        self.formLayout_6.setObjectName("formLayout_6")
        self.label_16 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_16.setObjectName("label_16")
        self.formLayout_6.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label_16)
        self.label_17 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_17.setObjectName("label_17")
        self.formLayout_6.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.label_17)
        self.PID_KiM = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_KiM.setKeyboardTracking(False)
        self.PID_KiM.setDecimals(1)
        self.PID_KiM.setMaximum(65535.0)
        self.PID_KiM.setSingleStep(0.1)
        self.PID_KiM.setProperty("value", 6.5)
        self.PID_KiM.setObjectName("PID_KiM")
        self.formLayout_6.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.PID_KiM)
        self.label_18 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_18.setObjectName("label_18")
        self.formLayout_6.setWidget(2, QtWidgets.QFormLayout.LabelRole, self.label_18)
        self.PID_KdM = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_KdM.setKeyboardTracking(False)
        self.PID_KdM.setDecimals(1)
        self.PID_KdM.setMaximum(65535.0)
        self.PID_KdM.setSingleStep(0.1)
        self.PID_KdM.setProperty("value", 2.2)
        self.PID_KdM.setObjectName("PID_KdM")
        self.formLayout_6.setWidget(2, QtWidgets.QFormLayout.FieldRole, self.PID_KdM)
        self.PID_KpM = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_KpM.setKeyboardTracking(False)
        self.PID_KpM.setDecimals(1)
        self.PID_KpM.setMaximum(65535.0)
        self.PID_KpM.setSingleStep(0.1)
        self.PID_KpM.setProperty("value", 6.3)
        self.PID_KpM.setObjectName("PID_KpM")
        self.formLayout_6.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.PID_KpM)
        self.horizontalLayout_2.addLayout(self.formLayout_6)
        self.formLayout_7 = QtWidgets.QFormLayout()
        self.formLayout_7.setObjectName("formLayout_7")
        self.label_19 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_19.setObjectName("label_19")
        self.formLayout_7.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label_19)
        self.PID_KpL = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_KpL.setKeyboardTracking(False)
        self.PID_KpL.setDecimals(1)
        self.PID_KpL.setMaximum(65535.0)
        self.PID_KpL.setSingleStep(0.1)
        self.PID_KpL.setProperty("value", 6.3)
        self.PID_KpL.setObjectName("PID_KpL")
        self.formLayout_7.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.PID_KpL)
        self.label_20 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_20.setObjectName("label_20")
        self.formLayout_7.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.label_20)
        self.PID_KiL = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_KiL.setKeyboardTracking(False)
        self.PID_KiL.setDecimals(1)
        self.PID_KiL.setMaximum(65535.0)
        self.PID_KiL.setSingleStep(0.1)
        self.PID_KiL.setProperty("value", 6.5)
        self.PID_KiL.setObjectName("PID_KiL")
        self.formLayout_7.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.PID_KiL)
        self.label_21 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_21.setObjectName("label_21")
        self.formLayout_7.setWidget(2, QtWidgets.QFormLayout.LabelRole, self.label_21)
        self.PID_KdL = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_KdL.setKeyboardTracking(False)
        self.PID_KdL.setDecimals(1)
        self.PID_KdL.setMaximum(65535.0)
        self.PID_KdL.setSingleStep(0.1)
        self.PID_KdL.setProperty("value", 2.2)
        self.PID_KdL.setObjectName("PID_KdL")
        self.formLayout_7.setWidget(2, QtWidgets.QFormLayout.FieldRole, self.PID_KdL)
        self.horizontalLayout_2.addLayout(self.formLayout_7)
        self.formLayout = QtWidgets.QFormLayout()
        self.formLayout.setObjectName("formLayout")
        self.label_6 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_6.setObjectName("label_6")
        self.formLayout.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.label_6)
        self.PID_ThredHold_Low = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_ThredHold_Low.setKeyboardTracking(False)
        self.PID_ThredHold_Low.setDecimals(0)
        self.PID_ThredHold_Low.setMaximum(65535.0)
        self.PID_ThredHold_Low.setProperty("value", 100.0)
        self.PID_ThredHold_Low.setObjectName("PID_ThredHold_Low")
        self.formLayout.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.PID_ThredHold_Low)
        self.PID_ThredHold_High = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_ThredHold_High.setKeyboardTracking(False)
        self.PID_ThredHold_High.setDecimals(0)
        self.PID_ThredHold_High.setMaximum(65535.0)
        self.PID_ThredHold_High.setSingleStep(50.0)
        self.PID_ThredHold_High.setProperty("value", 2000.0)
        self.PID_ThredHold_High.setObjectName("PID_ThredHold_High")
        self.formLayout.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.PID_ThredHold_High)
        self.label_8 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_8.setObjectName("label_8")
        self.formLayout.setWidget(2, QtWidgets.QFormLayout.LabelRole, self.label_8)
        self.PID_ThredHold_PWM = QtWidgets.QDoubleSpinBox(self.horizontalLayoutWidget_2)
        self.PID_ThredHold_PWM.setKeyboardTracking(False)
        self.PID_ThredHold_PWM.setDecimals(0)
        self.PID_ThredHold_PWM.setMinimum(0.0)
        self.PID_ThredHold_PWM.setMaximum(65535.0)
        self.PID_ThredHold_PWM.setSingleStep(500.0)
        self.PID_ThredHold_PWM.setProperty("value", 5000.0)
        self.PID_ThredHold_PWM.setObjectName("PID_ThredHold_PWM")
        self.formLayout.setWidget(2, QtWidgets.QFormLayout.FieldRole, self.PID_ThredHold_PWM)
        self.label_7 = QtWidgets.QLabel(self.horizontalLayoutWidget_2)
        self.label_7.setObjectName("label_7")
        self.formLayout.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label_7)
        self.horizontalLayout_2.addLayout(self.formLayout)
        self.formLayoutWidget_8 = QtWidgets.QWidget(self.groupBox)
        self.formLayoutWidget_8.setGeometry(QtCore.QRect(10, 110, 160, 100))
        self.formLayoutWidget_8.setObjectName("formLayoutWidget_8")
        self.formLayout_8 = QtWidgets.QFormLayout(self.formLayoutWidget_8)
        self.formLayout_8.setContentsMargins(0, 0, 0, 0)
        self.formLayout_8.setObjectName("formLayout_8")
        self.label_11 = QtWidgets.QLabel(self.formLayoutWidget_8)
        self.label_11.setObjectName("label_11")
        self.formLayout_8.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label_11)
        self.Delay_Time = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_8)
        self.Delay_Time.setKeyboardTracking(False)
        self.Delay_Time.setDecimals(2)
        self.Delay_Time.setMinimum(0.0)
        self.Delay_Time.setMaximum(10.0)
        self.Delay_Time.setSingleStep(0.5)
        self.Delay_Time.setProperty("value", 0.1)
        self.Delay_Time.setObjectName("Delay_Time")
        self.formLayout_8.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.Delay_Time)
        self.label_9 = QtWidgets.QLabel(self.formLayoutWidget_8)
        self.label_9.setObjectName("label_9")
        self.formLayout_8.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.label_9)
        self.Responce_Time = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_8)
        self.Responce_Time.setAutoFillBackground(True)
        self.Responce_Time.setKeyboardTracking(False)
        self.Responce_Time.setDecimals(1)
        self.Responce_Time.setMaximum(10.0)
        self.Responce_Time.setProperty("value", 0.3)
        self.Responce_Time.setObjectName("Responce_Time")
        self.formLayout_8.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.Responce_Time)
        self.label_15 = QtWidgets.QLabel(self.formLayoutWidget_8)
        self.label_15.setObjectName("label_15")
        self.formLayout_8.setWidget(2, QtWidgets.QFormLayout.LabelRole, self.label_15)
        self.F_Cutoff = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_8)
        self.F_Cutoff.setAutoFillBackground(False)
        self.F_Cutoff.setKeyboardTracking(False)
        self.F_Cutoff.setDecimals(2)
        self.F_Cutoff.setMaximum(100.0)
        self.F_Cutoff.setSingleStep(5.0)
        self.F_Cutoff.setProperty("value", 5.0)
        self.F_Cutoff.setObjectName("F_Cutoff")
        self.formLayout_8.setWidget(2, QtWidgets.QFormLayout.FieldRole, self.F_Cutoff)
        self.label_22 = QtWidgets.QLabel(self.formLayoutWidget_8)
        self.label_22.setObjectName("label_22")
        self.formLayout_8.setWidget(3, QtWidgets.QFormLayout.LabelRole, self.label_22)
        self.SampleCycle = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_8)
        self.SampleCycle.setAutoFillBackground(False)
        self.SampleCycle.setKeyboardTracking(False)
        self.SampleCycle.setDecimals(0)
        self.SampleCycle.setMaximum(65535.0)
        self.SampleCycle.setSingleStep(5.0)
        self.SampleCycle.setProperty("value", 20.0)
        self.SampleCycle.setObjectName("SampleCycle")
        self.formLayout_8.setWidget(3, QtWidgets.QFormLayout.FieldRole, self.SampleCycle)
        self.formLayoutWidget_9 = QtWidgets.QWidget(self.groupBox)
        self.formLayoutWidget_9.setGeometry(QtCore.QRect(180, 110, 160, 81))
        self.formLayoutWidget_9.setObjectName("formLayoutWidget_9")
        self.formLayout_9 = QtWidgets.QFormLayout(self.formLayoutWidget_9)
        self.formLayout_9.setContentsMargins(0, 0, 0, 0)
        self.formLayout_9.setObjectName("formLayout_9")
        self.DeadZone = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_9)
        self.DeadZone.setKeyboardTracking(False)
        self.DeadZone.setDecimals(1)
        self.DeadZone.setMaximum(20.0)
        self.DeadZone.setProperty("value", 0.0)
        self.DeadZone.setObjectName("DeadZone")
        self.formLayout_9.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.DeadZone)
        self.label_23 = QtWidgets.QLabel(self.formLayoutWidget_9)
        self.label_23.setObjectName("label_23")
        self.formLayout_9.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.label_23)
        self.PID_Calc_Mode = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_9)
        self.PID_Calc_Mode.setAutoFillBackground(True)
        self.PID_Calc_Mode.setKeyboardTracking(False)
        self.PID_Calc_Mode.setDecimals(0)
        self.PID_Calc_Mode.setMaximum(1.0)
        self.PID_Calc_Mode.setProperty("value", 0.0)
        self.PID_Calc_Mode.setObjectName("PID_Calc_Mode")
        self.formLayout_9.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.PID_Calc_Mode)
        self.label_24 = QtWidgets.QLabel(self.formLayoutWidget_9)
        self.label_24.setObjectName("label_24")
        self.formLayout_9.setWidget(2, QtWidgets.QFormLayout.LabelRole, self.label_24)
        self.PID_Factor = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_9)
        self.PID_Factor.setAutoFillBackground(True)
        self.PID_Factor.setKeyboardTracking(False)
        self.PID_Factor.setDecimals(0)
        self.PID_Factor.setMinimum(1.0)
        self.PID_Factor.setMaximum(4.0)
        self.PID_Factor.setProperty("value", 1.0)
        self.PID_Factor.setObjectName("PID_Factor")
        self.formLayout_9.setWidget(2, QtWidgets.QFormLayout.FieldRole, self.PID_Factor)
        self.label_10 = QtWidgets.QLabel(self.formLayoutWidget_9)
        self.label_10.setObjectName("label_10")
        self.formLayout_9.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label_10)
        self.PIDAutoMode = QtWidgets.QGroupBox(self.groupBox)
        self.PIDAutoMode.setGeometry(QtCore.QRect(420, 110, 91, 101))
        self.PIDAutoMode.setObjectName("PIDAutoMode")
        self.verticalLayoutWidget = QtWidgets.QWidget(self.PIDAutoMode)
        self.verticalLayoutWidget.setGeometry(QtCore.QRect(0, 9, 81, 89))
        self.verticalLayoutWidget.setObjectName("verticalLayoutWidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.verticalLayoutWidget)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setObjectName("verticalLayout")
        self.PID_AutoIncMode1 = QtWidgets.QRadioButton(self.verticalLayoutWidget)
        self.PID_AutoIncMode1.setChecked(True)
        self.PID_AutoIncMode1.setObjectName("PID_AutoIncMode1")
        self.verticalLayout.addWidget(self.PID_AutoIncMode1)
        self.PID_AutoIncMode2 = QtWidgets.QRadioButton(self.verticalLayoutWidget)
        self.PID_AutoIncMode2.setChecked(False)
        self.PID_AutoIncMode2.setObjectName("PID_AutoIncMode2")
        self.verticalLayout.addWidget(self.PID_AutoIncMode2)
        self.PID_ManuInc = QtWidgets.QRadioButton(self.verticalLayoutWidget)
        self.PID_ManuInc.setObjectName("PID_ManuInc")
        self.verticalLayout.addWidget(self.PID_ManuInc)
        self.groupBox_4 = QtWidgets.QGroupBox(Dialog)
        self.groupBox_4.setGeometry(QtCore.QRect(10, 260, 271, 101))
        self.groupBox_4.setObjectName("groupBox_4")
        self.SetRuningParam = QtWidgets.QPushButton(self.groupBox_4)
        self.SetRuningParam.setGeometry(QtCore.QRect(440, 120, 75, 23))
        self.SetRuningParam.setObjectName("SetRuningParam")
        self.formLayoutWidget_4 = QtWidgets.QWidget(self.groupBox_4)
        self.formLayoutWidget_4.setGeometry(QtCore.QRect(10, 20, 160, 61))
        self.formLayoutWidget_4.setObjectName("formLayoutWidget_4")
        self.formLayout_4 = QtWidgets.QFormLayout(self.formLayoutWidget_4)
        self.formLayout_4.setContentsMargins(0, 0, 0, 0)
        self.formLayout_4.setObjectName("formLayout_4")
        self.label_4 = QtWidgets.QLabel(self.formLayoutWidget_4)
        self.label_4.setObjectName("label_4")
        self.formLayout_4.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label_4)
        self.PWM_SET = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_4)
        self.PWM_SET.setKeyboardTracking(False)
        self.PWM_SET.setDecimals(0)
        self.PWM_SET.setMaximum(65535.0)
        self.PWM_SET.setSingleStep(50.0)
        self.PWM_SET.setProperty("value", 50.0)
        self.PWM_SET.setObjectName("PWM_SET")
        self.formLayout_4.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.PWM_SET)
        self.label_5 = QtWidgets.QLabel(self.formLayoutWidget_4)
        self.label_5.setObjectName("label_5")
        self.formLayout_4.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.label_5)
        self.PID_SetPoint = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_4)
        self.PID_SetPoint.setKeyboardTracking(False)
        self.PID_SetPoint.setDecimals(1)
        self.PID_SetPoint.setMaximum(6000.0)
        self.PID_SetPoint.setSingleStep(2000.0)
        self.PID_SetPoint.setProperty("value", 5000.0)
        self.PID_SetPoint.setObjectName("PID_SetPoint")
        self.formLayout_4.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.PID_SetPoint)
        self.groupBox_5 = QtWidgets.QGroupBox(Dialog)
        self.groupBox_5.setGeometry(QtCore.QRect(10, 380, 271, 161))
        self.groupBox_5.setObjectName("groupBox_5")
        self.formLayoutWidget_5 = QtWidgets.QWidget(self.groupBox_5)
        self.formLayoutWidget_5.setGeometry(QtCore.QRect(10, 20, 161, 94))
        self.formLayoutWidget_5.setObjectName("formLayoutWidget_5")
        self.formLayout_5 = QtWidgets.QFormLayout(self.formLayoutWidget_5)
        self.formLayout_5.setFieldGrowthPolicy(QtWidgets.QFormLayout.AllNonFixedFieldsGrow)
        self.formLayout_5.setContentsMargins(0, 0, 0, 0)
        self.formLayout_5.setObjectName("formLayout_5")
        self.label_13 = QtWidgets.QLabel(self.formLayoutWidget_5)
        self.label_13.setObjectName("label_13")
        self.formLayout_5.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label_13)
        self.BackForward_Start = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_5)
        self.BackForward_Start.setKeyboardTracking(False)
        self.BackForward_Start.setDecimals(0)
        self.BackForward_Start.setMaximum(65535.0)
        self.BackForward_Start.setProperty("value", 100.0)
        self.BackForward_Start.setObjectName("BackForward_Start")
        self.formLayout_5.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.BackForward_Start)
        self.label_12 = QtWidgets.QLabel(self.formLayoutWidget_5)
        self.label_12.setObjectName("label_12")
        self.formLayout_5.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.label_12)
        self.BackForward_End = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_5)
        self.BackForward_End.setKeyboardTracking(False)
        self.BackForward_End.setDecimals(0)
        self.BackForward_End.setMaximum(65535.0)
        self.BackForward_End.setProperty("value", 65530.0)
        self.BackForward_End.setObjectName("BackForward_End")
        self.formLayout_5.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.BackForward_End)
        self.label_14 = QtWidgets.QLabel(self.formLayoutWidget_5)
        self.label_14.setObjectName("label_14")
        self.formLayout_5.setWidget(3, QtWidgets.QFormLayout.LabelRole, self.label_14)
        self.BackForward_StepSize = QtWidgets.QDoubleSpinBox(self.formLayoutWidget_5)
        self.BackForward_StepSize.setKeyboardTracking(False)
        self.BackForward_StepSize.setDecimals(0)
        self.BackForward_StepSize.setMinimum(10.0)
        self.BackForward_StepSize.setMaximum(65535.0)
        self.BackForward_StepSize.setSingleStep(500.0)
        self.BackForward_StepSize.setProperty("value", 2000.0)
        self.BackForward_StepSize.setObjectName("BackForward_StepSize")
        self.formLayout_5.setWidget(3, QtWidgets.QFormLayout.FieldRole, self.BackForward_StepSize)
        self.formLayout_2 = QtWidgets.QFormLayout()
        self.formLayout_2.setObjectName("formLayout_2")
        self.formLayout_5.setLayout(2, QtWidgets.QFormLayout.FieldRole, self.formLayout_2)
        self.horizontalLayoutWidget = QtWidgets.QWidget(self.groupBox_5)
        self.horizontalLayoutWidget.setGeometry(QtCore.QRect(10, 110, 239, 31))
        self.horizontalLayoutWidget.setObjectName("horizontalLayoutWidget")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.horizontalLayoutWidget)
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.getVoltageVsPWMCurse = QtWidgets.QPushButton(self.horizontalLayoutWidget)
        self.getVoltageVsPWMCurse.setFocusPolicy(QtCore.Qt.NoFocus)
        self.getVoltageVsPWMCurse.setObjectName("getVoltageVsPWMCurse")
        self.horizontalLayout.addWidget(self.getVoltageVsPWMCurse)
        self.StopVoltageVsPWMCurse = QtWidgets.QPushButton(self.horizontalLayoutWidget)
        self.StopVoltageVsPWMCurse.setFocusPolicy(QtCore.Qt.NoFocus)
        self.StopVoltageVsPWMCurse.setObjectName("StopVoltageVsPWMCurse")
        self.horizontalLayout.addWidget(self.StopVoltageVsPWMCurse)
        self.savePVFDtoMCU = QtWidgets.QPushButton(self.horizontalLayoutWidget)
        self.savePVFDtoMCU.setFocusPolicy(QtCore.Qt.NoFocus)
        self.savePVFDtoMCU.setObjectName("savePVFDtoMCU")
        self.horizontalLayout.addWidget(self.savePVFDtoMCU)

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "高级设置"))
        self.groupBox.setTitle(_translate("Dialog", "PID设置"))
        self.SetPIDParam.setText(_translate("Dialog", "设置"))
        self.label.setText(_translate("Dialog", "KpH"))
        self.label_2.setText(_translate("Dialog", "KiH"))
        self.label_3.setText(_translate("Dialog", "KdH"))
        self.label_16.setText(_translate("Dialog", "KpM"))
        self.label_17.setText(_translate("Dialog", "KiM"))
        self.label_18.setText(_translate("Dialog", "KdM"))
        self.label_19.setText(_translate("Dialog", "KpL"))
        self.label_20.setText(_translate("Dialog", "KiL"))
        self.label_21.setText(_translate("Dialog", "KdL"))
        self.label_6.setText(_translate("Dialog", "ThredHold_L"))
        self.label_8.setText(_translate("Dialog", "ThredHold_PWM"))
        self.label_7.setText(_translate("Dialog", "ThredHold_H"))
        self.label_11.setText(_translate("Dialog", "DelayTime(s)"))
        self.label_9.setText(_translate("Dialog", "ResTime(s)"))
        self.label_15.setText(_translate("Dialog", "F_Cutoff(Hz)"))
        self.label_22.setText(_translate("Dialog", "SampCycle(ms)"))
        self.label_23.setText(_translate("Dialog", "Calc_Mode"))
        self.label_24.setText(_translate("Dialog", "PID_Factor"))
        self.label_10.setText(_translate("Dialog", "DeadZone(mv)"))
        self.PIDAutoMode.setTitle(_translate("Dialog", "PID_MODE"))
        self.PID_AutoIncMode1.setText(_translate("Dialog", "自动增量1"))
        self.PID_AutoIncMode2.setText(_translate("Dialog", "自动增量2"))
        self.PID_ManuInc.setText(_translate("Dialog", "手动增量"))
        self.groupBox_4.setTitle(_translate("Dialog", "调试"))
        self.SetRuningParam.setText(_translate("Dialog", "设置"))
        self.label_4.setText(_translate("Dialog", "PWMSet"))
        self.label_5.setText(_translate("Dialog", "SetPoint(mv)"))
        self.groupBox_5.setTitle(_translate("Dialog", "控制"))
        self.label_13.setText(_translate("Dialog", "Start"))
        self.label_12.setText(_translate("Dialog", "End"))
        self.label_14.setText(_translate("Dialog", "StepSize"))
        self.getVoltageVsPWMCurse.setText(_translate("Dialog", "绘制滞回线"))
        self.StopVoltageVsPWMCurse.setText(_translate("Dialog", "停止"))
        self.savePVFDtoMCU.setText(_translate("Dialog", "保存"))

