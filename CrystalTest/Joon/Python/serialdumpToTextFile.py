import serial

ser = serial.Serial('COM3',115200)
textfile = open("", 'w')
for i in range(0,4):
    print ser.readline().strip()
    textfile.write(ser.readline().strip())
textfile.close()