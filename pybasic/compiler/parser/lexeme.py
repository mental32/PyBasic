from dataclasses import dataclass
from typing import Tuple, NamedTuple


class Lexeme(NamedTuple):
    """A basic lexical unit.

    >>> Lexeme(body="foo", pos=("bar.py", 1, 1))

    Attributes
    ----------
    body : :class:`str`
        The string body of the lexeme.
    pos : :class:`typing.Tuple`[:class:`str`, :class:`int`, :class`int`]
        A tuple triplet consisting of a string and two integers respectively.
        The string is the filepath from which the lexeme was lex'd from.
        The remaining two ints are the line and column numbers respectively.
    """
    body: str
    file: str
    line: int
    col: int

    @property
    def pos(self):
        return (self.file, self.line, self.col)
