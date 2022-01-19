import sys
import serial

ser = serial.Serial('COM3',baudrate=9600, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)  # open serial port
ser.reset_input_buffer()
ser.reset_output_buffer()
buffer = ""
while (1):
    if ser.in_waiting > 0:
        serialString = ser.readline()
        try:
            buffer += serialString.decode("Ascii")
            if buffer.find("\n\n") != -1:
                print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nRecieved:\n")
                tuple = buffer.partition("\n\n")
                print(tuple[0])
                buffer = tuple[2]

        except:
            pass