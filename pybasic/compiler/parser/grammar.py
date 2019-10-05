"""A hard grammar mapping for the GW BASIC dialect."""

from .token import TOKENS, TokenType


KEYWORDS = (
    'end',
    'print',
    'let',
    'if',
    'then',
    'goto',
    'def',
)

AST_STR_EXPR = {
    TokenType.Add: {},
    TokenType.Semicolon: None,
    TokenType.EndOfLine: {},
}

AST_INT = {
    TokenType.Integer: None,
    TokenType.Ident: None,
}

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
}

AST_INT[TokenType.Integer] = AST_INT_EXPR
AST_INT[TokenType.Ident] = AST_INT_EXPR

AST_EXPRESSION = {
    TokenType.Integer: AST_INT_EXPR,
    TokenType.String: AST_STR_EXPR,
    TokenType.Semicolon: {},
    TokenType.Ident: None,
    TokenType.EndOfLine: {}
}

AST_IDENTIFIER = {
    TokenType.Add: AST_EXPRESSION,
    TokenType.Sub: AST_EXPRESSION,
    TokenType.Mul: AST_EXPRESSION,
    TokenType.Div: AST_EXPRESSION,
    TokenType.And: AST_EXPRESSION,
    TokenType.Shl: AST_EXPRESSION,
    TokenType.Shr: AST_EXPRESSION,
    TokenType.Semicolon: None,
    TokenType.EndOfLine: {},
}

AST_EXPRESSION[TokenType.Ident] = AST_IDENTIFIER
AST_EXPRESSION[TokenType.Semicolon] = AST_EXPRESSION


AST_IDENTIFIER[TokenType.Semicolon] = AST_EXPRESSION

AST_STR_EXPR[TokenType.Semicolon] = AST_EXPRESSION
AST_INT_EXPR[TokenType.Semicolon] = AST_EXPRESSION

AST_STR_EXPR[TokenType.Add][TokenType.String] = AST_STR_EXPR
AST_STR_EXPR[TokenType.Add][TokenType.Ident] = AST_STR_EXPR


SYNTAX_ROOT = SYNTAX_BASE = {
    TokenType.Integer: {
        TokenType.Whitespace: {
            TokenType.String: AST_STR_EXPR,
            TokenType.Integer: AST_INT_EXPR,

            TokenType.PrintStmt: {
                TokenType.Whitespace: AST_EXPRESSION,
            },

            TokenType.EndStmt: {
                TokenType.Whitespace: {},
                TokenType.EndOfLine: {},
            },

            TokenType.IfStmt: {
                TokenType.Whitespace: AST_EXPRESSION,
                TokenType.ThenStmt: {
                    TokenType.Whitespace: {
                        TokenType.GotoStmt: {
                            TokenType.Whitespace: {
                                TokenType.Integer
                            }
                        }
                    }
                }
            },

            TokenType.LetStmt: {
                TokenType.Whitespace: {
                    TokenType.Ident: {
                        TokenType.Assign: AST_EXPRESSION
                    }
                }
            },

            TokenType.GotoStmt: {
                TokenType.Whitespace: {
                    TokenType.Integer: TokenType.EndOfLine
                }
            }
        }
    }
}
