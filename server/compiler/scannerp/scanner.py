import ply.lex as lex

# List of token names

tokens = (
    'OLINE_COMMENT',
    'MLINE_COMMENT',
    
    'INIT_DELIMITER',
    
    'ADD',

    'X_COORD',
    'Y_COORD',

    'COLON',
    'NUMBER',

    'COMMA',

    'FINAL_DELIMITER'
)

# Regular expression rules for simple tokens

t_INIT_DELIMITER = r'conf_ini'
t_ADD = r'ADD'
t_X_COORD = r'x'
t_Y_COORD = r'y'
t_COLON = r':'
t_COMMA = r','
t_FINAL_DELIMITER = r'conf:fin'

# Regular expression rules for complex tokens
def t_NUMBER(t):
    r'\d+'
    t.value = int(t.value)    
    return t

# Rule for single-line comments (e.g., // comment)
def t_OLINE_COMMENT(t):
    r'//.*'
    pass  # Ignore the comment or process it if needed

# Rule for multi-line comments (e.g., /* comment */)
def t_MLINE_COMMENT(t):
    r'/\*.*?\*/'
    pass  # Ignore the comment or process it if needed

# Define a rule so we can track line numbers
def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)

# A string containing ignored characters (spaces and tabs)
t_ignore  = ' \t'

# Error handling rule
def t_error(t):
    print("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)

lexer = lex.lex()
