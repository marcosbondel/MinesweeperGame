from flask import Blueprint, request
from utils.responder import respond_with_success, respond_with_error, respond_with_not_found
from db.ram import Ram
from models.bomb import Bomb
import numpy as np
from compiler.compiler import Compiler

BombBlueprint = Blueprint('bombs', __name__)

@BombBlueprint.route('/bombs.json')
def index_bombs():
    return respond_with_success([bomb.to_dict() for bomb in Ram.get_bombs()])

@BombBlueprint.route('/bombs.json', methods=['POST'])
def create_bomb():
    data = request.get_json()

    if not data:
        return respond_with_error("Missing JSON body")
    
    new_bomb = Bomb(data['x'], data['y'])

    if not new_bomb.valid_coordinates():
        return respond_with_error("Invalid coordinates")

    # If the bomb already exists at this location, we return and error
    if new_bomb.check_bomb_existence():
        return respond_with_error("Bomb already exists at this location")

    # Create the new bomb location and add it to the RAM
    Ram.add_bomb(new_bomb)

    return respond_with_success('Bomb created successfully')

@BombBlueprint.route('/bombs/matrix.json')
def bombs_matrix():
    response_data = {
        "configured": str(Ram.bombs_configured()),
        "matrix_representation": Ram.bombs_matrix_representation.tolist(),
    }

    return respond_with_success(response_data)

@BombBlueprint.route('/bombs/verify.json', methods=['POST'])
def verify_bomb():
    data = request.get_json()

    if not data:
        return respond_with_error("Missing JSON body")

    x = data.get('x')
    y = data.get('y')

    if not x or not y:
        return respond_with_error("Missing coordinates")

    if not isinstance(x, int) or not isinstance(y, int):
        return respond_with_error("Coordinates must be integers")

    if int(x) < 1 or int(x) > 4 or int(y) < 1 or int(y) > 4:
        return respond_with_error("Coordinates out of range")

    bombs = Ram.get_bombs()
    for bomb in bombs:
        if bomb.x == int(x) and bomb.y == int(y):
            Ram.verify_bomb(bomb)
            Ram.reset_game()
            return respond_with_success('BOOM')

    Ram.increment_points()

    if Ram.points == np.size(Ram.bombs_matrix_representation) - np.count_nonzero(Ram.bombs_matrix_representation):
        Ram.won()
        Ram.reset_game()
        return respond_with_success('WIN')

    return respond_with_success('SAFE')

@BombBlueprint.route('/bombs/import.json', methods=['POST'])
def import_bombs():
    data = request.get_json()
    
    if not data or 'content' not in data:
        return respond_with_error("Missing 'content' in the request")
    
    content = data['content']
    content = content.replace('\\n', '\n')  # Replace literal \n with actual newlines

    Ram.cache_imported_points = []  # Clear the cache before importing new points

    # Run the compiler with the content
    compiler = Compiler(content)
    compiler.run_scanner()
    compiler.run_parser()

    return respond_with_success({'message': 'Bombs imported successfully', 'content': Ram.cache_imported_points})
