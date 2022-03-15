# IMPORT PYQT5 CLASSES:
import os
from re import L
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QFileDialog, QMessageBox
from defer import return_value

# IMPORTS FUNCTIONS FROM FILES:
from Generate import ComStructure


# GUI CLASS:
class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        # VARIABLES:
        self.Out_Skeleton = False
        self.Path = ""
        self.Filenames = ["machines", "datatype", "service_interfaces", "software_compoenents", "executables", "deployment", "processes", "mappings"]
        self.File_Num = 0
        self.rtn = []


        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1540, 900)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        # Browse Button
        self.browse_btn = QtWidgets.QPushButton(self.centralwidget)
        self.browse_btn.setGeometry(QtCore.QRect(1170, 10, 151, 51))
        font = QtGui.QFont()
        font.setFamily("Times New Roman")
        font.setPointSize(12)
        self.browse_btn.setFont(font)
        self.browse_btn.setObjectName("browse_btn")

        # Input Scroll Area
        self.input = QtWidgets.QScrollArea(self.centralwidget)
        self.input.setGeometry(QtCore.QRect(60, 100, 601, 701))
        self.input.setWidgetResizable(True)
        self.input.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignTop)
        self.input.setObjectName("input")
        self.input.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.scrollAreaWidgetContents = QtWidgets.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(20, 0, 629, 699))
        self.scrollAreaWidgetContents.setObjectName("scrollAreaWidgetContents")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.scrollAreaWidgetContents)
        self.verticalLayout.setObjectName("verticalLayout")

        # Input Text Edit
        self.input_tedt = QtWidgets.QTextEdit(self.scrollAreaWidgetContents)
        self.input_tedt.setGeometry(QtCore.QRect(30, 10, 611, 681))
        font.setPointSize(10)
        self.input_tedt.setFont(font)
        self.input_tedt.setText("")
        self.input_tedt.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignTop)
        self.input_tedt.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.input_tedt.setObjectName("input_tedt")
        self.verticalLayout.addWidget(self.input_tedt)
        self.input.setWidget(self.scrollAreaWidgetContents)
        self.input_tedt.setText("")

        # Output Scroll Area
        self.output = QtWidgets.QScrollArea(self.centralwidget)
        self.output.setGeometry(QtCore.QRect(690, 100, 631, 701))
        self.output.setWidgetResizable(True)
        self.output.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignTop)
        self.output.setObjectName("output")
        self.output.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.scrollAreaWidgetContents_2 = QtWidgets.QWidget()
        self.scrollAreaWidgetContents_2.setGeometry(QtCore.QRect(0, 0, 629, 699))
        self.scrollAreaWidgetContents_2.setObjectName("scrollAreaWidgetContents_2")
        self.verticalLayout_2 = QtWidgets.QVBoxLayout(self.scrollAreaWidgetContents_2)
        self.verticalLayout_2.setObjectName("verticalLayout_2")

        # Output Label
        self.output_lbl = QtWidgets.QLabel(self.scrollAreaWidgetContents_2)
        self.output_lbl.setGeometry(QtCore.QRect(10, 10, 611, 681))
        font.setPointSize(10)
        self.output_lbl.setFont(font)
        self.output_lbl.setText("")
        self.output_lbl.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignTop)
        self.output_lbl.setStyleSheet("background-color: rgb(255, 255, 255);")
        self.output_lbl.setObjectName("output_lbl")
        self.output_lbl.setTextInteractionFlags(QtCore.Qt.TextSelectableByMouse)
        self.verticalLayout_2.addWidget(self.output_lbl)
        self.output.setWidget(self.scrollAreaWidgetContents_2)

        # Browse Line Edit
        self.lineEdit = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit.setGeometry(QtCore.QRect(30, 10, 1131, 51))
        font.setPointSize(14)
        self.lineEdit.setFont(font)
        self.lineEdit.setText("")
        self.lineEdit.setObjectName("lineEdit")

        # Scan Button
        self.Scan_btn = QtWidgets.QPushButton(self.centralwidget)
        self.Scan_btn.setGeometry(QtCore.QRect(1340, 100, 161, 61))
        font.setPointSize(12)
        self.Scan_btn.setFont(font)
        self.Scan_btn.setObjectName("Scan_btn")

        # Generate Skeleton Button
        self.SklGn_btn = QtWidgets.QPushButton(self.centralwidget)
        self.SklGn_btn.setGeometry(QtCore.QRect(1340, 175, 161, 61))
        font.setPointSize(12)
        self.SklGn_btn.setFont(font)
        self.SklGn_btn.setObjectName("SklGn_btn")
        self.SklGn_btn.setEnabled(False)

        # Generate Proxy Button
        self.PrxGn_btn = QtWidgets.QPushButton(self.centralwidget)
        self.PrxGn_btn.setGeometry(QtCore.QRect(1340, 250, 161, 61))
        font.setPointSize(12)
        self.PrxGn_btn.setFont(font)
        self.PrxGn_btn.setObjectName("PrxGn_btn")
        self.PrxGn_btn.setEnabled(False)

        # Generate Manifest
        self.MnFGn_btn = QtWidgets.QPushButton(self.centralwidget)
        self.MnFGn_btn.setGeometry(QtCore.QRect(1340, 325, 161, 61))
        font.setPointSize(12)
        self.MnFGn_btn.setFont(font)
        self.MnFGn_btn.setObjectName("MnFGn_btn")
        self.MnFGn_btn.setEnabled(False)

        # Label Pointing to Input
        self.InputPointing_lbl = QtWidgets.QLabel(self.centralwidget)
        self.InputPointing_lbl.setGeometry(QtCore.QRect(10, 810, 621, 41))
        font.setPointSize(18)
        self.InputPointing_lbl.setFont(font)
        self.InputPointing_lbl.setAlignment(QtCore.Qt.AlignCenter)
        self.InputPointing_lbl.setObjectName("InputPointing_lbl")

        # Label Pointing to Output
        self.OutputPointing_lbl = QtWidgets.QLabel(self.centralwidget)
        self.OutputPointing_lbl.setGeometry(QtCore.QRect(680, 810, 631, 41))
        font.setPointSize(18)
        self.OutputPointing_lbl.setFont(font)
        self.OutputPointing_lbl.setAlignment(QtCore.Qt.AlignCenter)
        self.OutputPointing_lbl.setObjectName("OutputPointing_lbl")

        # Save Button
        self.Save_btn = QtWidgets.QPushButton(self.centralwidget)
        self.Save_btn.setGeometry(QtCore.QRect(1340, 740, 161, 61))
        font.setPointSize(12)
        self.Save_btn.setFont(font)
        self.Save_btn.setObjectName("Save_btn")
        self.Save_btn.setEnabled(False)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1549, 26))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "ARA-COM Generator"))
        self.InputPointing_lbl.setText(_translate("MainWindow", "Input"))
        self.OutputPointing_lbl.setText(_translate("MainWindow", "Output"))
        self.browse_btn.setText(_translate("MainWindow", "Browse"))
        self.Scan_btn.setText(_translate("MainWindow", "Scan"))
        self.SklGn_btn.setText(_translate("MainWindow", "Generate Skeletons"))
        self.PrxGn_btn.setText(_translate("MainWindow", "Generate Proxies"))
        self.MnFGn_btn.setText(_translate("MainWindow", "Generate Manifests"))
        self.Save_btn.setText(_translate("MainWindow", "Save"))
        self.browse_btn.clicked.connect(self.browse_handler)
        self.Scan_btn.clicked.connect(self.scan_handler)
        self.Save_btn.clicked.connect(self.save_handler)
        self.SklGn_btn.clicked.connect(self.SklGn_handler)
        self.PrxGn_btn.clicked.connect(self.PrxGn_handler)

    def browse_handler(self):
        filename = QFileDialog.getOpenFileName(
            caption = f'Select the {self.Filenames[self.File_Num]}.arxml file',
            directory = os.getcwd(),
            filter = 'XML Files (*.xml *.arxml)'
        )
        path = filename[0]
        ListPath = path.split("/")
        self.Path = ""
        for n in range(len(ListPath)-1):
            self.Path += ListPath[n] + "/"
        self.output_lbl.setText("")
        self.lineEdit.setText(path)
        return path


    def scan_handler(self):
        self.ComStruct = ComStructure()
        for self.File_Num in range(len(self.Filenames)):
            path = self.Path + self.Filenames[self.File_Num] + ".arxml"
            self.output_lbl.setText("")
            try:
                f = open(path, "r", encoding='utf-8')
            except OSError:
                path = self.Path + "/" + self.Filenames[self.File_Num] + ".xml"
                try:
                    f = open(path, "r", encoding='utf-8')
                except OSError:
                    path = self.browse_handler()
                    f = open(path, "r", encoding='utf-8')
            with f:
                Input_String = f.read()
                self.input_tedt.setText(Input_String)
                self.lineEdit.setText(path)
            self.ComStruct.Scan(self.input_tedt.toPlainText(), self.Filenames[self.File_Num])

        self.SklGn_btn.setEnabled(True)
        self.PrxGn_btn.setEnabled(True)
        self.MnFGn_btn.setEnabled(True)
        self.Save_btn.setEnabled(False)

    def SklGn_handler(self):
        self.rtn = self.ComStruct.GenerateSkeleton()
        Out_String = ""
        for SI in self.rtn:
            Out_String = Out_String + SI[0] + "Skeleton.hpp\n"
            Out_String = Out_String + SI[1] + "\n\n"
        self.output_lbl.setText(Out_String)
        self.Save_btn.setEnabled(True)
        self.Out_Skeleton = True

    def PrxGn_handler(self):
        self.rtn = self.ComStruct.GenerateProxy()
        Out_String = ""
        for SI in self.rtn:
            Out_String = Out_String + SI[0] + "Proxy.hpp\n"
            Out_String = Out_String + SI[1] + "\n\n"
        self.output_lbl.setText(Out_String)
        self.Save_btn.setEnabled(True)
        self.Out_Skeleton = False
        

    def save_handler(self):
        for SI in self.rtn:
            if self.Out_Skeleton:
                suffix = 'Skeleton'
            else:
                suffix = "Proxy"

            filename = QFileDialog.getSaveFileName(
                caption = f'Save {SI[0]}{suffix}.hpp',
                directory= f'{SI[0]}{suffix}.hpp',
            )
            path = filename[0]
            with open(path, "w", encoding='utf-8') as f:
                f.write(SI[1])

            # Message Box
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Information)
            msg.setText("Saved")
            msg.setText(f"{SI[0]}{suffix}.hpp is Saved in '{path}'")
            msg.setWindowTitle("Information")
            msg.setStandardButtons(QMessageBox.Ok)
            msg.exec_()



# MAIN:
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
