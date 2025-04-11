import ply.yacc as yacc
from compiler.scannerp.scanner import tokens
from db.ram import Ram
from models.bomb import Bomb

def p_config(p):
    '''config : INIT_DELIMITER statements FINAL_DELIMITER'''
    p[0] = ('config', p[2])

def p_statements(p):
    '''statements : statement
                  | statements statement'''
    if len(p) == 2:
        p[0] = [p[1]]
    else:
        p[0] = p[1] + [p[2]]

def p_statement(p):
    '''statement : add_statement
                 | newline'''
    if len(p) == 2 and p[1] is not None:
        p[0] = p[1]

def p_add_statement(p):
    'add_statement : ADD X_COORD COLON NUMBER COMMA Y_COORD COLON NUMBER'
    p[0] = ('add', p[2], p[4], p[6], p[8])
    
    new_bomb = Bomb(p[4], p[8])

    if new_bomb.valid_coordinates() and not new_bomb.check_bomb_existence():
        Ram.add_bomb(Bomb(p[4], p[8]))
        Ram.cache_imported_points.append({'x': p[4], 'y': p[8]})
        print("Coordenadas agregadas:", p[2], p[4], p[6], p[8])


def p_newline(p):
    'newline : '
    pass  # Ignorar líneas vacías

def p_error(p):
    if p:
        print(f"Error de sintaxis en '{p.value}' en la línea {p.lineno}")
    else:
        print("Error de sintaxis al final del archivo")



parser = yacc.yacc(debug=True)