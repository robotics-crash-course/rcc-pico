# This Python file uses the following encoding: utf-8
import os
from pathlib import Path
import sys

from PySide2.QtWidgets import QApplication, QWidget, QCheckBox
from PySide2.QtCore import QFile
from PySide2.QtUiTools import QUiLoader
from qwt import QwtPlotCurve, QwtPlot
import numpy as np


class Widget(QWidget):
    def __init__(self):
        super(Widget, self).__init__()
        self.load_ui()
        self.start_plotting()

    def load_ui(self):
        loader = QUiLoader()
        loader.registerCustomWidget(QwtPlot)
        path = os.fspath(Path(__file__).resolve().parent / "form.ui")
        ui_file = QFile(path)
        ui_file.open(QFile.ReadOnly)
        loader.load(ui_file, self)
        ui_file.close()

    def start_plotting(self):
        self.curve = QwtPlotCurve("Curve 1")
        x = np.linspace(0, 1, 101)
        y = np.linspace(1, 0, 101)
        plotter = self.findChildren(QwtPlot, "robotDataPlot")[0]
        self.curve1 = QwtPlotCurve.make(x, y, plot=plotter)
        plotter.replot()
        print(self)

if __name__ == "__main__":
    app = QApplication([])
    widget = Widget()
    widget.show()
    sys.exit(app.exec_())
