import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Set up the serial connection (Change 'COM3' to your port)
ser = serial.Serial('COM10', 9600, timeout=1)

# Store data
time_values = []
pot_values = []
max_points = 100  # Maximum points to show on the graph

# Set up plot
fig, ax = plt.subplots()
ax.set_ylim(0, 1023)  # Potentiometer range (0-1023 for Arduino)
ax.set_xlim(0, max_points)
line, = ax.plot([], [], 'b-', label="Potentiometer Value")
ax.legend()

# Function to update plot
def update(frame):
    try:
        data = ser.readline().decode('utf-8').strip()
        if data:
            pot_value = int(data)
            time_values.append(len(time_values))
            pot_values.append(pot_value)
            
            # Keep only last 'max_points' values
            if len(time_values) > max_points:
                time_values.pop(0)
                pot_values.pop(0)
            
            # Update plot
            line.set_data(time_values, pot_values)
            ax.set_xlim(time_values[0], time_values[-1])
    except:
        pass

    return line,

# Animate the plot
ani = animation.FuncAnimation(fig, update, interval=100, blit=True)

plt.xlabel("Time (frames)")
plt.ylabel("Potentiometer Reading")
plt.title("Real-time Potentiometer Readings")
plt.show()

# Close serial connection on exit
ser.close()
