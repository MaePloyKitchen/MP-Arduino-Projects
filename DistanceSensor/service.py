import serial
import os
from pyautogui import typewrite, click

#Insert password to your computer here. Add the newline character '\n' at the end
password = ''

#Insert your RFID Tag value here. Use 'UID: XX XX XX XX' format
uid = ''

#Insert your port. Usually COM or /dev/ttyACM0 
port = ''

if __name__ == '__main__':
    ser = serial.Serial(port,9600,timeout=1)
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
            elif line == uid:

                print("Trigger Keyboard sequence")
                click()
                typewrite(password)
                
            else:
                print("Nothing important")

            if lock_count > 5:
                #Uncomment which command would allow your computer to be locked. I haven't tested in Windows so it might not work.

                #os.system("rundll32.exe user32.dll, LockWorkStation")
                os.system('xdg-screensaver lock')

                lock_count = 0