from pynput import keyboard
import serial
import time
from pynput.keyboard import Key, Controller

ser = serial.Serial('COM3',baudrate=9600, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)  # open serial port
print(ser.name)         # check which port was really used
while (1):
    if ser.in_waiting > 0:
        serialString = ser.readline()
        try:
            print(serialString.decode("Ascii"))
        except:
            pass

ser.write(b'hello')     # write a string
ser.close()             # close port


X="Texto a escribir"
for char in X:
    keyboard.press(char)
    keyboard.release(char)
    time.sleep(0.13)