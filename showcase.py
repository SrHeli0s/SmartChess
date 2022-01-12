import serial

ser = serial.Serial('COM3',baudrate=9600, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)  # open serial port
ser.reset_input_buffer()
ser.reset_output_buffer()

while (1):
    if ser.in_waiting > 0:
        serialString = ser.readline()
        try:
            print(serialString.decode("Ascii"))
        except:
            pass