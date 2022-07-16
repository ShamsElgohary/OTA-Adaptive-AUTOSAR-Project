# IMPORT PYQT5 CLASSES:
import os
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QFileDialog

# IMPORTS FUNCTIONS FROM FILES:
from Generate import Generator


# GUI CLASS:
class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        # VARIABLES:
        self.Out_Skeleton = False
        self.Path = ""
        self.Filenames = ["machines", "datatype", "service_interfaces", "deployment", "mappings", "software_compoenents"]
        self.File_Num = 0
        self.rtn = []


        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(100, 200)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        self.VerticalLayout = QtWidgets.QVBoxLayout(self.centralwidget)
        self.VerticalLayout.setContentsMargins(20,20,20,20)
        self.VerticalLayout.addSpacing(20)

        self.UpperHorizontalLayout = QtWidgets.QHBoxLayout(self.centralwidget)
        self.VerticalLayout.setContentsMargins(20,20,20,20)
        self.VerticalLayout.addLayout(self.UpperHorizontalLayout)

        self.LowerHorizontalLayout = QtWidgets.QHBoxLayout(self.centralwidget)
        self.VerticalLayout.setContentsMargins(20,20,20,20)
        self.VerticalLayout.addLayout(self.LowerHorizontalLayout)
        
        self.centralwidget.setLayout(self.VerticalLayout)

        # Browse Button
        self.browse_btn = QtWidgets.QPushButton()
        self.UpperHorizontalLayout.addWidget(self.browse_btn)
        font = QtGui.QFont()
        font.setFamily("Times New Roman")
        font.setPointSize(16)
        self.browse_btn.setFont(font)
        self.browse_btn.setObjectName("browse_btn")

        # Browse Line Edit
        self.lineEdit = QtWidgets.QLineEdit()
        self.UpperHorizontalLayout.addWidget(self.lineEdit)
        font.setPointSize(16)
        self.lineEdit.setFont(font)
        self.lineEdit.setText("")
        self.lineEdit.setObjectName("lineEdit")

        # Scan Button
        self.Scan_btn = QtWidgets.QPushButton()
        self.LowerHorizontalLayout.addWidget(self.Scan_btn)
        font.setPointSize(16)
        self.Scan_btn.setFont(font)
        self.Scan_btn.setObjectName("Scan_btn")

        # Generate Skeleton Button
        self.SklGn_btn = QtWidgets.QPushButton()
        self.LowerHorizontalLayout.addWidget(self.SklGn_btn)
        font.setPointSize(16)
        self.SklGn_btn.setFont(font)
        self.SklGn_btn.setObjectName("SklGn_btn")
        self.SklGn_btn.setEnabled(False)

        # Generate Proxy Button
        self.PrxGn_btn = QtWidgets.QPushButton()
        self.LowerHorizontalLayout.addWidget(self.PrxGn_btn)
        font.setPointSize(16)
        self.PrxGn_btn.setFont(font)
        self.PrxGn_btn.setObjectName("PrxGn_btn")
        self.PrxGn_btn.setEnabled(False)

        # Generate Manifest
        self.MnFGn_btn = QtWidgets.QPushButton()
        self.LowerHorizontalLayout.addWidget(self.MnFGn_btn)
        font.setPointSize(16)
        self.MnFGn_btn.setFont(font)
        self.MnFGn_btn.setObjectName("MnFGn_btn")
        self.MnFGn_btn.setEnabled(False)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
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
        self.browse_btn.setText(_translate("MainWindow", "Browse"))
        self.Scan_btn.setText(_translate("MainWindow", "Scan"))
        self.SklGn_btn.setText(_translate("MainWindow", "Generate Skeletons"))
        self.PrxGn_btn.setText(_translate("MainWindow", "Generate Proxies"))
        self.MnFGn_btn.setText(_translate("MainWindow", "Generate Manifests"))
        self.browse_btn.clicked.connect(self.browse_handler)
        self.Scan_btn.clicked.connect(self.scan_handler)
        self.SklGn_btn.clicked.connect(self.SklGn_handler)
        self.PrxGn_btn.clicked.connect(self.PrxGn_handler)
        self.MnFGn_btn.clicked.connect(self.MnFGn_handler)

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
        self.lineEdit.setText(path)
        return path


    def scan_handler(self):
        self.Gen = Generator()
        Input_String = ""
        for self.File_Num in range(len(self.Filenames)):
            path = self.Path + self.Filenames[self.File_Num] + ".arxml"
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
                self.lineEdit.setText(path)
            self.Gen.Scan(Input_String, self.Filenames[self.File_Num])

        self.SklGn_btn.setEnabled(True)
        self.PrxGn_btn.setEnabled(True)
        self.MnFGn_btn.setEnabled(True)

    def SklGn_handler(self):
        self.rtn = self.Gen.GenerateSkeleton()
        self.Out_Skeleton = True

    def PrxGn_handler(self):
        self.rtn = self.Gen.GenerateProxy()
        self.Out_Proxy = False

    def MnFGn_handler(self):
        self.rtn = self.Gen.GenerateManifest()
        self.Out_Manifest = False




# MAIN:
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
