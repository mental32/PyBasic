from dataclasses import dataclass
from enum import IntEnum, auto
from collections import namedtuple
from typing import Tuple, NamedTuple

__all__ = ("Token", "TokenType")


class TokenType(IntEnum):
    Whitespace = auto()
    Semicolon = auto()
    Colon = auto()
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

    GosubKeyword = auto()
    ClearKeyword = auto()
    InputKeyword = auto()
    SleepKeyword = auto()
    PrintKeyword = auto()
    EndKeyword = auto()
    IfKeyword = auto()
    LetKeyword = auto()
    ThenKeyword = auto()
    GotoKeyword = auto()

    DollarSign = auto()
    DoubleQuote = auto()
    SingleQuote = auto()

    Unknown = auto()

    @classmethod
    def from_body(cls, body: str) -> "TokenType":
        from .grammar import TOKENS

        if body in TOKENS:
            return TOKENS[body]

        if body.isdigit():
            return TokenType.Integer

        same_ends = body[0] == body[-1]
        quoted = body[0] in (['"', "'"])

        if len(body) >= 2 and same_ends and quoted:
            return TokenType.String

        if len(body) >= 1 and body[0].isalpha():
            return TokenType.Ident

        if set(body) == {" "}:
            return TokenType.Whitespace

        raise ValueError(f"Unknown token type: {body!r}")


class Token(NamedTuple):
    """A recognised lexeme."""

    body: str
    tp: TokenType
    file: str
    line: int
    col: int

    @property
    def pos(self):
        return (self.file, self.line, self.col)
