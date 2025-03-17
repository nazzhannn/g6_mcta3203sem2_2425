import serial
import time

# Define the serial port and baud rate (adjust 'COM10' to your Arduino port)
ser = serial.Serial('COM10', 9600, timeout=1)
time.sleep(2)  # Allow time for Arduino to reset

def send_angle(angle):
    """Send the servo angle to Arduino"""
    ser.write(f"{angle}\n".encode())  # Send data with newline
    ser.flush()  # Ensure data is sent
    time.sleep(0.1)  # Give Arduino time to process

try:
    print(ser.readline().decode().strip())  # Read Arduino's startup message
    while True:
        angle = input("Enter servo angle (0-180 degrees, or 'q' to quit): ")
        if angle.lower() == 'q':
            break
        try:
            angle = int(angle)
            if 0 <= angle <= 180:
                send_angle(angle)
                print(ser.readline().decode().strip())  # Read Arduino response
            else:
                print("Angle must be between 0 and 180 degrees.")
        except ValueError:
            print("Invalid input. Please enter a number between 0 and 180.")
except KeyboardInterrupt:
    print("\nKeyboard interrupt detected. Exiting...")
finally:
    ser.close()  # Close the serial connection
    print("Serial connection closed.")
