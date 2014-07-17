import Gatherer
import PulseFinder
import time
import numpy as np
import serial


# Calling update() will request a copy of the most recently-acquired 
# samples and write them to the text file.
def update():
    global plt, thread
    v = thread.get()
    global txtfl
    txtfl.write(str(v))

def analyze(txtflname, trigger, rise, tail, filedir):
    voltagedata = open(txtflname, 'r')
    voltages = voltagedata.read().split()
    pltr = PulseFinder.Plotter(voltages, trigger, rise, tail, filedir)
    pltr.filterdata()
    pltr.plotsimple()
    pltr.splicedata()
    #pltr.savepulses()
    pltr.plotdatatog()
    #pltr.plotdatasep()
    voltagedata.close()


#PARAMETERS
serialportname = '/dev/tty.usbmodem1421'
txtflname = "voltagedata.txt"
trigger = 20
rise = 5
tail = 5
filedir = "//Users//Joon//OneDrive//Cornell//LEPPSummer2014DarkPhoton//Plots//July17"
sleeptime = 1


# Get handle to serial port
s = serial.Serial(serialportname)
# Create thread to read and buffer serial data.
thread = Gatherer.SerialReader(s, 1024, 500)
thread.start()
#Makes sure that numpy won't truncate the middle of a long array
np.set_printoptions(threshold = 'nan')
#open textfile
txtfl = open(txtflname, 'w')
#wait for data to be written to buffer
time.sleep(sleeptime)
print "sleeptime: %s sec" % sleeptime
#write data to textfile
update()
txtfl.close()
thread.exit()

np.set_printoptions(threshold = 'nan')
for i in range(1):
    thread = Gatherer.SerialReader(s, 1024, 500)
    thread.start()
    txtfl = open(txtflname, 'a')
    txtfl.write(" " + str([-10 for x in range(10)])+ " ")
    time.sleep(sleeptime)
    update()
    txtfl.close()
    thread.exit()

analyze(txtflname, trigger, rise, tail, filedir)