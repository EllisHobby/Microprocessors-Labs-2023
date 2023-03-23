import time
import json

import serial
import serial.tools.list_ports as tools

ARDUINO_MANUFACTURER = 'Arduino'

REQ_HANDSHAKE =  0
REQ_DIRECTION =  1
REQ_BUZZER    =  2
REQ_SHAKE     =  3


class SerialController:
    
    def __init__(self, device = ARDUINO_MANUFACTURER, baudrate=9600, timeout = 2,  **kwargs):

        self.dev_type = device
        self.timeout = timeout
        self.baud = baudrate
        self.dev = None


    def find_device (self):
        '''
        attempt to find device

        '''
        print(f'Attempting to find: {self.dev_type}')
        
        for port in tools.comports():
            if self.dev_type in port.manufacturer:
                print(f'Found device: {port.manufacturer} {port.device}')
                self.dev = serial.Serial(port.device, baudrate=self.baud, timeout=self.timeout)
    

    def check_connection (self, handshake_code=REQ_HANDSHAKE):
        '''
        ensure connection working

        '''
        print('Checking Connection')
        if self.dev:
            
            self.dev.close()
            self.dev.open()
           
            time.sleep(1)

            self.dev.read_until()
            self.dev.read_until()

            self.dev.write(bytes([handshake_code]))
            
            msg = self.dev.read_until().decode().strip('\r\n')

            print(f'Status Message Recieved: {msg}')
        else:
            print('ERROR: No device')


    def get_control_dict(self, dispatch=REQ_DIRECTION):
        '''
        get game direction data as dict

        '''

        if self.dev:
            # write dispatch code
            self.dev.write(bytes([dispatch]))

            # load control data as dict
            msg = self.dev.read_until().decode().strip('\r\n')
            control = json.loads(msg)

            return control
        else:
            print('No device')


    def reset_shake (self, dispatch=REQ_SHAKE):
        '''
        send request to reset shake variable

        '''
        if self.dev:
            self.dev.write(bytes([dispatch]))
        else:
            print('No device')

    
    def start_buzzer (self, dispatch=REQ_BUZZER):
        '''
        send request to start buzzer

        '''
        if self.dev:
            self.dev.write(bytes([dispatch]))
        else:
            print('No device')
        



if __name__ == "__main__":

    print("Testing Serial Control")
    
    ser = SerialController()
    ser.find_device()
    ser.check_connection()
    
    if ser.dev:
        while True:
            data = ser.get_control_dict()
            print(data) 
            time.sleep(0.1)
          
        
