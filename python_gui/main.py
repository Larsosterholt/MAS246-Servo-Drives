# Importing Libraries
# In terminal: pip install pyserial
# in terminal: sudo apt-get install python3-tk
import serial
import time
#import threading
import re



arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)
#Should be port="COM4" or port="COM3" for windows
#linux: port="/dev/ttyACM0"
#Virtual port: port="/dev/pts/6" in a tertminal: cat < /dev/pts/> for debugging

# To kill prosses fuser -k /dev/ttyACM0 in case if the port still is open after exiting GUI


def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data


# Python program to create a simple GUI
# calculator using Tkinter

# import everything from tkinter module
from tkinter import *



# globally declare the expression variable
expressionPressed = ""
expressionActual = ""
# create a GUI window
gui = Tk()
actualFloor = StringVar()


# Function to update expression
# in the text entry box
def press(num):
    # point out the global expression variable
    global expressionPressed
    expressionPressed = "You pressed: "
    floorPressed.set("")
    # concatenation of string
    expressionPressed = expressionPressed + str(num)

    # update the expression by using set method
    floorPressed.set(expressionPressed)

    # Writng To seral:
    value = write_read(str(num))

def getFloor():
    numLast = "rubish"
    while 1:
        num = str(arduino.read(2))
        num = ''.join(x for x in num if x.isdigit())

        if num != numLast and num.isdigit():
            numLast= num
            # point out the global expression variable
            global expressionActual
            global actualFloor
            expressionActual = "Actual floor: "
            actualFloor.set("")
            # concatenation of string
            expressionActual = expressionActual + str(num)
            actualFloor.set(expressionActual)
            print("Actual floor (from arduino): ", str(num))



def funcExit(gui):
    arduino.close()
    gui.quit()


# Driver code
if __name__ == "__main__":
    # create a GUI window
   # gui = Tk()

    # set the background colour of GUI window
    gui.configure(background="white")

    # set the title of GUI window
    gui.title("Elevator")

    # set the configuration of GUI window
    gui.geometry("300x350")

    # StringVar() is the variable class
    # we create an instance of this class
    floorPressed = StringVar()
   # actualFloor = StringVar()

    # create the text entry box for
    # showing the expression .
    expression_floorPressed = Entry(gui, textvariable=floorPressed)
    #expression_actualFloor = Entry(gui, textvariable=actualFloor)

    # grid method is used for placing
    # the widgets at respective positions
    # in table like structure .
    expression_floorPressed.grid(columnspan=5, ipadx=70)
    #expression_actualFloor.grid(columnspan=5, ipadx=70)


    # create a Buttons and place at a particular
    # location inside the root window .
    # when user press the button, the command or
    # function affiliated to that button is executed .
    button1 = Button(gui, text=' 1. ', fg='black', bg='red',
                     command=lambda: press(11), height=1, width=7)
    button1.grid(row=2, column=1)

    button2 = Button(gui, text=' 2. ', fg='black', bg='red',
                     command=lambda: press(22), height=1, width=7)
    button2.grid(row=3, column=1)

    button3 = Button(gui, text=' 3. ', fg='black', bg='red',
                     command=lambda: press(33), height=1, width=7)
    button3.grid(row=4, column=1)

    button4 = Button(gui, text=' 1. floor call button', fg='black', bg='red',
                     command=lambda: press(1), height=1, width=15)
    button4.grid(row=2, column=3)

    button5 = Button(gui, text=' 2. floor call button', fg='black', bg='red',
                     command=lambda: press(21), height=1, width=15)
    button5.grid(row=3, column=3)

    button6 = Button(gui, text=' 3. floor call button', fg='black', bg='red',
                     command=lambda: press(3), height=1, width=15)
    button6.grid(row=4, column=3)

    Exit = Button(gui, text=' Exit GUI ', fg='black', bg='red',
                     command=lambda: funcExit(gui), height=1, width=10)
    Exit.grid(row=15, column=2)

    lLeft = Label(text="Inside")
    lLeft.grid(row=1, column=1)

    lLeft = Label(text="Outside (call buttons)")
    lLeft.grid(row=1, column=3)

    Emergency = Button(gui, text=' Emergency button', fg='black', bg='red',
                     command=lambda: press(911), height=1, width=15)
    Emergency.grid(row=6, column=1)

    Reset = Button(gui, text=' Reset alarm button', fg='black', bg='red',
                     command=lambda: press(999), height=1, width=15)
    Reset.grid(row=7, column=1)


    #Order = Button(gui, text=' Hei ', fg='black', bg='red',
    #                 command=lambda: funcExit(gui), height=1, width=7)
    #Order.grid(row=5, column=3)


    #Starting a thread to get floor from arduino
   # trd1 = threading.Thread(target=getFloor)
    #trd1.run()
   # trd1.start()
    # start the GUI
    gui.mainloop()

