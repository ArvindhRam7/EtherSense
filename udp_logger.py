import socket
import csv
from datetime import datetime

# Set up UDP socket
UDP_IP = "0.0.0.0"  # Listen on all interfaces
UDP_PORT = 5000     # STM32's tx port

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for UDP packets on {UDP_IP}:{UDP_PORT}...")

# Receive, parse, and log
with open("sensor_log.csv", mode="a", newline="") as file:
    writer = csv.writer(file)

    while True:
        try:
            # Wait for data
            data, addr = sock.recvfrom(1024)
            message = data.decode('utf-8').strip()
            print(f"Received from {addr}: {message}")

            # Parse message like "TEMP:24.8,HUM:40.5"
            fields = dict(item.split(":") for item in message.split(","))
            temperature = float(fields.get("TEMP", 0))
            humidity = float(fields.get("HUM", 0))

            # Timestamp the reading
            timestamp = datetime.now().isoformat()

            # Write to CSV
            writer.writerow([timestamp, temperature, humidity])
            file.flush()

        except Exception as e:
            print(f"Error processing packet: {e}")
