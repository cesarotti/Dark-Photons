#!/usr/bin/env python
"""This module prints the noise from all the runs in the folder"""

import os

__author__ = "Joon Park"
__maintainer__ = "Joon Park"
__email__ = "JoonPark2017@u.northwestern.edu"


# Replace this with the directory that contains your run folders
folder_dir = "//Users//Joon//OneDrive//Cornell//DarkPhoton//Crystals"

noise_list = []
# Sorts through all the run folders and gets the noise
# Can be modified to get whatever value you want (average, trigger, fit parameters, etc)
for run in os.listdir(folder_dir):
    if run == ".DS_Store":
        pass
    elif os.path.isdir(folder_dir + "//" + run):
        txtfl = open(folder_dir + "//" + run + "//data.txt", 'r')
        found = False
        while not found:
            templine = txtfl.readline()
            # modify line below if you want different values
            if "Noise" in templine or "noise" in templine:
                found = True
                noise_list.append((run, templine))
            elif "" == templine:
                break

new_list = []
# Comment this block out and replace with new_list = noise_list if you want every unique run
# This block gives you the value from the latest run per crystal
"""
for i in range(len(noise_list)):
    found = False
    for j in range(len(new_list)):
        if new_list[j][0][:-19] == noise_list[i][0][:-19]:
            found = True
            new_list[j] = noise_list[i]
    if not found:
        new_list.append(noise_list[i])
"""
new_list = noise_list

for item in new_list:
    print item[0] + " - " + item[1][:-2]
