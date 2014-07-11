import matplotlib.pyplot as plt
import serial

class Plotter(object):
    #dta: list of data to plot
    def __init__(self, datastring, trigger = 1, rise = 1, tail = 1, filedir = "//"):
        self.datastring = datastring
        self.trigger = trigger
        self.rise = rise
        self.tail = tail
        self.filedir = filedir
        self.dta = self.datastring.split()
        self.listofdata = []
    '''
    #getters
    def getdatastring(self):
        return self.datastring
    
    def getdta(self):
        return self.dta
    
    def getlistofdata(self):
        return self.listofdata'''
    
    #filters the data
    def filterdata(self):
        usefuldata = []
        for i in range(len(self.dta)):
            #makes sure the data is a number and is within range
            if self.dta[i].isdigit() and int(self.dta[i]) < 4096:
                usefuldata.append(int(self.dta[i]))
        self.dta = usefuldata

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
    
    #Plots all pulses together on one graph,
    #then saves it as plot.png
    def plotdatatog(self):
        for item in self.listofdata:
            plt.plot(item)
        plt.savefig(self.filedir + "//plot.png")
        plt.close()

    #Plots all pulses separately on individual graphs,
    #then saves them as plotn.png, where n is the plot number
    def plotdatasep(self):
        for i in range(0,len(self.listofdata)):
            plt.plot(self.listofdata[i])
            savename = "//plot%s.png" % (i + 1)
            plt.savefig(self.filedir + savename)
            plt.close()

def main():
    ser = serial.Serial('//dev//tty.usbmodem1411',115200)

    dta = ""
    for i in range(3):
        dta += ser.readline().strip()

    pltr = Plotter(dta, 0, filedir = "//Users//Joon//Desktop//TEMP")

if __name__ == '__main__':
    main()