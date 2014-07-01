import serial

ser = serial.Serial('COM3',115200)
textfile = open("data", 'w')
while (1):
    print ser.readline().strip()
    textfile.write(ser.readline().strip())
textfile.close()
