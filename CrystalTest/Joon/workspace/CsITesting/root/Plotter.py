import matplotlib.pyplot as plt

class Plotter(object):
    #dta: list of data to plot
    def __init__(self, dta, trigger, rise, tail):
        self.dta = dta
        self.self.trigger = trigger
        self.self.rise = rise
        self.self.tail = tail
        self.listofdata = []
    
    #filters the data
    def filterdata(self):
        usefuldata = []
        for i in range(len(self.dta)):
            if i == len(self.dta):
                break
            #makes sure the data is a number and is within range
            if self.dta[i].isdigit() and float(self.dta[i]) < 4096:
                usefuldata.append(float(self.dta[i]))
            else:
                i -= 1
        self.dta = usefuldata

    #given a a trigger value, and expected rise/fall times, 
    #returns a list of pulses
    def splicedata(self):
        i = 0
        while i < len(self.dta):
            usefuldata = []
            if self.dta[i] >= self.trigger: #if a pulse is found
                if self.rise < i: #checks to see if there's enough data before the pulse to give a complete rise
                    j = self.rise
                    while j != 0:
                        usefuldata.append(self.dta[i-j])
                        j -= 1
                else: #if not, select the data points where data begins
                    j = 0
                    while j != i:
                        usefuldata.append(self.dta[j])
                        j += 1
                #selects the data points that comprise the actual plot
                k = i
                while self.dta[k] >= self.trigger:
                    usefuldata.append(self.dta[k])
                    k += 1
                #add the data points that comprise the tail, or until data ends
                l = 0
                while k < len(self.dta) and l < self.tail:
                    usefuldata.append(self.dta[k])
                    l += 1
                    k += 1
                self.listofdata.append(usefuldata)
                i = k
            else:
                i += 1
    
    #Plots all pulses together on one graph,
    #then saves it as plot.png
    def plotdatatog(self):
        for item in self.listofdata:
            plt.plot(item)
        plt.savefig("C:\\Users\\Joon\\OneDrive\\Cornell\\LEPPSummer2014DarkPhoton\\Plots\\plot.png")
        plt.close()

    #Plots all pulses separately on individual graphs,
    #then saves them as plotn.png, where n is the plot number
    def plotdatasep(self):
        for i in range(0,len(self.listofdata)):
            plt.plot(self.listofdata[i])
            savename = "plot%s" % (i + 1)
            plt.savefig("C:\\Users\\Joon\\OneDrive\\Cornell\\LEPPSummer2014DarkPhoton\\Plots\\" + savename + '.png')
            plt.close()