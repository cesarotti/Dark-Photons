import serial
import matplotlib.pyplot as plt
import numpy as np
import math
import scipy.optimize as sciopt


class Plotter(object):
    #datastring: array of strings containing voltage data
    def __init__(self, dta, rise = 1, tail = 1, filedir = "//"):
        self.dta = dta[:8000] + [0 for x in range(10)] + dta[34000:]
        self.rise = rise
        self.tail = tail
        self.filedir = filedir
        self.listofdata = []
        self.listofpars = []
        self.noise = []
        
    #Saves all data for that run to a text file
    def save_data(self):
        txtfl = open(self.filedir + "//voltagedata.txt", 'w')
        txtfl.write(str(self.dta))
        txtfl.close()

    #given a a trigger value, and expected rise/fall times, 
    #returns a list of pulses and saves it if specified
    def splice_data(self, trigger, save = False):
        i = 0
        while i < len(self.dta) - 1:
            pulsedata = []
            #if a pulse is found
            if self.dta[i] >= trigger and self.dta[i + 1] >= trigger:
                #checks to see if there's enough data before the pulse
                #to give a complete rise                
                if self.rise < i:
                    j = self.rise
                    while j != 0:
                        pulsedata.append(self.dta[i-j])
                        j -= 1
                else: #if not, select the data points where data begins
                    j = 0
                    while j != i:
                        pulsedata.append(self.dta[j])
                        j += 1
                #selects the data points that comprise the actual pulse
                k = i
                while k < len(self.dta) and self.dta[k] >= trigger:
                    pulsedata.append(self.dta[k])
                    k += 1
                #add the data points that comprise the tail, or until data ends
                l = 0
                while k < len(self.dta) and l < self.tail:
                    #makes sure another pulse isn't part of the tail
                    if self.dta[k] >= trigger:
                        break
                    else:
                        pulsedata.append(self.dta[k])
                        l += 1
                        k += 1
                #throws out pulses cut off by the loop switching
                found = False
                for num in pulsedata:
                    if num == 0:
                        found = True
                        break
                if found == False:
                    self.listofdata.append(pulsedata)
                    i = k
                else:
                    found = False
                    i += 1
            #if the data does not constitute a pulse and
            #is not a loop delimiter, put it in self.noise
            elif self.dta[i] != 0:
                self.noise.append(self.dta[i])
                i += 1
            else:
                i += 10
        #if specified to save the pulse data, saves to txtfl
        if save == True:
            txtfl = open(self.filedir + "//pulses.txt", 'w')
            txtfl.write(str(self.listofdata))
            txtfl.close()
        print "%s pulses detected" % len(self.listofdata)

    #Simple plot of all data collected during that run
    def plot_simp(self, start = None, end = None):
        if start == None and end == None:
            plt.plot(self.dta)
            plt.ylim(ymin = 270)
            plt.savefig(self.filedir + "//simpleplot.png")
            plt.show()
            plt.close()
        else:
            plt.plot(self.dta[start:end])
            plt.ylim(ymin = 270)
            plt.savefig(self.filedir + "//simpleplot%s" % (start) + "to" + \
                str(end) + ".png", dpi = 500)
            plt.show()
            plt.close()
    
    #Plots all pulses together on one graph,
    #then saves it as plot.png
    def plot(self):
        for item in self.listofdata:
            plt.plot(item)
        plt.savefig(self.filedir + "//plot.png", dpi = 500)
        plt.show()
        plt.close()
    
    def plot_one(self, ind):
        plt.plot(self.listofdata[ind])
        plt.savefig(self.filedir + "//plot%s.png" % (ind + 1))
        plt.show()
        plt.close()
    
    #Plots all pulses separately on individual graphs,
    #then saves them as plotn.png, where n is the plot number
    def plot_sep(self):
        for i in range(0,len(self.listofdata)):
            plt.plot(self.listofdata[i])
            savename = "//plot%s.png" % (i + 1)
            plt.savefig(self.filedir + savename)
            plt.close()
    
    # Fits all pulses to a curve and saves the resulting fit parameters
    # if specified. Saves the plot.
    def fit(self, save = False, show = False):
        def powerfunc(t, tau, A_i, offset, c):
            return A_i * ((t-offset) / tau)**2 * math.e**(-(t-offset) / tau + \
                (2 - math.log(4, math.e))) + c
        for i in range(len(self.listofdata)):
            x = np.linspace(0, len(self.listofdata[i]),
                            len(self.listofdata[i]))
            try:
                pars, covariance_matrix = (sciopt.curve_fit(powerfunc, x,
                                                            self.listofdata[i]))
            #Sometimes curve_fit can't fit
            except (RuntimeError, TypeError):
                pass
            else:
                self.listofpars.append(pars)
                if show == True:
                    x = np.linspace(0, len(self.listofdata[i]), 100)
                    plt.plot(x, powerfunc(x, *pars))
        if show == True:
            plt.savefig(self.filedir + "//fits.png", dpi = 500)
            plt.show()
            plt.close()
        if save == True:
            txtfl = open(self.filedir + "//fitpars.txt", 'w')
            txtfl.write(str(self.listofpars))
            txtfl.close()
    
    # Fits the specified pulse to a curve and overlays it on top of the data.
    # Saves the plot.
    def fit_one(self, ind):
        def powerfunc(t, tau, A_i, offset, c):
            return A_i * ((t-offset) / tau)**2 * math.e**(-(t-offset) / tau + \
                (2 - math.log(4, math.e))) + c
        x = np.linspace(0, len(self.listofdata[ind]),
                        len(self.listofdata[ind]))
        try:
            try:
                #does the fitting, returns a list of parameters called pars
                pars, covariance_matrix = sciopt.curve_fit(powerfunc, x,
                                                           self.listofdata[ind])
                #produce a smooth graph
                x = np.linspace(0, len(self.listofdata[ind]), 100)
                plt.plot(x, powerfunc(x, *pars))
                plt.plot(self.listofdata[ind], 'r.')
                plt.savefig(self.filedir + "//fit%s.png" % (ind + 1),
                            dpi = 500)
                plt.show()
                print "parameters: " + str(pars)
                plt.close()
            except TypeError:
                print "TypeError on pulse index %s" % ind
        except RuntimeError:
            print "RuntimeError on pulse index %s" % ind
    
    def fit_sep(self):
        def powerfunc(t, tau, A_i, offset, c):
            return A_i * ((t-offset) / tau)**2 * math.e**(-(t-offset) / tau + \
                (2 - math.log(4, math.e))) + c
        typeerrlist = []
        runtimeerrlist = []
        for i in range(len(self.listofdata)):
            x = np.linspace(0, len(self.listofdata[i]),
                            len(self.listofdata[i]))
            try:
                try:
                    #does the fitting, returns a list of parameters called pars
                    pars, covariance_matrix = sciopt.curve_fit(powerfunc, x,
                                                               self.listofdata[i])
                    #produce a smooth graph
                    x = np.linspace(0, len(self.listofdata[i]), 100)
                    plt.plot(x, powerfunc(x, *pars))
                    plt.plot(self.listofdata[i], 'r.')
                    plt.savefig(self.filedir + "//fit%s.png" % (i + 1),
                                dpi = 500)
                    plt.show()
                    print "parameters: " + str(pars)
                    plt.close()
                except TypeError:
                    typeerrlist.append(i)
            except RuntimeError:
                runtimeerrlist.append(i)
        print "TypeError on " + str(typeerrlist)
        print "RuntimeError on " + str(runtimeerrlist)

    #Gets the average of all noise that are not thread delimiters (0)
    def get_avg(self):
        total = 0
        for num in self.noise:
            total += num
        return float(total) / len(self.noise)

    #gets the averages of all values that are not thread delimeters (0),
    #pulses included
    def get_avg_trg(self):
        total = 0
        length = 0
        for num in self.dta:
            if num != 0:
                length += 1
                total += num
        return float(total) / length

    #gets the noise of all value that are not thread delimters (0),
    #pulses included
    def get_noise_trg(self):
        mean = 0
        variance = 0
        temp = []
        for num1 in self.dta:
            if num1 != 0:
                temp.append(num1)
                mean += num1
        mean = float(mean) / len(temp)
        for num2 in temp:
            variance += (num2 - mean)**2
        return math.sqrt(variance / len(temp))

    #Gets the noise of the sections with no pulses
    def get_noise(self):
        mean = 0
        variance = 0
        for num in self.noise:
            mean += num
        mean = float(mean) / len(self.noise)
        for num in self.noise:
            variance += (num - mean)**2
        return math.sqrt(variance / len(self.noise))
    
    #Plots max height of pulses in a histogram
    def plot_heights(self, bins = 50, savetxt = False, saveimg = False):
        def powerfunc(t, tau, A_i, offset, c):
            return A_i * ((t-offset) / tau)**2 * math.e**(-(t-offset) / tau + \
                (2 - math.log(4, math.e))) + c
        listofmax = []
        for num in self.listofpars:
            listofmax.append(powerfunc(2 * num[0], *num))
        plt.hist(listofmax, bins)
        plt.show()
        if savetxt == True:
            txtfl = open(self.filedir + "pulseheights.txt", 'w')
            txtfl.write(str(listofmax))
            txtfl.close()
        if saveimg == True:
            plt.savefig(self.filedir + "heighthist.png", dpi = 500)


#returns voltage data from the serial port
def get_data(serialportname, loops, chunkSize):
    # Get handle to serial port
    s = serial.Serial(serialportname)
    voltages = []
    for i in range(loops):
        data = s.read(chunkSize*2)
        data = np.fromstring(data, dtype=np.uint16)
        np.set_printoptions(threshold = 'nan')
        for j in range(10):
            voltages.append(0)
        for num in data:
            voltages.append(num)
    return voltages

#returns voltage data from a previously saved txtfl
def get_data_from_text(filedir):
    txtfl = open(filedir + "//voltagedata.txt", 'r')
    voltages = txtfl.read().split()
    voltagelist = []
    for i in range(len(voltages)):
            temp = voltages[i].translate(None, '[],')
            if temp == "":
                pass
            else:
                voltagelist.append(float(temp))
    return voltagelist

#analysis module
def analyze(voltages, rise, tail, filedir):
    pltr = Plotter(voltages, rise, tail, filedir)
    #pltr.save_data()
    pltr.splice_data(pltr.get_avg_trg() + pltr.get_noise_trg() * 4)
    pltr.plot_heights(saveimg = True)
    
def main():
    #PARAMETERS
    serialportname = '/dev/tty.usbmodem1421'
    chunkSize = 2**13
    loops = 500 #Must result in > 34000 dta pts
    rise = 1
    tail = 30
    filedir = "//Users//Joon//OneDrive//Cornell//DarkPhoton//Plots//July28"
    
    #analyze(get_data(serialportname, loops, chunkSize), rise, tail, filedir)
    analyze(get_data_from_text(filedir), rise, tail, filedir)
    
if __name__ == '__main__':
    main()
