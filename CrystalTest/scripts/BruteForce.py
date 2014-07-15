import serial
import matplotlib.pyplot as plt

#filters a given set of data
def filterdata(dta):
    usefuldata = []
    for i in range(len(dta)):
        if i == len(dta):
            break
        #makes sure the data is a number and is within range
        if dta[i].isdigit() and float(dta[i]) < 4096:
            usefuldata.append(float(dta[i]))
        else:
            i -= 1
    return usefuldata

#given a filtered set of data, a trigger value, and expected rise/fall times, 
#returns a list of pulses
def splicedata(dta, trigger, rise, tail):
    listofdata = []
    i = 0
    while i < len(dta):
        usefuldata = []
        if dta[i] >= trigger: #if a pulse is found
            if rise < i: #checks to see if there's enough data before the pulse to give a complete rise
                j = rise
                while j != 0:
                    usefuldata.append(dta[i-j])
                    j -= 1
            else: #if not, select the data points where data begins
                j = 0
                while j != i:
                    usefuldata.append(dta[j])
                    j += 1
            #selects the data points that comprise the actual plot
            k = i
            while dta[k] >= trigger:
                usefuldata.append(dta[k])
                k += 1
            #add the data points that comprise the tail, or until data ends
            l = 0
            while k < len(dta) and l < tail:
                usefuldata.append(dta[k])
                l += 1
                k += 1
            listofdata.append(usefuldata)
            i = k
        else:
            i += 1
    return listofdata

#Given a list of pulses, plots them all together on one graph,
#then save is as plot.png
def plotdatatog(lod):
    for item in lod:
        plt.plot(item)
    plt.savefig("C:\\Users\\Joon\\OneDrive\\Cornell\\LEPPSummer2014DarkPhoton\\BruteForcePlots\\plot.png")
    plt.close()

#Given a list of pulses, plots them separately on individual graphs,
#then save them as plotn.png, where n is the plot number
def plotdatasep(lod):
    for i in range(0,len(lod)):
        plt.plot(lod[i])
        savename = "plot%s" % (i + 1)
        plt.savefig("C:\\Users\\Joon\\OneDrive\\Cornell\\LEPPSummer2014DarkPhoton\\BruteForcePlots\\" + savename + '.png')
        plt.close()
        
#read from serial port and save it in list "data"
ser = serial.Serial('COM3',115200)
datastring = ""
for i in range(0,2):
    datastring += ser.readline().strip()    
data = datastring.split()
#get a list of pulses
temp = splicedata(filterdata(data), 50, 5, 5)
#plot them together and separately
plotdatatog(temp)
plotdatasep(temp)
