from compiler.scannerp.scanner import lexer
from compiler.parserp.parser  import parser

class Compiler:

    def __init__(self, entry):
        self.lexer = lexer
        self.parser = parser
        self.entry = entry

    def run_scanner(self):
        self.lexer.input(self.entry)
        while True:
            tok = lexer.token()
            if not tok: 
                break      # No more input
            print(f"Tipo: {tok.type}, Valor: {tok.value}, Línea: {tok.lineno}")

    def run_parser(self):
        self.parser.parse(self.entry)

