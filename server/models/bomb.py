from db.ram import Ram


class Bomb:

    def __init__(self, x, y):
        self._x = x
        self._y = y
        self.exploded = False

    @property
    def x(self):
        return self._x
    @x.setter
    def x(self, value):
        if 0 <= value < 4:
            self._x = value
        else:
            raise ValueError("X coordinate out of range")
    
    @property
    def y(self):
        return self._y
    @y.setter
    def y(self, value):
        if 0 <= value < 4:
            self._y = value
        else:
            raise ValueError("Y coordinate out of range")

    def valid_coordinates(self):
        if not isinstance(self.x, int) or not isinstance(self.y, int):
            return False

        if self.x < 1 or self.x > 4 or self.y < 1 or self.y > 4:
            return False

        return True

    def check_bomb_existence(self):
        bombs = Ram.get_bombs()
        for bomb in bombs:
            print(bomb.x, bomb.y)
            if bomb.x == self.x and bomb.y == self.y:
                return True
        return False

    def to_dict(self):
        return {
            'x': self.x,
            'y': self.y,
        }