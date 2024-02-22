import serial
import os
from pyautogui import typewrite, click

#Insert password to your computer here. Add the newline character '\n' at the end
password = ''

#Insert your RFID Tag value here. Use 'UID: XX XX XX XX' format
UID = ''

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0',9600,timeout=1)
    ser.flush()
    lock_count = 0
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)

            #Get the data that contains float data first to convert. 
            #If the data is for the UID, initiate the keyboard sequence
            if '.' in line:
                if float(line) >= 200:
                    print("Lock")
                    lock_count += 1
                else:
                    lock_count = 0
            elif line == UID:

                print("Trigger Keyboard sequence")
                click()
                typewrite(password)
                
            else:
                print("Nothing important")

            if lock_count > 5:
                os.system('xdg-screensaver lock')
                lock_count = 0