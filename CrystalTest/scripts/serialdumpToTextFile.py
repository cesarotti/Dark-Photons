import serial

ser = serial.Serial('COM3',115200)
textfile = open("dataMARKV.txt", 'w')
for i in range(0,5):
    print ser.readline().strip()
    textfile.write(ser.readline().strip())
textfile.close()
