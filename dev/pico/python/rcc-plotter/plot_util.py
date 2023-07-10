from comms import packet
from comms.messages import *


class PlotData:
    def __init__(self, msg_type:):
        self.data = dict()

class Plotter:
    def __init__(self):
        self.