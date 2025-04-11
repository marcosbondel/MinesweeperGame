from enum import Enum


class GameStatus(Enum):
    PLAYING = "playing"
    LOST = "lost"
    WON = "won"

class Game:

    points = 0
    history = []

    def __init__(self):
        pass

    @property
    def status(self):
        return self._status

    @status.setter
    def status(self, status):
        self._status = GameStatus(status)

    @property
    def points(self):
        return self._points
    @points.setter
    def points(self, points):
        self._points = points

    