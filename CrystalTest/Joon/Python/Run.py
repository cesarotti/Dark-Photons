import Gatherer
import PulseFinder
import time
import numpy as np
import serial


# Calling update() will request a copy of the most recently-acquired 
# samples and write them to the text file.
def update():
    global plt, thread
    v = thread.get(1024, downsample = 1)

    #Write to txt
    global txtfl
    txtfl.write(str(v))

def analyze(txtflname, trigger, rise, tail, filedir):
    voltagedata = open(txtflname, 'r')
    voltages = voltagedata.read().split()
    pltr = PulseFinder.Plotter(voltages, trigger, rise, tail, filedir)
    pltr.filterdata()
    pltr.plotsimple()
    """
    pltr.splicedata()
    pltr.plotdatatog()
    pltr.plotdatasep()
    """
    voltagedata.close()


#PARAMETERS
serialportname = '/dev/tty.usbmodem1421'
txtflname = "voltagedata.txt"
trigger = 20
rise = 5
tail = 5
filedir = "//Users//Joon//OneDrive//Cornell//LEPPSummer2014DarkPhoton//Plots//TEMP7"

# Get handle to serial port
s = serial.Serial(serialportname)
            
# Create thread to read and buffer serial data.
thread = Gatherer.SerialReader(s)
thread.start()

#Makes sure that numpy won't truncate the middle of a long array
np.set_printoptions(threshold = 'nan')

txtfl = open(txtflname, 'w')
time.sleep(0.1)

for i in range(1000):
    update()

txtfl.close()

#analyze(txtflname, trigger, rise, tail, filedir)