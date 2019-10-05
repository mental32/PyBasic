from dataclasses import dataclass
from enum import IntEnum, auto
from collections import namedtuple
from typing import Tuple


class TokenType(IntEnum):
    Whitespace = auto()
    Semicolon = auto()
    Comma = auto()
    Assign = auto()
    Eq = auto()
    Neq = auto()
    Add = auto()
    Sub = auto()
    Mul = auto()
    Div = auto()
    And = auto()
    Shl = auto()
    Shr = auto()
    Lbr = auto()
    Rbr = auto()
    Newline = auto()

    Ident = auto()
    Integer = auto()
    String = auto()
    EndOfLine = auto()
    EndOfInput = auto()

    PrintStmt = auto()
    EndStmt = auto()
    IfStmt = auto()
    LetStmt = auto()
    ThenStmt = auto()
    GotoStmt = auto()

    DoubleQuote = auto()
    SingleQuote = auto()

    Unknown = auto()

    @classmethod
    def from_body(cls, body: str) -> 'TokenType':
        for left, right in TOKENS.items():
            if body in left:
                return right

        if body.isdigit():
            return TokenType.Integer

        if len(body) >= 2 and ((body[0], body[-1]) == (q, q) for q in '"\''):
            return TokenType.String

        return TokenType.Unknown

@dataclass(frozen=True)
class Token:
    """A recognised lexeme."""
    body: str
    tp: TokenType
    pos: Tuple[str, int, int]

    @property
    def file(self):
        return self.pos[0]

    @property
    def line(self):
        return self.pos[1]

    @property
    def col(self):
        return self.pos[2]

QUOTES = {
    '"': TokenType.DoubleQuote,
    '\'': TokenType.SingleQuote,
}

SIGILS = {
    ' ': TokenType.Whitespace,
    ';': TokenType.Semicolon,
    ',': TokenType.Comma,
    '=': TokenType.Assign,
    '==': TokenType.Eq,
    '!=': TokenType.Neq,
    '+': TokenType.Add,
    '-': TokenType.Sub,
    '*': TokenType.Mul,
    '/': TokenType.Div,
    '&': TokenType.And,
    '<<': TokenType.Shl,
    '>>': TokenType.Shr,
    '(' : TokenType.Lbr,
    ')' : TokenType.Rbr,
    '\n': TokenType.Newline,
    **QUOTES
}


KEYWORDS = {
    'print': TokenType.PrintStmt,
    'PRINT': TokenType.PrintStmt,

    'end': TokenType.EndStmt,
    'END': TokenType.EndStmt,
}


TOKENS = {
    **SIGILS,
    **KEYWORDS
}
