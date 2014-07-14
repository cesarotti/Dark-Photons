import Gatherer
import PulseFinder
import time
import numpy as np
import serial


#PARAMETERS
serialportname = '/dev/tty.usbmodem1421'
txtflname = "voltagedata.txt"
trigger = 20
rise = 5
tail = 5
filedir = "//Users//Joon//OneDrive//Cornell//LEPPSummer2014DarkPhoton//Plots//TEST"


# Get handle to serial port
s = serial.Serial(serialportname)
            
# Create thread to read and buffer serial data.
thread = Gatherer.SerialReader(s)
thread.start()

# Calling update() will request a copy of the most recently-acquired 
# samples and write them to the text file.
def update():
    global plt, thread
    t,v,r = thread.get(1000, downsample = 1)

    #Makes sure that numpy won't truncate the middle of a long array
    np.set_printoptions(threshold = 'nan')

    #Write to txt
    global txtfl
    txtfl.write(str(v))

def analyze(txtflname, trigger, rise, tail, filedir):
    voltagedata = open(txtflname, 'r')
    voltages = voltagedata.read().split()
    pltr = PulseFinder.Plotter(voltages, trigger, rise, tail, filedir)
    pltr.filterdata()
    pltr.splicedata()
    pltr.plotdatatog()
    pltr.plotdatasep()
    voltagedata.close()

txtfl = open(txtflname, 'w')
time.sleep(0.1)
for i in range(1000):
    update()
txtfl.close()
analyze(txtflname, trigger, rise, tail, filedir)