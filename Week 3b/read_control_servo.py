import serial
import time

# Connect to Arduino (Change 'COM10' to your port, e.g., 'COM3' for Windows, '/dev/ttyUSB0' for Linux)
ser = serial.Serial('COM10', 9600, timeout=1)
time.sleep(2)  # Wait for Arduino to initialize

try:
    print(ser.readline().decode().strip())  # Read Arduino startup message

    while True:
        angle_data = ser.readline().decode().strip()  # Read angle from Arduino
        if angle_data:
            print(f"Servo Angle: {angle_data}°")  # Display servo position
except KeyboardInterrupt:
    print("\nExiting...")
finally:
    ser.close()  # Close serial connection
    print("Serial connection closed.")
