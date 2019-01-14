from string import digits, ascii_letters as alpha, punctuation

_c = chr

TOKENS = {
    ';': 'sep_semicolon',
    ',': 'sep_comma',
    '=': 'assign',
    '==': 'bin_eq',
    '!=': 'bin_neq',
    '+': 'bin_add',
    '-': 'bin_sub',
    '*': 'bin_mul',
    '/': 'bin_div',
    '&': 'bin_and',
    '<<': 'bin_lshift',
    '>>': 'bin_rshift',
    '(' : 'lbracket',
    ')' : 'rbracket',
    None: 'identifier',
    int:  'integer',
    str:  'string',
    ...:  'eol',
}

CHAR_ENUM = {
    digits: 0, # Digits
    alpha : 1, # ascii, english alphabet, lowercase + uppercase
    _c(39): 2, # single quotation mark
    _c(34): 3, # double quatation mark
    '<>!=': 4, # logical operators
    '+-*/': 5, # basic mathematical operators
    ' '   : 6, # Whitespace
    ';,'  : 7, # Separators
}

# Helper aliases

_speech_tokens = {
    '\'': CHAR_ENUM['\''],
    '\"': CHAR_ENUM['\"']
}

_separator = CHAR_ENUM[';,']

_token_string  = TOKENS[str]
_token_integer = TOKENS[int]
_token_ident   = TOKENS[None]
