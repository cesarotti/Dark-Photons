#!/usr/bin/env python
"""
This module contains the Plotter class used to analyze voltage data from the Arduino and some helper functions.

It can be run on its own, but it is highly recommended that you use the run.py module (included in the package)
instead.
"""

import timing
import sys
import serial
import matplotlib.pyplot as plt
import numpy as np
import math
import scipy.optimize as sciopt

__author__ = "Joon Park"
__maintainer__ = "Joon Park"
__email__ = "JoonPark2017@u.northwestern.edu"


def printfl(s):
    """Simple print function that makes sure the text will appear immediately on the console"""
    print s
    sys.stdout.flush()


class Plotter(object):
    def __init__(self, dta, rise=1, tail=30, filedir="//"):
        """
        The Plotter object takes in voltage data allows you to splice pulses, plot data, etc.
        :type dta: list
        :type rise: int
        :type tail: int
        :type filedir: str
        :param dta: voltage data, is a list
        :param rise: how many data points you'd like spliced in before the pulse
        :param tail: how many data points you'd like spliced in after the pulse
        :param filedir: file director in which text and image files will be saved
        """
        self.dta = dta[:8000] + [0] + dta[34000:]
        """
        Intended to skip over bad data from the Arduino. Not sure what is causing this. The 0 is a delimiter.
        """
        self.rise = rise
        self.tail = tail
        self.filedir = filedir
        self.listofdata = []  # Contains spliced pulses after running splice_data method
        self.listofpars = []  # Contains fit parameters after running fit method

    def save_data(self, txtfl=None):
        """
        Saves all data for that run to a text file
        Input textfile for use with run.py UI
        """
        if txtfl == None:
            txtfl = open(self.filedir + "//voltagedata.txt", 'w')
            printfl("txtfl opened, saving...")
            txtfl.write(str(self.dta))
            txtfl.close()
        else:
            printfl("saving to txt...")
            txtfl.write("Raw data:\n")
            txtfl.write(str(self.dta) + "\n")
            txtfl.write("Data saved at: %s\n" % timing.log_return())
        timing.log("data saved", timing.log_return())

    def splice_data(self, trigger, save=False, txtfl=None):
        """
        given a a trigger value, and expected rise/fall times, returns a list of pulses and saves it if specified
        input textfile for use with run.py
        """
        printfl("splicing data...")
        i = 0
        while i < len(self.dta) - 1:
            if self.dta[i] >= trigger and self.dta[i + 1] >= trigger:
                if i - self.rise < 0:
                    a = 0
                else:
                    a = i - self.rise
                if i + self.tail > len(self.dta):
                    b = len(self.dta)
                else:
                    b = i + self.tail
                flag = False
                for j in range(len(self.dta[a:b])):
                    if self.dta[a:b][j] == 0:
                        flag = True
                        if a == 0:
                            i = j + 1
                        else:
                            i += j + self.rise + 1
                if flag == False:
                    self.listofdata.append(self.dta[a:b])
                    i += self.tail
            else:
                i += 1
        if save == True:
            if txtfl == None:
                txtfl = open(self.filedir + "//pulses.txt", 'w')
                printfl("txtfl opened")
                txtfl.write(str(self.listofdata))
                txtfl.close()
            else:
                printfl("saving to txt...")
                txtfl.write("Pulses:\n")
                txtfl.write(str(self.listofdata) + "\n")
                txtfl.write("Pulses saved at: %s\n" % timing.log_return())
        printfl("%s pulses detected" % len(self.listofdata))
        timing.log("data spliced", timing.log_return())

    def plot_simp(self, start=None, end=None):
        """Simple plot of all data collected during that run"""
        printfl("plotting data...")
        if start == None and end == None:
            start = 0
            end = len(self.dta)
        plt.plot([x * (3.3 / 2**13) * 1000 for x in self.dta[start:end]], '.')
        # the x * (3.3 / 2**13) * 1000 converts raw serial data to actual voltage values.
        # Change the last parameter in plot to specify marker style (current set to dots)
        plt.xlabel("microsec")
        plt.ylabel("mV")
        plt.ylim(ymin=230 * (3.3 / 2**13) * 1000)
        if start == None and end == None:
            plt.savefig(self.filedir + "//simpleplot")
        else:
            plt.savefig(self.filedir + "//simpleplot%s" % (start) + \
                        "to" + str(end) + ".png", dpi=500)
        plt.show()
        plt.close()

    def plot(self):
        """Plots all pulses together on one graph, then saves it as plot.png"""
        printfl("plotting data...")
        for item in self.listofdata:
            plt.plot([x * (3.3 / 2**13) * 1000 for x in item])
        plt.xlabel("microsec")
        plt.ylabel("mV")
        plt.title("%s Pulses (%s Rise, %s Tail)" % (len(self.listofdata), self.rise, self.tail))
        plt.savefig(self.filedir + "//plot.png", dpi=500)
        plt.show()
        plt.close()
        timing.log("data plotted", timing.log_return())

    def plot_one(self, ind):
        """Plots one pulse, the saves it as plot#.png, where # is the plot number"""
        plt.plot([x * (3.3 / 2**13) * 1000 for x in self.listofdata[ind]])
        plt.xlabel("microsec")
        plt.ylabel("mV")
        plt.title("Pulse %s (%s Rise, %s Tail)" % ((ind + 1), self.rise, self.tail))
        plt.savefig(self.filedir + "//plot%s.png" % (ind + 1))
        plt.show()
        plt.close()

    def plot_sep(self):
        """
        Plots all pulses separately on individual graphs, then saves them as plotn.png, where n is the plot number
        """
        for i in range(0, len(self.listofdata)):
            plt.plot([x * (3.3 / 2**13) * 1000 for x in self.listofdata[i]])
            plt.xlabel("microsec")
            plt.ylabel("mV")
            plt.title("Pulse %s (%s Rise, %s Tail)" % ((i + 1), self.rise, self.tail))
            savename = "//plot%s.png" % (i + 1)
            plt.savefig(self.filedir + savename)
            plt.close()

    def fit(self, savepars=False, showimg=False, saveimg=False, txtfl=None):
        """
        Fits all pulses to a curve and saves the resulting fit parameters if specified. Saves the plot.
        """
        printfl("fitting data...")

        def powerfunc(t, tau, amplitude, offset, c):
            """The function being used for the fit, identical in all the fitting methods below"""
            return amplitude * ((t - offset) / tau)**2 * math.e**(-(t - offset) / tau + (2 - math.log(4, math.e))) + c

        count = 0
        for i in range(len(self.listofdata)):
            x = np.linspace(0, len(self.listofdata[i]), len(self.listofdata[i]))
            try:
                pars, covariance_matrix = sciopt.curve_fit(powerfunc, x, self.listofdata[i], [1, 100, 0, 284])
                self.listofpars.append(pars.tolist())
                if showimg == True or saveimg == True:
                    x = np.linspace(0, len(self.listofdata[i]), 100)
                    plt.plot(x, [y * (3.3 / 2**13) * 1000 for y in powerfunc(x, *pars)])
            #Sometimes curve_fit can't fit
            except (RuntimeError, TypeError):
                count += 1
        if showimg == True or saveimg == True:
            plt.xlabel("microsec")
            plt.ylabel("mV")
            plt.title("%s Pulses" % (len(self.listofdata) - count))
        if saveimg == True:
            plt.savefig(self.filedir + "//fits.png", dpi=500)
        if showimg == True:
            plt.show()
        if saveimg == True or showimg == True:
            plt.close()
        if savepars == True:
            if txtfl == None:
                txtfl = open(self.filedir + "//fitpars.txt", 'w')
                printfl("txtfl opened")
                txtfl.write(str(self.listofpars))
                txtfl.close()
            else:
                printfl("saving to txt...")
                txtfl.write("Parameters:\n")
                txtfl.write(str(self.listofpars) + "\n")
                txtfl.write("Parameters saved at: %s\n" % timing.log_return())
        timing.log("data fitted", timing.log_return())

    def fit_one(self, ind):
        """
        Fits the specified pulse to a curve and overlays it on top of the data.
        Saves the plot.
        """
        printfl("fitting data...")

        def powerfunc(t, tau, amplitude, offset, c):
            return amplitude * ((t - offset) / tau)**2 * math.e**(-(t - offset) / tau + (2 - math.log(4, math.e))) + c

        x = np.linspace(0, len(self.listofdata[ind]), len(self.listofdata[ind]))
        try:
            try:
                #does the fitting, returns a list of parameters called pars
                pars, covariance_matrix = sciopt.curve_fit(powerfunc, x, self.listofdata[ind, [1, 100, 0, 284]])
                #produce a smooth graph
                x = np.linspace(0, len(self.listofdata[ind]), 100)
                plt.plot(x, [y * (3.3 / 2**13) * 1000 for y in powerfunc(x, *pars)])
                plt.plot([z * (3.3 / 2**13) * 1000 for z in self.listofdata[ind]], 'r.')
                plt.xlabel("microsec")
                plt.ylabel("mV")
                plt.title("Pulse %s" % (ind + 1))
                plt.savefig(self.filedir + "//fit%s.png" % (ind + 1),
                            dpi=500)
                plt.show()
                printfl("parameters: " + str(pars))
                plt.close()
            except TypeError:
                printfl("TypeError on pulse index %s" % ind)
        except RuntimeError:
            printfl("RuntimeError on pulse index %s" % ind)

    def fit_sep(self):
        """Fits each pulse individually"""
        printfl("fitting data...")

        def powerfunc(t, tau, amplitude, offset, c):
            return amplitude * ((t - offset) / tau)**2 * math.e**(-(t - offset) / tau + (2 - math.log(4, math.e))) + c

        typeerrlist = []
        runtimeerrlist = []
        for i in range(len(self.listofdata)):
            x = np.linspace(0, len(self.listofdata[i]), len(self.listofdata[i]))
            try:
                try:
                    #does the fitting, returns a list of parameters called pars
                    pars, covariance_matrix = sciopt.curve_fit(powerfunc, x, self.listofdata[i], [1, 100, 0, 284])
                    #produce a smooth graph
                    x = np.linspace(0, len(self.listofdata[i]), 100)
                    plt.plot(x, [y * (3.3 / 2**13) * 1000 for y in powerfunc(x, *pars)])
                    plt.plot([z * (3.3 / 2**13) * 1000 for z in self.listofdata[i]], 'r.')
                    plt.xlabel("microsec")
                    plt.ylabel("mV")
                    plt.title("Pulse %s" % (i + 1))
                    plt.savefig(self.filedir + "//fit%s.png" % (i + 1),
                                dpi=500)
                    plt.close()
                except TypeError:
                    typeerrlist.append(i)
            except RuntimeError:
                runtimeerrlist.append(i)
        printfl("TypeError on " + str(typeerrlist))
        printfl("RuntimeError on " + str(runtimeerrlist))

    def get_avg(self, downsample=1):
        """
        gets the averages of all values that are not thread delimiters (0), downsampling uses every n samples
        (pulses included in calculation, but has negligible affect on value)
        """
        total = 0
        length = 0
        for num in self.dta[::downsample]:
            if num != 0:
                length += 1
                total += num
        return float(total) / length

    def get_noise(self, downsample=1):
        """
        gets the noise of all values that are not thread delimiters (0), downsampling uses every n samples
        (pulses included in calculation, but has negligible affect on value)
        """
        mean = 0
        variance = 0
        temp = []
        for num1 in self.dta[::downsample]:
            if num1 != 0:
                temp.append(num1)
                mean += num1
        mean = float(mean) / len(temp)
        for num2 in temp:
            variance += (num2 - mean)**2
        return math.sqrt(float(variance) / len(temp))

    def plot_heights(self, trigger, bins=None, savetxt=False, showimg=False, saveimg=False, txtfl=None):
        """Plots max height of pulses in a histogram"""
        if not self.listofpars:
            printfl("run fit() first")
        else:

            def powerfunc(t, tau, A_i, offset, c):
                return A_i * ((t - offset) / tau)**2 * math.e**(-(t - offset) / tau + (2 - math.log(4, math.e))) + c

            listofmax = []
            count = 0
            for num in self.listofpars:
                if not (2 * num[0] + num[2] < 0 and num[2] < 0): # Ensures that the found point is a max not a min
                    max1 = powerfunc(2 * num[0] + num[2], *num)
                    max2 = powerfunc(num[2], *num)
                    if max1 > max2 and max1 > trigger and max1 < 2**13:
                        listofmax.append(max1)
                        count += 1
                    elif max2 > trigger and max2 < 2**13:
                        listofmax.append(max2)
                        count += 1
            if showimg == True or saveimg == True:
                plt.hist([x * (3.3 / 2**13) * 1000 for x in listofmax], bins)
                plt.xlabel("Pulse Height (mV)")
                plt.ylabel("# of Pulses")
                plt.title("%s Pulses, %s Bins" % (count, bins))
            if savetxt == True:
                if txtfl == None:
                    txtfl = open(self.filedir + "//pulseheights.txt", 'w')
                    printfl("opened txtfl")
                    txtfl.write(str(listofmax))
                    txtfl.close()
                else:
                    printfl("saving to txt...")
                    txtfl.write("Max heights:\n")
                    txtfl.write(str(listofmax) + "\n")
                    txtfl.write("Heights saved at: %s\n" % timing.log_return())
            if saveimg == True:
                plt.savefig(self.filedir + "//heighthist.png")
            if showimg == True:
                plt.show()
            if showimg == True or saveimg == True:
                plt.close()
        timing.log("histogram created", timing.log_return())


def get_data(serialportname, loops, chunkSize):
    """returns voltage data from the serial port"""
    # Get handle to serial port
    s = serial.Serial(serialportname)
    printfl("port init")
    voltages = []
    printfl("collecting data...")
    for i in range(loops):
        printfl(i)
        data = s.read(chunkSize * 2)
        data = np.fromstring(data, dtype=np.uint16)
        np.set_printoptions(threshold='nan')
        voltages.append(0)
        voltages += data.tolist()
    timing.log("data collected", timing.log_return())
    return voltages


def get_data_from_text(filedir):
    """returns voltage data from a previously saved text file"""
    printfl("txtfl open")
    txtfl = open(filedir, 'r')
    printfl("reading data...")
    voltages = txtfl.read().split()
    voltagelist = []
    printfl("translating data...")
    for i in range(len(voltages)):
        temp = voltages[i].translate(None, '[],')
        if temp == "":
            pass
        else:
            voltagelist.append(float(temp))
    timing.log("data read", timing.log_return())
    return voltagelist


def analyze_default(voltages, rise, tail, filedir):
    """Used to analyze new data from the Arduino"""
    printfl("initalizing object...")

    pltr = Plotter(voltages, rise, tail, filedir)
    timing.log("object initalized", timing.log_return())

    pltr.save_data()

    printfl("calculating trigger...")
    trg = pltr.get_avg() + pltr.get_noise() * 4
    printfl("Trigger value:" + str(trg * (3.3 / 2**13) * 1000))

    pltr.splice_data(trg)

    # Add and remove methods below as needed per run
    pltr.plot()

    pltr.fit(True, True, True)

    pltr.plot_heights(trg, 100, True, True)


def analyze_secondary(voltages, rise, tail, filedir):
    """Used to perform secondary analysis on already collected data"""
    printfl("initalizing object...")

    pltr = Plotter(voltages, rise, tail, filedir)
    timing.log("object initalized", timing.log_return())

    trg = pltr.get_avg() + pltr.get_noise() * 4
    printfl("Trigger value:" + str(trg * (3.3 / 2**13) * 1000))

    pltr.splice_data(trg)

    # Add and remove methods below as needed per run
    pltr.fit()

    pltr.plot_heights(trg, 100, True, True)


def main():
    """
    This module can be run on its own using this main method, but the recommended method of use is through the module
    run.py (included in package). main method has not been tested after run.py was written.
    """
    # PARAMETERS
    serialportname = '/dev/tty.usbmodem1421'
    chunkSize = 2**13 # One loop collects this many data points
    loops = 30000  # Must result in > 34000 data points
    rise = 1
    tail = 30
    filedir = "//Users//Joon//OneDrive//Cornell//DarkPhoton//Plots//July30"

    """
    Use analyze_default to collect new data from the Arduino. Use analyze_secondary to perform secondary analysis on
    already collected data. Comment out the line that is not in use.
    """
    analyze_default(get_data(serialportname, loops, chunkSize), rise, tail, filedir)
    # analyze_secondary(get_data_from_text(filedir + "//voltagedata.txt"), rise, tail, filedir)


if __name__ == '__main__':
    main()
    # timing.endlog()
