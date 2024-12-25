import socket
import serial
import time

def initialize_arduino(port='WRITE HERE', baudrate=9600, timeout=0.01):
    while True:
        try:
            return serial.Serial(port=port, baudrate=baudrate, timeout=timeout)
        except serial.serialutil.SerialException:  # Fully qualified exception
            print("Retrying connection...")
            time.sleep(2)  

def getlocalip():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("192.168.0.1", 1))  # Connect to a local address to determine the local IP
        localip = s.getsockname()[0]
        s.close()
        return localip
    except (OSError, socket.error) as e:
        print(f"Error getting local IP: {e}")
        return None  # Return None if unable to determine local IP
    finally:
        s.close()

arduino = initialize_arduino()
last_ip = None

while True:
    try:
        
        localip = getlocalip()

        if localip is None:
            time.sleep(1) 
            continue

        if localip != last_ip:
            arduino.write(f"{localip}\n".encode())
            arduino.flush() 
            print(f"IP Sent: {localip}", flush=True)
            last_ip = localip  

        if arduino.in_waiting > 0:
            arduino_output = arduino.readline().decode().strip()
            print(f"! {arduino_output}", flush=True)

        time.sleep(0.1)

    except serial.serialutil.SerialException:
        print("Connection lost.")
        arduino = initialize_arduino() 
        last_ip = None 
