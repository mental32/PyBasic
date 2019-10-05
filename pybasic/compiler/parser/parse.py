from typing import List, Iterable

from ..lexer.lexeme import Lexeme

from ..errors import SyntaxError
from .grammar import SYNTAX_ROOT
from .source_object import SourceObject
from .token import Token, TokenType


def parse(source_lexemes: Iterable[Iterable[Lexeme]]) -> List[SourceObject]:

    source_objects = []

    tokens = []

    for lexemes in source_lexemes:
        root = node = SYNTAX_ROOT

        for lexeme in lexemes:
            tp = TokenType.from_body(lexeme.body)

            if tp not in node:
                raise SyntaxError(f'{tp!r}, {lexeme!r}')

            root, node = node, node[tp]

            tokens.append(Token(lexeme.body, tp, lexeme.pos))

        source_objects.append(SourceObject(tokens))
        tokens = []

    return source_objects
