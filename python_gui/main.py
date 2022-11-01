# Importing Libraries
import serial
import time
from tkinter import *

arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)
# windows: port="COM4" or port="COM3"
# linux: port="/dev/ttyACM0" ore simular
# Virtual port: <port="/dev/pts/6> in a tertminal: cat < /dev/pts/> for debugging

# To kill prosseses fuser -k /dev/ttyACM0 in case if the port still is open after exiting GUI


def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data


# globally declare the expression variable
expressionPressed = ""
expressionActual = ""
# create a GUI window
gui = Tk()
actualFloor = StringVar()


# Function to send data on serial, and update textbox for debugging.
def press(num):
    global expressionPressed
    expressionPressed = "You pressed: "
    floorPressed.set("")
    expressionPressed = expressionPressed + str(num)
    floorPressed.set(expressionPressed)

    # Writng To seral:
    value = write_read(str(num))


# Get floor data from arduion TODO
def getFloor():
    numLast = "rubish"
    while 1:
        num = str(arduino.read(2))
        num = ''.join(x for x in num if x.isdigit())

        if num != numLast and num.isdigit():
            numLast = num
            # point out the global expression variable
            global expressionActual
            global actualFloor
            expressionActual = "Actual floor: "
            actualFloor.set("")
            # concatenation of string
            expressionActual = expressionActual + str(num)
            actualFloor.set(expressionActual)
            print("Actual floor (from arduino): ", str(num))


# Exit functiuon
def funcExit(gui):
    arduino.close()  # Closes the port
    gui.quit()


if __name__ == "__main__":
    gui.configure(background="white")
    gui.title("Elevator")
    gui.geometry("350x350")

    # StringVar() is the variable class
    # we create an instance of this class
    floorPressed = StringVar()

    # create the text entry box for
    # showing the expression .
    expression_floorPressed = Entry(gui, textvariable=floorPressed)
    expression_floorPressed.grid(columnspan=5, ipadx=70)

    # Elevator floor buttons
    button1 = Button(gui, text=' 1. ', fg='black', bg='blue',
                     command=lambda: press(11), height=1, width=7)
    button1.grid(row=2, column=1)

    button2 = Button(gui, text=' 2. ', fg='black', bg='blue',
                     command=lambda: press(22), height=1, width=7)
    button2.grid(row=3, column=1)

    button3 = Button(gui, text=' 3. ', fg='black', bg='blue',
                     command=lambda: press(33), height=1, width=7)
    button3.grid(row=4, column=1)

    Emergency = Button(gui, text=' Emergency button', fg='black', bg='blue',
                       command=lambda: press(911), height=1, width=15)
    Emergency.grid(row=6, column=1)

    Reset = Button(gui, text=' Reset alarm button', fg='black', bg='blue',
                   command=lambda: press(999), height=1, width=15)
    Reset.grid(row=7, column=1)

    # Call to  floor buttons
    button4 = Button(gui, text=' Going Up', fg='black', bg='green',
                     command=lambda: press(1), height=1, width=15)
    button4.grid(row=3, column=3)

    button5 = Button(gui, text=' Going Up', fg='black', bg='green',
                     command=lambda: press(23), height=1, width=15)
    button5.grid(row=6, column=3)

    button6 = Button(gui, text=' Going Down', fg='black', bg='green',
                     command=lambda: press(21), height=1, width=15)
    button6.grid(row=7, column=3)

    button7 = Button(gui, text=' Going Down', fg='black', bg='green',
                     command=lambda: press(3), height=1, width=15)
    button7.grid(row=9, column=3)

    # Exit button. This is the prefered method to exit the gui. This closes the port.
    Exit = Button(gui, text=' Exit GUI ', fg='black', bg='red',
                  command=lambda: funcExit(gui), height=1, width=10)
    Exit.grid(row=20, column=2)

    # Some labels:
    lLeft = Label(text="Inside cabin")
    lLeft.grid(row=1, column=1)

    lLeft = Label(text="Outside (call buttons)")
    lLeft.grid(row=1, column=3)

    tFirst = Label(text="Button panel 1. floor")
    tFirst.grid(row=2, column=3)

    tSecond = Label(text="Button panel 2. floor")
    tSecond.grid(row=4, column=3)

    tThird = Label(text="Button panel 3. floor")
    tThird.grid(row=8, column=3)

    gui.mainloop()
