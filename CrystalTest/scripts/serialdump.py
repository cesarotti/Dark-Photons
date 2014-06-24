#! /usr/bin/env python
# simple program to dump data from the serial port 
# to the screen. This uses the serial package in python
# which you might need to install. It is for the SerialUSB
# code

import serial
import time

while (1):
    try:
        ser = serial.Serial('/dev/ttyACM0',115200)
        while 1:
            print ser.readline().strip()
    except :
        print "device went away, sleeping for a bit..."
        time.sleep(3)
