import threading
import serial
import matplotlib.pyplot as plt
import numpy as np


class SerialReader(threading.Thread):
    # Defines a thread for writing serial data to text file
    def __init__(self, port, chunkSize=1024, loops = 1):
        threading.Thread.__init__(self)
        self.chunkSize = chunkSize  # size of a single chunk (items, not bytes)
        self.port = port            # serial port handle
        self.loops = loops
    
    def run(self):
        port = self.port
        
        txtfl = open("voltagedata.txt", 'w')
      
        for i in range(self.loops):
            data = port.read(self.chunkSize*2)
            data = np.fromstring(data, dtype=np.uint16)
            np.set_printoptions(threshold = 'nan')
            txtfl.write(" " + str([-10 for x in range(10)])+ " ")
            txtfl.write(str(data))
        
        txtfl.close()


class Plotter(object):
    #datastring: array of strings containing voltage data
    def __init__(self, datastring, trigger = 1, rise = 1, tail = 1, filedir = "//"):
        self.datastring = datastring
        self.trigger = trigger
        self.rise = rise
        self.tail = tail
        self.filedir = filedir
        self.dta = []
        self.listofdata = []
    
    #filters the data
    def filterdata(self):
        for i in range(len(self.datastring)):
            temp = self.datastring[i].translate(None, '[],')
            if temp == "":
                pass
            else:
                self.dta.append(float(temp))

    #given a a trigger value, and expected rise/fall times, 
    #returns a list of pulses
    def splicedata(self):
        i = 0
        while i < len(self.dta):
            pulsedata = []
            if self.dta[i] >= self.trigger: #if a pulse is found
                if self.rise < i: #checks to see if there's enough data before the pulse to give a complete rise
                    j = self.rise
                    while j != 0:
                        pulsedata.append(self.dta[i-j])
                        j -= 1
                else: #if not, select the data points where data begins
                    j = 0
                    while j != i:
                        pulsedata.append(self.dta[j])
                        j += 1
                #selects the data points that comprise the actual plot
                k = i
                while k < len(self.dta) and self.dta[k] >= self.trigger:
                    pulsedata.append(self.dta[k])
                    k += 1
                #add the data points that comprise the tail, or until data ends
                l = 0
                while k < len(self.dta) and l < self.tail:
                    pulsedata.append(self.dta[k])
                    l += 1
                    k += 1
                self.listofdata.append(pulsedata)
                i = k
            else:
                i += 1
        #throws out pulses cut off by the thread switching
        newlistofdata = []
        for item in self.listofdata:
            found = False
            for num in item:
                if num == -10:
                    found = True
                    break
            if found == False:
                newlistofdata.append(item)
            else:
                found = False
        self.listofdata = newlistofdata
        
    
    #Simple plot
    def plotsimple(self, start = -1, end = -1):
        if start == -1 and end == -1:
            plt.plot(self.dta)
            plt.savefig(self.filedir + "//simpleplot.png", dpi = 500)
            plt.show()
            plt.close()
        else:
            plt.plot(self.dta[start:end])
            plt.savefig(self.filedir + "//simpleplot%s" % (start) + "to" + str(end) + ".png", dpi = 500)
            plt.show()
            plt.close()
    
    #Plots all pulses together on one graph,
    #then saves it as plot.png
    def plottog(self):
        for item in self.listofdata:
            plt.plot(item)
        plt.savefig(self.filedir + "//plot.png", dpi = 750)
        plt.close()

    #Plots all pulses separately on individual graphs,
    #then saves them as plotn.png, where n is the plot number
    def plotsep(self):
        for i in range(0,len(self.listofdata)):
            plt.plot(self.listofdata[i])
            savename = "//plot%s.png" % (i + 1)
            plt.savefig(self.filedir + savename)
            plt.close()
            
    #Saves pulses to textfile
    def savepulses(self):
        txtfl = open(self.filedir + "//pulses.txt", 'w')
        txtfl.write(str(self.listofdata))
        txtfl.close()


def analyze(txtflname, trigger, rise, tail, filedir):
    voltagedata = open(txtflname, 'r')
    voltages = voltagedata.read().split()
    pltr = Plotter(voltages, trigger, rise, tail, filedir)
    pltr.filterdata()
    #pltr.plotsimple()
    pltr.splicedata()
    pltr.plottog()
    voltagedata.close()

def main():
    #PARAMETERS
    serialportname = '/dev/tty.usbmodem1421'
    txtflname = "voltagedata.txt"
    trigger = 20
    rise = 5
    tail = 5
    filedir = "//Users//Joon//OneDrive//Cornell//DarkPhoton//Plots//July20"
    """
    # Get handle to serial port
    s = serial.Serial(serialportname)
    # Create thread to read serial data.
    thread = SerialReader(s, 2**13, 10)
    thread.start()
    thread.join()
    """
    analyze(txtflname, trigger, rise, tail, filedir)
    
if __name__ == '__main__':
    main()