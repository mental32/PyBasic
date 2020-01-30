"""A hard grammar mapping for the GW BASIC dialect."""

from .token import TokenType


KEYWORDS = {
    "end": TokenType.EndKeyword,
    "print": TokenType.PrintKeyword,
    "let": TokenType.LetKeyword,
    "if": TokenType.IfKeyword,
    "then": TokenType.ThenKeyword,
    "goto": TokenType.GotoKeyword,
    "sleep": TokenType.SleepKeyword,
    "input": TokenType.InputKeyword,
    "cls": TokenType.ClearKeyword,
    "clear": TokenType.ClearKeyword,
    "gosub": TokenType.GosubKeyword,
}

for name, tp in KEYWORDS.copy().items():
    KEYWORDS[name.upper()] = tp

QUOTES = {'"': TokenType.DoubleQuote, "'": TokenType.SingleQuote}

SIGILS = {
    " ": TokenType.Whitespace,
    ";": TokenType.Semicolon,
    ":": TokenType.Colon,
    ",": TokenType.Comma,
    "=": TokenType.Assign,
    "==": TokenType.Eq,
    "!=": TokenType.Neq,
    "+": TokenType.Add,
    "-": TokenType.Sub,
    "*": TokenType.Mul,
    "/": TokenType.Div,
    "&": TokenType.And,
    "<<": TokenType.Shl,
    ">>": TokenType.Shr,
    "(": TokenType.Lbr,
    ")": TokenType.Rbr,
    "\n": TokenType.Newline,
    "$": TokenType.DollarSign,
    **QUOTES,
}

TOKENS = {**SIGILS, **KEYWORDS}


AST_EXPRESSION = {TokenType.Add: {}, TokenType.Semicolon: None, TokenType.EndOfLine: {}}

AST_INT = {TokenType.Integer: None, TokenType.Ident: None, TokenType.String: None, TokenType.Whitespace: None}

AST_INT_EXPR = {
    TokenType.Add: AST_INT,
    TokenType.Sub: AST_INT,
    TokenType.Mul: AST_INT,
    TokenType.Div: AST_INT,
    TokenType.And: AST_INT,
    TokenType.Shl: AST_INT,
    TokenType.Shr: AST_INT,
    TokenType.Semicolon: None,
    TokenType.EndOfLine: {},
    TokenType.Whitespace: None,
}

AST_INT[TokenType.Whitespace] = AST_INT
AST_INT_EXPR[TokenType.Whitespace] = AST_INT_EXPR

AST_INT[TokenType.String] = AST_INT_EXPR
AST_INT[TokenType.Integer] = AST_INT_EXPR
AST_INT[TokenType.Ident] = AST_INT_EXPR

AST_EXPRESSION = {
    TokenType.Integer: AST_INT_EXPR,
    TokenType.String: None,
    TokenType.Ident: None,
    TokenType.Whitespace: None,
    TokenType.EndOfLine: {},
    TokenType.ThenKeyword: {TokenType.Whitespace: {TokenType.GotoKeyword: None}},
}

AST_EXPRESSION[TokenType.String] = AST_INT_EXPR

AST_EXPRESSION[TokenType.ThenKeyword][TokenType.Whitespace][
    TokenType.GotoKeyword
] = AST_EXPRESSION

AST_EXPRESSION[TokenType.Whitespace] = AST_EXPRESSION

AST_LOGIC_EXPR = {TokenType.Assign: AST_EXPRESSION}

AST_EXPRESSION.update(AST_EXPRESSION)

AST_EXPRESSION.update(AST_LOGIC_EXPR)

AST_IDENTIFIER = {
    TokenType.DollarSign: AST_EXPRESSION,
    TokenType.Add: AST_EXPRESSION,
    TokenType.Sub: AST_EXPRESSION,
    TokenType.Mul: AST_EXPRESSION,
    TokenType.Div: AST_EXPRESSION,
    TokenType.And: AST_EXPRESSION,
    TokenType.Shl: AST_EXPRESSION,
    TokenType.Shr: AST_EXPRESSION,
    TokenType.Semicolon: None,
    TokenType.EndOfLine: {},
    TokenType.Whitespace: None,
}

AST_IDENTIFIER[TokenType.Whitespace] = AST_EXPRESSION

AST_EXPRESSION[TokenType.Ident] = AST_IDENTIFIER


AST_IDENTIFIER[TokenType.Semicolon] = AST_EXPRESSION

AST_EXPRESSION[TokenType.Semicolon] = AST_EXPRESSION
AST_INT_EXPR[TokenType.Semicolon] = AST_EXPRESSION

AST_COMMA_SEPARATED_VARS = {
    TokenType.Ident: {
        TokenType.DollarSign: None,
        TokenType.EndOfLine: {},
        TokenType.Comma: None,
    },
    TokenType.EndOfLine: {},
}

AST_COMMA_SEPARATED_VARS[TokenType.Ident][TokenType.Comma] = AST_COMMA_SEPARATED_VARS
AST_COMMA_SEPARATED_VARS[TokenType.Ident][
    TokenType.DollarSign
] = AST_COMMA_SEPARATED_VARS

AST_INPUT = {
    TokenType.String: {
        TokenType.Semicolon: AST_COMMA_SEPARATED_VARS,
        TokenType.Comma: AST_COMMA_SEPARATED_VARS,
    },
    **AST_COMMA_SEPARATED_VARS,
}

AST_COLON_SEP = {TokenType.Semicolon: None, TokenType.Colon: None}


SYNTAX_ROOT = SYNTAX_BASE = {
    TokenType.Integer: {
        TokenType.Whitespace: {
            TokenType.String: AST_EXPRESSION,
            TokenType.Integer: AST_INT_EXPR,
            TokenType.InputKeyword: {
                TokenType.Whitespace: {TokenType.Semicolon: AST_INPUT, **AST_INPUT}
            },
            TokenType.PrintKeyword: {
                TokenType.Whitespace: AST_EXPRESSION,
                TokenType.Colon: None,
                TokenType.Semicolon: None,
            },
            TokenType.EndKeyword: {TokenType.Whitespace: {}, TokenType.EndOfLine: {}},
            TokenType.IfKeyword: {TokenType.Whitespace: AST_EXPRESSION},
            TokenType.LetKeyword: {
                TokenType.Whitespace: {
                    TokenType.Ident: {
                        TokenType.Assign: AST_EXPRESSION,
                        TokenType.Whitespace: None,
                    },
                    TokenType.Whitespace: None,
                }
            },
            TokenType.GotoKeyword: {
                TokenType.Whitespace: {TokenType.Integer: TokenType.EndOfLine}
            },
            TokenType.SleepKeyword: {TokenType.Whitespace: AST_EXPRESSION},
            TokenType.ClearKeyword: {TokenType.EndOfLine: None},
            TokenType.GosubKeyword: {TokenType.Whitespace: {TokenType.Integer: {}}},
        }
    },

    TokenType.Whitespace: {
        TokenType.EndOfLine: None,
        TokenType.Whitespace: None
    },
}

SYNTAX_ROOT[TokenType.Whitespace][TokenType.Whitespace] = SYNTAX_ROOT[TokenType.Whitespace]

_BASE = SYNTAX_BASE[TokenType.Integer][TokenType.Whitespace]

_BASE[TokenType.LetKeyword][TokenType.Whitespace][TokenType.Whitespace] = SYNTAX_ROOT[TokenType.Integer][TokenType.Whitespace][TokenType.LetKeyword][TokenType.Whitespace]
_BASE[TokenType.LetKeyword][TokenType.Whitespace][TokenType.Ident][TokenType.Whitespace] = _BASE[TokenType.LetKeyword][TokenType.Whitespace][TokenType.Ident]

_BASE[TokenType.PrintKeyword][TokenType.Colon] = SYNTAX_ROOT[TokenType.Integer][TokenType.Whitespace]
_BASE[TokenType.PrintKeyword][TokenType.Semicolon] = SYNTAX_ROOT[TokenType.Integer][TokenType.Whitespace]

AST_EXPRESSION.update(
    {
        TokenType.Semicolon: SYNTAX_ROOT[TokenType.Integer][TokenType.Whitespace],
        TokenType.Colon: SYNTAX_ROOT[TokenType.Integer][TokenType.Whitespace],
    }
)
