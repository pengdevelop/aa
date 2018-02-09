import serial
import sys
import datetime

keyword = '#'
timeout = 20 

def minicom():
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.port = 'COM4'
    ser.timeout = 1 
    return ser

def write(ser):
    
    global keyword
    strr = sys.argv[1]
    chrr = strr.split("+")

    if len(chrr) > 1:
        chrr[1] = chrr[1].strip()
        print chrr[1]
        cmd = chr(ord(chrr[1].upper()) - 64)
    else:
        cmd = strr

    if ser.isOpen():
        ser.write(cmd + '\n')
    else:
        ser.open()
        ser.write(cmd + '\n')
    ser.write('\n')

def read(ser):
    while True:
        data = ser.readline()
        sys.stdout.write(data)
        if data:
            if data.find(keyword) >= 0:
                break
        else:
            ser.write('\n')
            break
if __name__ == '__main__':
    ser = minicom()
    if ser:
        write(ser)
        read(ser)
        ser.close()
    else:
        print 'wrong serial'
