# This Python file uses the following encoding: utf-8
import os
from pathlib import Path
import sys

from PySide2.QtWidgets import QApplication, QWidget, QCheckBox
from PySide2.QtCore import QFile
from PySide2.QtUiTools import QUiLoader
from qwt import QwtPlotCurve, QwtPlot
import numpy as np
from comms.controller import WirelessInterface, WirelessController, packet_receive_demux
from comms.messages import Sensor_Data
from threading import Thread
from time import sleep

class Widget(QWidget):
    def __init__(self):
        super(Widget, self).__init__()
        self.xdata=[0]
        self.ydata=[0]
        self.load_ui()
        self.plotter = self.findChildren(QwtPlot, "robotDataPlot")[0]
        self.receive_thread = Thread(target=self.start_plotting, daemon=True)
        self.receive_thread.start()
        self.plot_thread = Thread(target=self.plot, daemon=True)
        self.plot_thread.start()
    
    def plot(self):
        while True:
            sleep(0.01)
            self.plotter.replot()


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
        self.curve1 = QwtPlotCurve.make(self.xdata, self.ydata, plot=self.plotter)
        self.interface = WirelessController(WirelessInterface)
        self.interface.packet_receive_process(self.plot_demux, 0.01)
    
    def plot_demux(self, p):
        try:
            if p.id_ == Sensor_Data.id():
                data = Sensor_Data(p)
                self.xdata.append(self.xdata[-1]+1)
                self.ydata.append(data.leftCount) 
                self.curve1.setData(self.xdata[-100:], self.ydata[-100:])
        except:
            pass

if __name__ == "__main__":
    app = QApplication([])
    widget = Widget()
    widget.show()
    sys.exit(app.exec_())
