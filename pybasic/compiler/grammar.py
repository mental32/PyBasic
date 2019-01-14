from .tokens import TOKENS


# Alias the tokens to this module scope
_cleanup = set()
for token in (*TOKENS.values(),):
    fmt = '_token_%s' % token
    globals()[fmt] = token
    _cleanup.add(fmt)
else:
    _cleanup = tuple(_cleanup)

KEYWORDS = {
    'end' : 'end_stmt',
    'print': 'print_stmt',
    'let': 'let_stmt',
    'if': 'if_stmt',
    'then': 'then_stmt',
    'goto': 'goto_stmt',
    'def': 'def_stmt',
}

_AST_STR_EXPR = {
    _token_bin_add: {},
    _token_sep_semicolon: None,
    _token_eol: {},
}

_AST_INT = {
    _token_integer: None,
    _token_identifier: None,
}

_AST_INT_EXPR = {
    _token_bin_add: _AST_INT,
    _token_bin_sub: _AST_INT,
    _token_bin_mul: _AST_INT,
    _token_bin_div: _AST_INT,
    _token_bin_and: _AST_INT,
    _token_bin_lshift: _AST_INT,
    _token_bin_rshift: _AST_INT,
    _token_sep_semicolon: None,
    _token_eol: {},
}

_AST_INT[_token_integer] = _AST_INT_EXPR
_AST_INT[_token_identifier] = _AST_INT_EXPR

_AST_EXPRESSION = {
    _token_integer: _AST_INT_EXPR,
    _token_string: _AST_STR_EXPR,
    _token_sep_semicolon: {},
    _token_identifier: None,
    _token_eol: {}
}

_AST_IDENTIFIER = {
    _token_bin_add: _AST_EXPRESSION,
    _token_bin_sub: _AST_EXPRESSION,
    _token_bin_mul: _AST_EXPRESSION,
    _token_bin_div: _AST_EXPRESSION,
    _token_bin_and: _AST_EXPRESSION,
    _token_bin_lshift: _AST_EXPRESSION,
    _token_bin_rshift: _AST_EXPRESSION,
    _token_sep_semicolon: None,
    _token_eol: {},
}

_AST_EXPRESSION[_token_identifier] = _AST_IDENTIFIER
_AST_EXPRESSION[_token_sep_semicolon] = _AST_EXPRESSION


_AST_IDENTIFIER[_token_sep_semicolon] = _AST_EXPRESSION

_AST_STR_EXPR[_token_sep_semicolon] = _AST_EXPRESSION
_AST_INT_EXPR[_token_sep_semicolon] = _AST_EXPRESSION

_AST_STR_EXPR[_token_bin_add][_token_string] = _AST_STR_EXPR
_AST_STR_EXPR[_token_bin_add][_token_identifier] = _AST_STR_EXPR


expr = _AST_EXPRESSION
expr_min = expr.copy()

del expr_min['eol']

if_stmt = {

# _token_int [+ - * /] (== != > <)
# _token_str 

    'then_stmt': {
        'goto_stmt': {
            _token_integer: {}
        }
    }
}

SYNTAX_BASE = {
    _token_integer: {
        _token_string: _AST_STR_EXPR,
        _token_integer: _AST_INT_EXPR,
        'print_stmt': _AST_EXPRESSION,

        'end_stmt': {
            'eol': {}
        },

        'if_stmt': if_stmt,

        'let_stmt': {
            _token_identifier: {
                _token_assign: expr_min
            }
        },

        'goto_stmt': {
            _token_integer: {}
        }
    }
}

# Clean up the lose tokens
for var in _cleanup:
    del globals()[var]
