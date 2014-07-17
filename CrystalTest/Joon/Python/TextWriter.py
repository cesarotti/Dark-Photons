import pyqtgraph as pg
import time, threading, sys
import serial
import numpy as np

class SerialReader(threading.Thread):
    """ Defines a thread for reading and buffering serial data.
    By default, about 5MSamples are stored in the buffer.
    Data can be retrieved from the buffer by calling get(N)"""
    def __init__(self, port, chunkSize=1024, chunks=5000):
        threading.Thread.__init__(self)
        # circular buffer for storing serial data until it is 
        # fetched by the GUI
        self.buffer = np.zeros(chunks*chunkSize, dtype=np.uint16) #uint16: unsigned int (0 to 65535)
        
        self.chunks = chunks        # number of chunks to store in the buffer
        self.chunkSize = chunkSize  # size of a single chunk (items, not bytes)
        self.ptr = 0                # pointer to (most recently collected buffer index) + 1
        self.port = port            # serial port handle
        self.sps = 0.0              # holds the average sample acquisition rate
        self.exitFlag = False
        self.exitMutex = threading.Lock()
        self.dataMutex = threading.Lock()
        
        self.rptr = 0
        
        
    def run(self):
        exitMutex = self.exitMutex
        dataMutex = self.dataMutex
        buffer = self.buffer
        port = self.port
        count = 0
        sps = None
        lastUpdate = pg.ptime.time()
        
        txtfl = open("txtbuffer.txt", 'w')        
        
        while True:
            # see whether an exit was requested
            with exitMutex:
                if self.exitFlag:
                    break
            
            # read one full chunk from the serial port
            data = port.read(self.chunkSize*2)
            # convert data to 16bit int numpy array
            data = np.fromstring(data, dtype=np.uint16)
            
            # keep track of the acquisition rate in samples-per-second
            count += self.chunkSize
            now = pg.ptime.time()
            dt = now-lastUpdate
            if dt > 1.0:
                # sps is an exponential average of the running sample rate measurement
                if sps is None:
                    sps = count / dt
                else:
                    sps = sps * 0.9 + (count / dt) * 0.1
                count = 0
                lastUpdate = now
                
            # write the new chunk into the textfile
            with dataMutex:
                txtfl.write(np.array_str(data))
                if sps is not None:
                    self.sps = sps
        
        txtfl.close()

    def exit(self):
        """ Instruct the serial thread to exit."""
        with self.exitMutex:
            self.exitFlag = True

def main():
    # Get handle to serial port
    s = serial.Serial('/dev/tty.usbmodem1421')
            
    # Create thread to read and buffer serial data.
    thread = SerialReader(s)
    thread.start()
    #JOON makes sure that numpy won't truncate the middle of a long array
    np.set_printoptions(threshold = 'nan')

if __name__ == '__main__':
    main()