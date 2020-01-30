from typing import List

from .token import Token, TokenType

__all__ = ("OPERATORS", "SourceObject")


OPERATORS = {
    TokenType.Eq,
    TokenType.Neq,
    TokenType.Add,
    TokenType.Sub,
    TokenType.Mul,
    TokenType.Div,
    TokenType.And,
    TokenType.Shl,
    TokenType.Shr,
}


class SourceObject:
    """A parse tree implementation

    SourceObjects are created from the parser.
    They represent a semantically valid parse tree
    additionally containing metadata such as their
    source file, line and column positions.
    """
    _tokens = None

    def __init__(self, tokens: List[Token]):
        self.line, _, *_tokens = tokens

        # Transform the parse tree into postfix notation.
        for index, token in enumerate(_tokens):
            if token.tp in OPERATORS:
                _tokens[index - 1], _tokens[index] = token, _tokens[index - 1]

        self._tokens = tuple(_tokens)

    def __repr__(self) -> str:
        return f'<SourceObject: {"".join([token.body for token in self.tokens])!r}>'

    # Properties

    @property
    def tokens(self):
        return tuple(self._tokens)

    # Public api

    def to_ast(self) -> 'AbstractSyntaxTree':
        """Serialize the source object into an :class:`AbstractSyntaxTree`.

        Returns
        -------
        ast : :class:`AbstractSyntaxTree`
            The translated ast.
        """
        from ..ast import AbstractSyntaxTree
        return AbstractSyntaxTree.from_source_object(self)
