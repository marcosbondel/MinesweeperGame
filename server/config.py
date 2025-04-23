from flask import Flask, request
from utils.responder import respond_with_success, respond_with_error, respond_with_not_found
from controllers.bombs_controller import BombBlueprint
from flask_cors import CORS
from db.ram import Ram

class FlaskServer:

    def __init__(self):
        self.app = Flask(__name__)
        self.register_blueprints()
        CORS(self.app)
        self.app.register_error_handler(404, respond_with_not_found)
        self.app.register_error_handler(500, respond_with_error)
        self.app.register_error_handler(Exception, respond_with_error)
        self.read_db()
        self.setup_routes()

    def register_blueprints(self):
        self.app.register_blueprint(BombBlueprint)

    def setup_routes(self):

        @self.app.route('/')
        def index():
            return "Hello, World!"
        
        @self.app.route('/game/play.json', methods=['POST'])
        def play_game():
            Ram.play_game(request.get_json()['game_mode'])

            return respond_with_success("Game played successfully")
        
        @self.app.route('/game/top5.json', methods=['POST'])
        def top5():
            Ram.top5()

            return respond_with_success("Top 5 players retrieved successfully")

        @self.app.route('/game/reset.json', methods=['POST'])
        def reset_game():
            Ram.reset_backend()

            return respond_with_success("Reset successfully")

    def read_db(self):
        pass

    def run(self):
        self.app.run(host='0.0.0.0', port=3000, debug=True)