# This Python file uses the following encoding: utf-8
import os
from pathlib import Path
import sys

from PySide2.QtWidgets import QApplication, QWidget, QCheckBox
from PySide2 import QtCore
from PySide2.QtCore import QFile, Signal, Slot, QTimerEvent, QTimer
from PySide2.QtUiTools import QUiLoader
# from qwt import QwtPlotCurve, QwtPlot
from pyqtgraph import PlotWidget, mkPen
import numpy as np
from comms.controller import WirelessInterface, WirelessController, packet_receive_demux
from comms.messages import Sensor_Data
from threading import Thread
from time import sleep
import traceback


class Widget(QWidget):
    msg_sig = Signal(Sensor_Data, QWidget)
    plotter: PlotWidget
    timer: QTimer
    data_line0: PlotWidget
    data_line1: PlotWidget
    i=0

    def __init__(self):
        super(Widget, self).__init__()
        self.xdata=[0]
        self.ydata=[0]
        self.wzdata=[0]
        self.interface = WirelessController(WirelessInterface)
        self.load_ui()
        self.plotter = self.findChildren(PlotWidget, "rccPlotter")[0]
        self.pen0 = mkPen((255,255,0))
        self.pen1 = mkPen((255,0,255))
        self.data_line0 = self.plotter.plot(self.xdata, self.ydata, pen=self.pen0)
        self.data_line1 = self.plotter.plot(self.xdata, self.ydata, pen=self.pen1)
        self.plotter.setYRange(-360, 360, padding=0)
        self.receive_thread = Thread(target=self.start_plotting, daemon=True)
        self.msg_sig.connect(self.add_data)
        self.timer = QTimer()
        self.timer.setTimerType(QtCore.Qt.PreciseTimer)
        self.timer.setInterval(10)
        self.timer.timeout.connect(self.update_plot)
        self.timer.start()

    def load_ui(self):
        self.loader = QUiLoader()
        self.loader.registerCustomWidget(PlotWidget)
        path = os.fspath(Path(__file__).resolve().parent / "form.ui")
        ui_file = QFile(path)
        ui_file.open(QFile.ReadOnly)
        self.loader.load(ui_file, self)
        ui_file.close()

    @Slot(Sensor_Data)
    def add_data(self, data:Sensor_Data):
        # self.xdata = self.xdata[1:]
        # self.ydata = self.ydata[1:]
        self.xdata.append(data.time)
        self.ydata.append(data.theta)
        self.wzdata.append(data.angVelZ)


    def update_plot(self):
        print(f"Plot: {self.i}")
        self.i = self.i + 1
        self.data_line0.setData(self.xdata[-100:], self.ydata[-100:])
        self.data_line1.setData(self.xdata[-100:], self.wzdata[-100:])
        # self.plotter.plot(self.xdata[-100:], self.ydata[-100:])

    def start_plotting(self):
        self.interface.packet_receive_process(self.plot_demux, 0.01)
    
    def plot_demux(self, p):
        try:
            if p.id_ == Sensor_Data.id():
                data = Sensor_Data(p)
                self.msg_sig.emit(data, self)
                # self.i = self.i + 1
                # print(f"Recv: {self.i}")
        except:
            print(traceback.format_exc())
    
    def start_threads(self):
        self.receive_thread.start()
        self.interface.start_inbound()

if __name__ == "__main__":
    QtCore.QCoreApplication.setAttribute(QtCore.Qt.AA_ShareOpenGLContexts)  
    app = QApplication([])
    widget = Widget()
    widget.start_threads()
    widget.show()
    sys.exit(app.exec_())
