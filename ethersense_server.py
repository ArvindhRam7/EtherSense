from flask import Flask, request
from datetime import datetime

app = Flask(__name__)

LOG_FILE = "sensor_log.csv"

@app.route("/log", methods=["POST"])
def log_sensor_data():
    data = request.data.decode().strip()
    print(f"[{datetime.now()}] Received: {data}")

    # Optional: append to CSV
    with open(LOG_FILE, "a") as f:
        f.write(f"{datetime.now()},{data}\n")

    return "OK", 200

if __name__ == "__main__":
    print("EtherSense server starting on port 5000...")
    app.run(host="0.0.0.0", port=5000)
