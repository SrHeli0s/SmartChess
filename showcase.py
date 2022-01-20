from stockfish import Stockfish
from pynput import keyboard
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

#Load stockfish
stockfish = Stockfish("./stockfish_14.1_avx2/stockfish_14.1_win_x64_avx2",parameters={"Threads": 2, "Minimum Thinking Time": 10})
#Open serial port
try:
    ser = serial.Serial('COM3',baudrate=9600, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)  # open serial port
    ser.reset_input_buffer()
    ser.reset_output_buffer()
except:
    print(bcolors.WARNING + "\n[WARN] SERIAL NOT FOUND, RUNNING IN DEVELOPMENT MODE (ALL CALLS TO SERIAL WILL EXPLODE)" + bcolors.ENDC)
#Configuration variables
verbose = False
move=0

#==================== FUNCTIONS ====================#
#Pre: move is a string that contains a move in coordinate format
#Post: The move is output to the mode configured
def output_move(move):
    try:
        stockfish.make_moves_from_current_position([move])
        print(stockfish.get_board_visual().replace("K"," ").replace("k"," ")+"\n\n")
    except:
        report_serial_error("Make move")


#Pre: desc contains a string describing the error
#Post: A warning is printed to the screen and the error message is sent to the board
def report_serial_error(desc = ""):
    print(bcolors.WARNING + "[WARN] ERROR IN SERIAL REPORTED: " + desc + bcolors.ENDC)
    ser.write(b'E')


#Pre: serial communication is in a unkown state
#Post: waits until there is a message and returns it
def read_block_serial():
    while ser.in_waiting == 0: pass
    serialString = ser.readline()
    ser.reset_input_buffer()
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
    serialString = ser.read_until('\n')
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
stockfish.set_fen_position("8/8/k7/8/8/8/6K1/2RQBN2 w - - 0 1")
print(stockfish.get_board_visual().replace("K"," ").replace("k"," ")+"\n\n")

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
        print("Waiting move...", end='\r')
        move = read_move_block_serial()
        print("Move recieved: " + bcolors.OKBLUE + move + bcolors.ENDC)
        output_move()
        if(move==0):
            stockfish.make_moves_from_current_position(["a6a7"])
            move=1
        else:
            stockfish.make_moves_from_current_position(["a7a6"])
            move=0
    
    print("Bye :)")
    ser.close()             # close port
    exit()
    
