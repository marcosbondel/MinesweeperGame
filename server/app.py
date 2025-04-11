import atexit, subprocess
from config import FlaskServer
from utils.serial_connection import ArduinoSerial
from db.ram import Ram

Ram.arduino = None

if __name__ == "__main__":
    Ram.arduino = ArduinoSerial()
    try:
        Ram.arduino.connect()
        server = FlaskServer()
        server.run()

    except KeyboardInterrupt:
        print("\nInterrupted by user.")
        # arduino.close()
    finally:
        Ram.arduino.close()