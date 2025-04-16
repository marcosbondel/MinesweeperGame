export default class Ram {
    static bombs = []
    static matrix_representation = []
    static points = 0
    static configured = false
    static game_mode = ""
    static status = "not_started"

    static async fetchBombs(){
        const response = await fetch('http://localhost:3000/bombs.json');
      
        if (!response.ok) {
            console.log('Error fetching bombs:', response.statusText);
            return;
        }

        
        Ram.bombs = await response.json();
        console.log(Ram.bombs);
    }

    static async fetchMatrixRepresentation(){
        const response = await fetch('http://localhost:3000/bombs/matrix.json');
      
        if (!response.ok) {
            console.log('Error fetching matrix representation:', response.statusText);
            return;
        }
        
        // Ram.matrix_representation = await response.json();
        const response_json = await response.json();
        Ram.configured = response_json['configured'];
        console.log(Ram.configured);
        Ram.matrix_representation = response_json['matrix_representation'];
    }
}
