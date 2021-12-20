from stockfish import Stockfish
from pynput import keyboard
import serial
import time
from pynput.keyboard import Key, Controller
import stockfish

#Serial communication codes:
# e =   Error on last message, resend it
# s =   Start game
# <move> =  Sequence of 4 characters describing origin position and final position 
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
except:
    print(bcolors.WARNING + "\n[WARN] SERIAL NOT FOUND, RUNNING IN DEVELOPMENT MODE (ALL CALLS TO SERIAL WILL EXPLODE)" + bcolors.ENDC)
#Configuration variables
output_type = 0 #0=none, 1=keyboard simulation, 2=file
f = None


#==================== FUNCTIONS ====================#
#Pre: move is a string that contains a move in coordinate format
#Post: The move is output to the mode configured
def output_move(move):
    if output_type == 0:
        stockfish.make_moves_from_current_position(move)
        print(stockfish.get_board_visual()+"\n\n")
    elif output_type == 1:
        for char in move:
            keyboard.press(char)
            keyboard.release(char)
            time.sleep(0.13)
    elif output_type == 2:
        f.write(move+"\n")


#Pre: desc contains a string describing the error
#Post: A warning is printed to the screen and the error message is sent to the board
def report_error(desc = ""):
    print(bcolors.WARNING + "[WARN] ERROR IN SERIAL REPORTED: "+desc + bcolors.ENDC)
    ser.write(b'E')


#Pre: serial communication is in a unkown state
#Post: returns the message waiting on serial or "" if there is no message
def read_noblock_serial():
    if ser.in_waiting > 0:
        serialString = ser.readline()
        try:
            return serialString.decode("Ascii")
        except:
            return ""
    return ""

#Pre: serial communication is in a unkown state
#Post: waits until there is a message and returns it
def read_block_serial():
    while ser.in_waiting == 0: pass
    serialString = ser.readline()
    try:
        return serialString.decode("Ascii")
    except:
        return ""

#Pre: serial communication is in a unkown state
#Post: waits until there is a message and returns it
def read_move_block_serial():
    while ser.in_waiting < 4: pass
    serialString = ser.readline()
    try:
        return serialString.decode("Ascii")
    except:
        return ""

#Pre: The game is in no gamemode loop
#Post: The game enters the computer gamemode loop
def loop_computer():
    validResponse = False
    while(not validResponse):
        print("\n-- Set ELO rating (0 to 20): --")
        print("q = QUIT")
        inp = input("")
        if inp == "q":
            exit()
        try:
            level = int(inp)
            if level>=0 and level<=20:
                stockfish.set_skill_level(level)
                validResponse = True
        except:
            pass
    
    validResponse = False
    while(not validResponse):
        ser.write(b'S')
        response = read_block_serial()
        if response == 's':
            validResponse = True
        else:
            report_error("Start computer gamemode")
    
    print("I'm ready, you can start :)")
    while(1):
        print("Waiting move...", end='\r')
        move = read_move_block_serial()
        print("Move recieved: " + bcolors.OKBLUE + move + bcolors.ENDC)
        stockfish.make_moves_from_current_position(move)
        print("Thinking...", end='\r')
        for item in stockfish.get_top_moves():
            print("Move: " + bcolors.OKGREEN + item["Move"] + bcolors.ENDC + "   Evaluation: "+item["Centipawn"]+"   Mate:" + bcolors.OKCYAN + item["Mate"] + bcolors.ENDC)
        print("\n")



#Pre: The game is in no gamemode loop
#Post: The game enters the normal gamemode loop
def loop_normal():
    validResponse = False
    while(not validResponse):
        print("\n-- Select desired output: --")
        print("1 = keyboard simulation")
        print("2 = show real time")
        print("3 = save to file")
        print("q = QUIT")
        inp = input("")
        if inp == "1":
            output_type = 1
            validResponse = True
        elif inp == "2":
            output_type = 0
            validResponse = True
        elif inp == "3":
            output_type = 2
            validResponse = True
            print("\n-- Write file name: --")
            inp = input("")
            f = open(inp+".txt", "w")
        elif inp == "q":
            exit()
    
    validResponse = False
    while(not validResponse):
        ser.write(b'S')
        response = read_block_serial()
        if response == 's':
            validResponse = True
        else:
            report_error("Start normal gamemode")

    print("I'm ready, you can start :)")
    while(1):
        print("Waiting move...", end='\r')
        move = read_move_block_serial()
        print("Move recieved: " + bcolors.OKBLUE + move + bcolors.ENDC)
        output_move(move)
        

#Pre: The game is in no gamemode loop
#Post: The game enters the 960 gamemode loop
def loop_960():
    validResponse = False
    while(not validResponse):
        print("\n-- Select desired output: --")
        print("1 = keyboard simulation")
        print("2 = show real time")
        print("3 = save to file")
        print("q = QUIT")
        inp = input("")
        if inp == "1":
            output_type = 1
            validResponse = True
        elif inp == "2":
            output_type = 0
            validResponse = True
        elif inp == "3":
            output_type = 2
            validResponse = True
            print("\n-- Write file name: --")
            inp = input("")
            f = open(inp+".txt", "w")
        elif inp == "q":
            exit()
    #TODO: Implement generation of 960
    validResponse = False
    while(not validResponse):
        ser.write(b'S')
        response = read_block_serial()
        if response == 's':
            validResponse = True
        else:
            report_error("Start 960 gamemode")

    print("I'm ready, you can start :)")
    while(1):
        print("Waiting move...", end='\r')
        move = read_move_block_serial()
        print("Move recieved: " + bcolors.OKBLUE + move + bcolors.ENDC)
        output_move(move)


#==================== MAIN ====================#
while (1):
    validResponse = False
    while(not validResponse):
        print("\n-- Select game mode: --")
        print("1 = Normal game")
        print("2 = 960 game")
        print("3 = Computer game")
        print("q = QUIT")
        inp = input("")
        if inp == "1":
            validResponse = True
            loop_normal()
        elif inp == "2":
            validResponse = True
            loop_960()
        elif inp == "3":
            validResponse = True
            loop_computer()
        elif inp == "q":
            exit()
    
    print("Bye :)")
    ser.close()             # close port
    if f!=None: f.close()
    exit()
    
