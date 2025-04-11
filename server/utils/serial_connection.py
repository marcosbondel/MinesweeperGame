import serial
import serial.tools.list_ports
import time

class ArduinoSerial:
    def __init__(self, port=None, baudrate=9600, timeout=1):
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout
        self.ser = None

    def auto_detect_port(self):
        ports = serial.tools.list_ports.comports()
        for p in ports:
            if "usbmodem" in p.device or "usbserial" in p.device:
                return p.device
        return None

    def connect(self):
        try:
            if not self.port:
                self.port = self.auto_detect_port()
                if not self.port:
                    raise serial.SerialException("No Arduino port found automatically.")
                else:
                    print(f"[INFO] Arduino auto-detected on {self.port}")

            self.ser = serial.Serial(self.port, self.baudrate, timeout=self.timeout)
            time.sleep(2)  # wait for Arduino to initialize
            print(f"[INFO] Serial connection established on {self.port}")
        except serial.SerialException as e:
            print(f"[ERROR] Serial error: {e}")
            raise
        except Exception as e:
            print(f"[ERROR] Unexpected error during connection: {e}")
            raise

    def send_message(self, msg):
        if self.ser and self.ser.is_open:
            self.ser.write(f"{msg}\n".encode('utf-8'))
            time.sleep(0.1)
        else:
            print("[WARN] Serial port is not open. Call connect() first.")

    def read_response(self):
        responses = []
        if self.ser and self.ser.in_waiting > 0:
            while self.ser.in_waiting > 0:
                try:
                    line = self.ser.readline().decode('utf-8').strip()
                    if line:
                        responses.append(line)
                except UnicodeDecodeError:
                    print("[WARN] Error decoding serial response")
        return responses

    def close(self):
        if self.ser and self.ser.is_open:
            self.ser.close()
            print("[INFO] Serial port closed.")
