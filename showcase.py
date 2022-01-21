import chess
import serial
import time
from pynput.keyboard import Key, Controller
import stockfish

#Serial communication codes:
# e =   Error on last message, resend it
# s =   Start game
# <move> =  [a-h][1-8][a-h][1-8]\n =  Sequence of 4 characters describing origin position and final position 
#
# * If the character is in lowercase: board to computer
# * If the character is in uppercase: computer to board


#==================== GLOBAL VARIABLES ====================#
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

#Open serial port
try:
    ser = serial.Serial('COM3',baudrate=9600, bytesize=8, timeout=None, stopbits=serial.STOPBITS_ONE)  # open serial port
    ser.reset_input_buffer()
    ser.reset_output_buffer()
except:
    print(bcolors.WARNING + "\n[WARN] SERIAL NOT FOUND, RUNNING IN DEVELOPMENT MODE (ALL CALLS TO SERIAL WILL EXPLODE)" + bcolors.ENDC)
#Configuration variables
verbose = False
board=['□','□','□','□','□','□','□','□',
       '□','□','□','□','□','□','□','□',
       '□','□','□','□','□','□','□','□',
       '□','□','□','□','□','□','□','□',
       '□','□','□','□','□','□','□','□',
       '□','□','□','□','□','□','□','□',
       '□','□','□','□','□','□','□','□',
       '□','□','R','Q','B','N','□','□',]

#==================== FUNCTIONS ====================#


#Pre: move is a string that contains a move in coordinate format
#Post: The move is output to the mode configured
def output_move(move):
    temp = board[chess.parse_square(move[:2])]
    board[chess.parse_square(move[:2])] = '□'
    board[chess.parse_square(move[2:])] = temp

    for i in range(64):
        print(board[i], end='')
        if i%8 == 7:
            print()



#Pre: desc contains a string describing the error
#Post: A warning is printed to the screen and the error message is sent to the board
def report_serial_error(desc = ""):
    print(bcolors.WARNING + "[WARN] ERROR IN SERIAL REPORTED: " + desc + bcolors.ENDC)
    ser.write(b'E')


#Pre: serial communication is in a unkown state
#Post: waits until there is a message and returns it
def read_block_serial():
    while ser.in_waiting == 0: pass
    serialString = ser.read()
    # ser.reset_input_buffer()
    try:
        serialString = serialString.decode("Ascii")
        if verbose: print(bcolors.FAIL + "[VERBOSE] Executed block read with output " + serialString + bcolors.ENDC)
        return serialString
    except:
        if verbose: print(bcolors.FAIL + "[VERBOSE] Executed block read with bad output " + bcolors.ENDC)
        return ""

#Pre: serial communication is in a unkown state
#Post: waits until there is a message and returns it
def read_move_block_serial():
    while (1):
        if ser.in_waiting > 4:
            serialString = ser.read(4)
            ser.reset_input_buffer()
            try:
                serialString = serialString.decode("Ascii")

                if verbose: print(bcolors.FAIL + "[VERBOSE] Executed block read move with output " + serialString + bcolors.ENDC)
                return serialString
            except:
                if verbose: print(bcolors.FAIL + "[VERBOSE] Executed block read move with bad output " + bcolors.ENDC)
                ser.reset_input_buffer()
                return ""


#==================== MAIN ====================#
for i in range(64):
    print(board[i], end='')
    if i%8 == 7:
        print()
while (1):
    validResponse = False
    while(not validResponse):
        print("\n-- Verbose? (Y-n) --")
        print("q = QUIT")
        inp = input("")
        if inp.lower() == "y":
            validResponse = True
            verbose = True
        elif inp.lower() == "n":
            validResponse = True
            verbose = False
        elif inp == "q":
            exit()
    
    validResponse = False
    while(not validResponse):
        ser.write(b'S')
        response = read_block_serial()
        if response == 's':
            validResponse = True
        elif response == 'e':
            pass
        else:
            report_serial_error("Start game")
    
    print("I'm ready, you can start :)")
    while(1):
        move = read_move_block_serial()
        print("Move recieved: " + bcolors.OKBLUE + move + bcolors.ENDC)
        output_move(move)
    
    print("Bye :)")
    ser.close()             # close port
    exit()
    
