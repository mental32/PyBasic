from abc import ABCMeta, abstractmethod
from dataclasses import dataclass
from sys import stderr
from typing import List, Tuple


class BASICTranslationException(Exception, metaclass=ABCMeta):
    """The base exception for all compiler related exception."""

    @abstractmethod
    def show(self):
        """Format the error and write it to stderr."""


@dataclass
class BASICSyntaxError(BASICTranslationException):
    """Base exception for all syntax related errors."""

    tp: "TokenType"
    bad: "Lexeme"
    source: List["Lexeme"]
    valid: Tuple["TokenType"]
    history: List["TokenType"]

    def show(self):
        source: str = "".join(lexeme.body for lexeme in self.source)
        history = ", ".join([repr(ex.name) for ex in self.history])

        from .compiler.parser.grammar import TOKENS

        table = dict(zip(TOKENS.values(), TOKENS.keys()))
        expected = ", ".join([repr(table.get(ex, ex.name)) for ex in self.valid])

        file, line, col, = map(str, self.bad.pos)

        file = self.bad.file
        pos = f"{line}:{col}"

        fmt = "\n".join([
            f"error: expected one of: {expected}, found: {self.bad.body!r} ({self.tp.name})",
            f"history: {history}",
            f"--> {file}:{pos}",
            f"{' ' * len(line)} | ",
            f"{line} | {source!s}",
            f"{' ' * len(line)} |"
        ])

        print(fmt, file=stderr)

        # print(f'Got: {self.tp!r}, {self.bad!r}! expected any of {valid!r} ({source!r}) history={self.history!r}', file=stderr)

class BadStringLiteral(BASICSyntaxError):
    """Raised when there is a bad string literal parsed."""
