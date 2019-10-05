from typing import List

from pybasic.compiler.ast import AbstractSyntaxTree

from .token import Token


class SourceObject:
    """A parse tree implementation

    SourceObjects are created from the parser.
    They represent a semantically valid parse tree
    additionally containing metadata such as their
    source file, line and column positions.
    """
    _tokens = None

    def __init__(self, tokens: List[Token]):
        self._tokens = tuple(tokens)

    def __repr__(self) -> str:
        return f'<SourceObject: {"".join([token.body for token in self.tokens])!r}>'

    # Properties

    @property
    def tokens(self):
        return self._tokens

    # Public api

    def to_ast(self) -> AbstractSyntaxTree:
        """Serialize the source object into an :class:`AbstractSyntaxTree`.

        Returns
        -------
        ast : :class:`AbstractSyntaxTree`
            The translated ast.
        """
        return AbstractSyntaxTree.from_source_object(self)
