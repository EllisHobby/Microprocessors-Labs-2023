import time
import json

import serial
import serial.tools.list_ports as tools

ARDUINO_MANUFACTURER = 'Arduino'

HANDSHAKE = 0
AXIS_CONTROL = 1
SHAKE_OUT = 2
EXIT = 99



class SerialController:
    
    def __init__(self, device = ARDUINO_MANUFACTURER, baudrate=9600, timeout = 1,  **kwargs):

        self.dev_type = device
        self.timeout = timeout
        self.baud = baudrate
        self.dev = self.find_device()


    def find_device (self):
        '''
            attempt to find device
        '''
        print(f'Attempting to find: {self.dev_type}')
        
        for port in tools.comports():
            if self.dev_type in port.manufacturer:
                print(f'Found device: {port.manufacturer} {port.device}')
                return serial.Serial(port.device, baudrate=self.baud, timeout=self.timeout)
    

    def check_connection (self, handshake_code=HANDSHAKE):
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

    def get_control_dict(self, dispatch=AXIS_CONTROL):
        
        if self.dev:
            # write dispatch code
            self.dev.write(bytes([dispatch]))

            # load control data as dict
            msg = self.dev.read_until().decode().strip('\r\n')
            control = json.loads(msg)

            return control
        else:
            print('No device')

    def reset_shake (self, dispatch=SHAKE_OUT):
        if self.dev:
            self.dev.write(bytes([dispatch]))
        else:
            print('No device')
        



if __name__ == "__main__":
    run = True
    move = "+X"
    ser = SerialController()
    ser.check_connection(HANDSHAKE)
    if ser.dev:
        while run:
            try:
                data = ser.get_control_dict()
                print(f"x = {data['x']}, y = {data['y']}, z = {data['z']}")
                
                if (data['z'] > 7e3):
                    print('----- SHAKE ------')
                    ser.reset_shake()
                else:
                    if ((abs(data['x']) >= 50) | (abs(data['y']) >= 50)):
                        
                        if (abs(data['x']) > abs(data['y'])):
                            if (data['x'] > 0):
                                move = '+X'
                            else:
                                move = '-X'
                        
                        if (abs(data['y']) > abs(data['x'])):
                            if (data['y'] > 0):
                                move = '+Y'
                            else:
                                move = '-Y'
                
                
                

                print(f'Move: {move}')
                
                time.sleep(0.1)
            except:
                run = False
                exit()
        
