from pynput import keyboard
import serial
import time
from pynput.keyboard import Key, Controller

ser = serial.Serial('/dev/COM3')  # open serial port
ser = serial.Serial('COM3')  # open serial port
print(ser.name)         # check which port was really used
ser.write(b'hello')     # write a string
ser.close()             # close port


X="Texto a escribir"
for char in X:
    keyboard.press(char)
    keyboard.release(char)
    time.sleep(0.13)