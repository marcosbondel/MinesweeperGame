import numpy as np
from utils.serial_connection import ArduinoSerial

class Ram:
    bombs = []
    bombs_matrix_representation = np.zeros((4, 4), dtype=int)
    points = 0
    history_points = []
    cache_imported_points = []
    arduino = None

    def __init__(self):
        pass

    @staticmethod
    def read():
        pass

    @staticmethod
    def write(action, data = {}):
        if action == "add_bomb":
            Ram.arduino.send_message(f"add_bomb {data.x} {data.y}")
        elif action == "reset_game":
            Ram.arduino.send_message("reset_game")
        elif action == "increment_points":
            Ram.arduino.send_message("increment_points")
        elif action == "verify_bomb":
            Ram.arduino.send_message(f"verify_bomb {data.x} {data.y}")
        elif action == "play_game":
            Ram.arduino.send_message(f"play_game {data}")
        elif action == "won":
            Ram.arduino.send_message("won")
        elif action == "top5":
            Ram.arduino.send_message("show_top5")
        
    @staticmethod
    def add_bomb(bomb):
        Ram.bombs.append(bomb)
        Ram.bombs_matrix_representation[bomb.x-1, bomb.y-1] = 1
        Ram.write("add_bomb", bomb)

    @staticmethod
    def get_bombs():
        Ram.read()
        return Ram.bombs

    @staticmethod
    def increment_points():
        Ram.points += 1
        Ram.write('increment_points')
    
    @staticmethod
    def play_game(mode):
        Ram.write("play_game", mode)

    @staticmethod
    def verify_bomb(bomb):
        Ram.write("verify_bomb", bomb)

    @staticmethod
    def top5():
        Ram.write('top5')
    
    @staticmethod
    def won():
        Ram.write('won')
    
    @staticmethod
    def reset_game():
        Ram.bombs_matrix_representation = np.zeros((4, 4), dtype=int)
        Ram.history_points.append(Ram.points)
        Ram.bombs = []
        Ram.points = 0
        Ram.write('reset_game')
    
    @staticmethod
    def reset_backend():
        Ram.bombs_matrix_representation = np.zeros((4, 4), dtype=int)
        if Ram.points > 0:
            Ram.history_points.append(Ram.points)
        Ram.bombs = []
        Ram.points = 0
        Ram.write('reset_game')
    
    @staticmethod
    def bombs_configured():
        return np.any(Ram.bombs_matrix_representation == 1)
