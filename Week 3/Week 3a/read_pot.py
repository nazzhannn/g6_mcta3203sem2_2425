import serial

# Replace 'COM10' with your Arduino's actual serial port (e.g., 'COM3', 'COM4' for Windows)
ser = serial.Serial('COM10', 9600, timeout=1)

try:
    while True:
        pot_value = ser.readline().decode().strip()
        if pot_value:
            print("Potentiometer Value:", pot_value)

except KeyboardInterrupt:
    ser.close()
    print("Serial connection closed.")
