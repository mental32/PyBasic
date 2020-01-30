from typing import List, Iterable

from pybasic.errors import BASICSyntaxError

from .lexeme import Lexeme
from .grammar import SYNTAX_ROOT
from .source_object import SourceObject
from .token import Token, TokenType
from .lexer import lex

__all__ = ("parse",)


def parse(source: str, filename: str) -> List[SourceObject]:
    source_objects = []
    tokens = []

    for lexemes in lex(source, filename):
        root = node = SYNTAX_ROOT
        rule_history = []

        for lexeme in lexemes:
            tp = TokenType.from_body(lexeme.body)

            if tp not in node:
                source: str = "".join(lexeme.body for lexeme in lexemes)
                valid: str = ", ".join(map(repr, node.keys()))
                raise BASICSyntaxError(tp=tp, bad=lexeme, source=lexemes, valid=tuple(node.keys()), history=rule_history)
                # raise BASICSyntaxError(f'Got: {tp!r}, {lexeme!r}! expected any of {valid!r} ({source!r}) history={rule_history!r}')

            rule_history.append(tp)

            root, node = node, node[tp]

            tokens.append(Token(lexeme.body, tp, *lexeme.pos))

        if not tokens:
            continue

        source_objects.append(SourceObject(tokens))
        tokens = []

    return source_objects
