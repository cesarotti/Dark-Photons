#!/usr/bin/env python
"""A text based user interface designed to prompt the user. Aids in using tester.py"""

from datetime import datetime
import os
from os.path import isdir, join
from tester import printfl
from tester import get_data
import tester
import timing

__author__ = "Joon Park"
__maintainer__ = "Joon Park"
__email__ = "JoonPark2017@u.northwestern.edu"


# Replace folder_dir with the directory that contains your run folders
folder_dir = "//Users//Joon//OneDrive//Cornell//DarkPhoton//Crystals"
# Replace serialportname with the USB port on your machine that you're using
# 'dev/something for linux and mac
# COMN where N is a number for windows
serialportname = '/dev/tty.usbmodem1421'
# chunkSize determines how many data points you get per loop. Modifying not recommended
chunkSize = 2**13

def get_data_from_previous_run(filename):
    """function used to read in data from a previously written text file"""
    printfl("txtfl open")
    textfile = open(filename, 'r')
    printfl("reading data...")
    while textfile.readline().strip() != 'Raw Data:':
        pass
    voltages_str = textfile.readline().strip()
    voltages = voltages_str.split()
    voltagelist = []
    printfl("translating data...")
    for voltage in voltages:
        translated = voltage.translate(None, '[],')
        if translated != "" and translated != " ":
            voltagelist.append(float(translated))
    timing.log("data read", timing.log_return())
    return voltagelist

# Asks user if they want to collect data or use previously collected data
while True:
    answer = raw_input("New run? (y/n) ")
    if answer == "y":
        new_crystal = raw_input("New crystal? (y/n) ")
        if new_crystal == "y":
            crystal_label = raw_input("Enter crystal label: ")
            if " " in crystal_label:
                crystal_label = crystal_label.replace(" ", "_")
            break
        elif new_crystal == "n":
            crystal_name_list = []
            for item in os.listdir(folder_dir):
                found = False
                for jtem in crystal_name_list:
                    if item[:-19] == jtem[:-19]:  # Exclude the date/time (only get crystal name)
                        found = True
                if not found:
                    crystal_name_list.append(item)
            for i in range(len(crystal_name_list)):
                printfl("%s: " % (i + 1) + crystal_name_list[i][:-19])
            while True:
                crystal_number = raw_input("Enter crystal number: ")
                try:
                    crystal_number = int(crystal_number)
                    break
                except ValueError:
                    printfl("Invalid input! Try again.")
            crystal_label = crystal_name_list[crystal_number - 1][:-19]
            if " " in crystal_label:
                crystal_label = crystal_label.replace(" ", "_")
            break
    elif answer == "n":
        break
    else:
        printfl("Invalid input! Try again.")

if answer == "y":
    while True:
        save_bool = raw_input(
            "Save raw data? (raw data is required to re-analyze this run in the future, but can lengthen runtime and "
            "create large textfiles for a large number of loops) (y/n) ")
        if save_bool == "y" or save_bool == "n":
            break
        else:
            printfl("Invalid input! Try again.")
    #ask user for how many loops to collect data
    while True:
        loops = raw_input("loops? ")
        try:
            loops = int(loops)
        except ValueError:
            printfl("Invalid input! Try again.")
        else:
            break

else:
    #list all the available previous runs
    printfl("available previous run data:")
    list_of_runs = [f for f in os.listdir(folder_dir) if isdir(join(folder_dir, f))]
    #parse user input
    for i in range(len(list_of_runs)):
        print str(i + 1) + ": " + str(list_of_runs[i])
    while True:
        folder_num = raw_input("input selection #: ")
        try:
            folder_num = int(folder_num)
        except ValueError:
            printfl("Invalid input! Try again.")
        folder_dir_temp = folder_dir + "//" + list_of_runs[folder_num - 1]
        if os.path.exists(folder_dir_temp):
            folder_dir = folder_dir_temp
            break
        else:
            printfl("Invalid input! Try again.")

#ask user to specify rise/tail
while True:
    rise = raw_input("rise? (#/default)")
    try:
        rise = int(rise)
    except ValueError:
        if rise == "default":
            break
        else:
            printfl("Invalid input! Try again.")
    else:
        break
while True:
    tail = raw_input("tail? (#/default)")
    try:
        tail = int(tail)
    except ValueError:
        if tail == "default":
            break
        else:
            printfl("Invalid input! Try again.")
    else:
        break
#ask user for which plots/info they'd like to make/show
while True:
    show_plot = raw_input("make plot? (y/n/simple/all/#) ")
    if show_plot == "y" or show_plot == "n" or show_plot == "simple" or show_plot == "all":
        break
    try:
        show_plot = int(show_plot)
    except ValueError:
        printfl("Invalid input! Try again.")
    else:
        break
while True:
    show_fit = raw_input("make fit? (y/n/all/#) ")
    if show_fit == "y" or show_fit == "n" or show_fit == "all":
        break
    try:
        show_fit = int(show_fit)
    except ValueError:
        printfl("Invalid input! Try again.")
    else:
        break
while True:
    show_avg_noise = raw_input("show average/noise? (both/n/avg/noise) ")
    if show_avg_noise == "both" or show_avg_noise == "n" or show_avg_noise == "avg" or show_avg_noise == "noise":
        while True:
            downsample = raw_input("downsample? (enter 1 for no downsampling) (#) ")
            try:
                downsample = int(downsample)
                break
            except ValueError:
                printfl("Invalid input! Try again.")
        break
    else:
        printfl("Invalid input! Try again.")
while True:
    show_hist = raw_input("make height histogram? (y/n) ")
    if show_hist == "y" or show_hist == "n":
        break
    else:
        printfl("Invalid input! Try again.")
if show_hist == "y":
    while True:
        bins = raw_input("how many bins? ")
        try:
            bins = int(bins)
        except ValueError:
            printfl("Invalid input! Try again.")
        else:
            break
notes = raw_input("notes for this run? ")

#reset timer for more accurate runtime.
timing.reset()

# New run
if answer == "y":
    #use crystal label and current date/time to name the folder 
    #containing the run data
    current = datetime.now()
    month_now = current.month
    date_now = current.month
    year_now = current.year
    hour_now = current.hour
    minute_now = current.minute
    second_now = current.second
    if month_now < 10:
        month_str = "0" + str(month_now)
    else:
        month_str = str(month_now)
    if date_now < 10:
        date_str = "0" + str(date_now)
    else:
        date_str = str(date_now)
    if year_now < 10:
        year_str = "0" + str(year_now)
    else:
        year_str = str(year_now)
    if hour_now < 10:
        hour_str = "0" + str(hour_now)
    else:
        hour_str = str(hour_now)
    if minute_now < 10:
        minute_str = "0" + str(minute_now)
    else:
        minute_str = str(minute_now)
    if second_now < 10:
        second_str = "0" + str(second_now)
    else:
        second_str = str(second_now)
    folder_dir += "//" + str(crystal_label) + "_" + month_str + "_" + date_str + "_" + year_str + "_" + hour_str\
                  + minute_str + "_" + second_str
    os.makedirs(folder_dir)
    txtfl = open(folder_dir + "//data.txt", 'w')
    txtfl.write("crystal: " + crystal_label + "\n")
    txtfl.write("loops: %s\n" % loops)

    printfl("initalizing object...")
    # If specified as default, replace with default values
    if rise == "default":
        rise = 1
    if tail == "default":
        tail = 30
    pltr = tester.Plotter(get_data(serialportname, loops, chunkSize), rise, tail, folder_dir)
    timing.log("object initalized")
    txtfl.write("object initalized at: %s\n" % timing.log_return())

    # If requested, save raw data to text file
    if save_bool == "y":
        pltr.save_data(txtfl)
        # Uncomment this below if you want the raw data written to a separate text file
        # You might want this because for longer loops, raw data can take up a lot of space and make it difficult to
        # open the text file
        """
        raw_data_txtfl = open(folder_dir + "//raw_data.txt", 'w')
        pltr.save_data(raw_data_txtfl)
        raw_data_txtfl.close()
        """

    printfl("calculating trigger...")
    avg = pltr.get_avg(downsample)
    noise = pltr.get_noise(downsample)
    #define trigger value
    trg = avg + noise * 4
    printfl("trigger value: %s" % (trg * (3.3 / 2 ** 13) * 1000))
    txtfl.write("raw trigger value: %s\n" % trg)
    txtfl.write("trigger value: %s\n" % (trg * (3.3 / 2 ** 13) * 1000))

    txtfl.write("average (baseline): %s\n" % (avg * (3.3 / 2 ** 13) * 1000))
    txtfl.write("noise: %s\n" % (noise * (3.3 / 2 ** 13) * 1000))
    txtfl.write("values calculated at: %s\n" % timing.log_return())

    pltr.splice_data(trg, True, txtfl)

    if show_plot == "y":
        pltr.plot()
    elif show_plot == "simple":
        pltr.plot_simp()
    elif show_plot == "all":
        pltr.plot_sep()
    elif show_plot != "n":
        pltr.plot_one(int(show_plot))

    if show_fit == "y":
        pltr.fit(True, True, True, txtfl)
    elif show_fit == "n":
        pltr.fit(True, False, False, txtfl)
    elif show_fit == "all":
        pltr.fit(True, False, False, txtfl)
        pltr.fit_sep()
    else:
        pltr.fit(True, False, False, txtfl)
        pltr.fit_one(int(show_fit))

    if show_avg_noise == "both":
        printfl("Average (baseline): %s" % (avg * (3.3 / 2 ** 13) * 1000))
        printfl("Noise: %s" % (noise * (3.3 / 2 ** 13) * 1000))
    elif show_avg_noise == "avg":
        printfl("Average (baseline): %s" % (avg * (3.3 / 2 ** 13) * 1000))
    elif show_avg_noise == "noise":
        printfl("Noise: %s" % (noise * (3.3 / 2 ** 13) * 1000))

    if show_hist == "y":
        pltr.plot_heights(trg, bins, True, True, True, txtfl)
    else:
        pltr.plot_heights(trg, None, True, False, False, txtfl)
    if notes != "":
        printfl("writing notes...")
        txtfl.write("Notes:\n")
        txtfl.write(notes + "\n")
    txtfl.write("Total runtime: %s" % timing.log_return())
    txtfl.close()

    if not os.listdir(folder_dir):
        try:
            os.rmdir(folder_dir)
        except OSError:
            printfl("directory not empty!")

# Analysis of previous runs
else:
    #get current date and time and use it to name the folder
    #containing the new analysis data
    current = datetime.now()
    hour_now = current.hour
    minute_now = current.minute
    if hour_now < 10:
        hour_str = "0" + str(hour_now)
    else:
        hour_str = str(hour_now)
    if minute_now < 10:
        minute_str = "0" + str(minute_now)
    else:
        minute_str = str(minute_now)
    new_folder_dir = folder_dir + "//%s_%s_%s_" % (
        current.month, current.day, current.year) + hour_str + minute_str + "_%s" % (current.second)
    os.makedirs(new_folder_dir)

    printfl("initalizing object...")
    # If specified as default, replace with default values
    if rise == "default":
        rise = 1
    if tail == "default":
        tail = 30
    pltr = tester.Plotter(get_data_from_previous_run(folder_dir + "//data.txt"), rise, tail, new_folder_dir)
    timing.log("object initalized")

    printfl("calculating trigger...")
    avg = pltr.get_avg(downsample)
    noise = pltr.get_noise(downsample)
    trg = avg + noise * 4
    printfl("Trigger value:" + str(trg * (3.3 / 2 ** 13) * 1000))

    pltr.splice_data(trg)

    if show_plot == "y":
        pltr.plot()
    elif show_plot == "simple":
        pltr.plot_simp()
    elif show_plot == "all":
        pltr.plot_sep()
    elif show_plot != "n":
        pltr.plot_one(int(show_plot))

    if show_fit == "y":
        pltr.fit(True, True, True)
    elif show_fit == "n":
        pltr.fit(True, False, False)
    elif show_fit == "all":
        pltr.fit(True, False, False)
        pltr.fit_sep()
    else:
        pltr.fit(True, False, False)
        pltr.fit_one(int(show_fit))

    if show_avg_noise == "both":
        printfl("Average (baseline): %s" % (avg * (3.3 / 2 ** 13) * 1000))
        printfl("Noise: %s" % (noise * (3.3 / 2 ** 13) * 1000))
    elif show_avg_noise == "avg":
        printfl("Average (baseline): %s" % (avg * (3.3 / 2 ** 13) * 1000))
    elif show_avg_noise == "noise":
        printfl("Noise: %s" % (noise * (3.3 / 2 ** 13) * 1000))

    if show_hist == "y":
        pltr.plot_heights(trg, bins, True, True, True)
    else:
        pltr.plot_heights(trg, None, True, False, False)

    if notes != "":
        notestext = open(new_folder_dir + "//notes.txt", 'w')
        printfl("txtfl opened, writing notes...")
        notestext.write(notes)
        notestext.close()

    if not os.listdir(new_folder_dir):
        try:
            os.rmdir(new_folder_dir)
        except OSError:
            printfl("directory not empty!")
