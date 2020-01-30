import io
from io import IOBase
from typing import Union
from string import digits, ascii_letters as alpha, punctuation
from enum import IntEnum, auto
from typing import List, Iterator

from pybasic.errors import BadStringLiteral

from .lexeme import Lexeme

__all__ = ("CharGroup", "CharMap", "CHAR_MAP", "CHAR_GROUP_QUOTES", "io_lazy_reader", "lex", "lex_one")


class CharGroup(IntEnum):
    Numeric = auto()
    Alpha = auto()
    SingleQuote = auto()
    DoubleQuote = auto()
    LogicOp = auto()
    MathOp = auto()
    Whitespace = auto()
    Separators = auto()


class CharMap(dict):
    def match(self, body: str) -> CharGroup:
        for group, ident in CHAR_MAP.items():
            if body in group:
                return ident


CHAR_MAP = CharMap(
    {
        digits: CharGroup.Numeric,   # Digits
        alpha: CharGroup.Alpha,      # ascii, english alphabet, lowercase + uppercase
        "'": CharGroup.SingleQuote,  # single quotation mark
        '"': CharGroup.DoubleQuote,  # double quatation mark
        "<>!=": CharGroup.LogicOp,   # logical operators
        "+-*/": CharGroup.MathOp,    # basic mathematical operators
        " ": CharGroup.Whitespace,   # Whitespace
        ";,": CharGroup.Separators,  # Separators
    }
)

CHAR_GROUP_QUOTES = (CharGroup.SingleQuote, CharGroup.DoubleQuote)


def io_lazy_reader(file: io.IOBase) -> Iterator[str]:
    """An iterator that lazily reads lines from a file-like object.

    Parameters
    ----------
    file : :class:`io.IOBase`
        A file-like object containing :class:`str` to read from.
    """
    buf = []

    while True:
        byte = file.read(1)

        if not byte:
            if buf:
                yield "".join(buf).strip()
            return

        if byte == "\n":
            yield "".join(buf).strip()
            buf.clear()

        buf.append(byte)


def lex_one(line: str, line_index: int, file_path: str) -> List[Lexeme]:
    """Lex a single source line.

    >>> lex_one("10 print (2 + 3) / 10", 1, "foo")
    ... [Lexeme("10", ("foo", 1, 1)), Lexeme(" ", ("foo", 1, 3)), Lexeme("print", ("foo", 1, 4)), ...]

    Parameters
    ----------
    line : :class:`str`
        The line to lex.
    line_index : :class:`int`
        The index of the line.
    file_path : :class:`str`
        The path of the file where the line originates from.
    """
    if not line:
        return []

    column = 1
    stream = iter(line)
    lexeme_body = char = next(stream)
    lexeme_group = CHAR_MAP.match(char)

    lexemes = []

    for char in stream:
        char_group = CHAR_MAP.match(char)

        if lexeme_group == char_group or char_group is CharGroup.Numeric and lexeme_group is CharGroup.Alpha:
            lexeme_body += char
            continue

        column += len(lexeme_body)
        lexemes.append(Lexeme(lexeme_body, file_path, line_index, column - len(lexeme_body)))

        lexeme_group = char_group
        lexeme_body = char

        if char_group in CHAR_GROUP_QUOTES:
            while True:
                try:
                    _char = next(stream)
                    _char_group = CHAR_MAP.match(_char)
                except StopIteration:
                    raise BadStringLiteral(
                        f"Invalid EOL while lexing string literal, line={line_index} col={column}"
                    )

                lexeme_body += _char

                if _char_group == char_group:
                    break

            column += len(lexeme_body)
            lexemes.append(Lexeme(lexeme_body, file_path, line_index, column))
            lexeme_body = ''
            lexeme_group = None

    if lexeme_body:
        column += len(lexeme_body)
        lexemes.append(Lexeme(lexeme_body, file_path, line_index, column))

    return list(filter((lambda lexeme: bool(lexeme.body)), lexemes))


def lex(source: Union[IOBase, str], file_path: str):
    if isinstance(source, str):
        source = StringIO(source)

    stream = io_lazy_reader(source)

    return [lex_one(source_line, line_index, file_path) for line_index, source_line in enumerate(stream)]
