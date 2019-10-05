from collections import deque
from typing import List

from pybasic.compiler.parser import TokenType

from .node import ASTNode

TOKEN_TYPE_AST_MAP = {
    1: [
        TokenType.PrintStmt,
    ],

    2: [
        TokenType.Eq,
    ]
}


class AbstractSyntaxTree:
    """An tree structure representing abstracting over the syntax semantics."""

    def __init__(self, root: ASTNode) -> None:
        self._root = root

    # Alternate constructors

    @classmethod
    def from_source_object(cls, source_object: 'SourceObject') -> 'AbstractSyntaxTree':
        root: ASTNode = None
        nodes: List[ASTNode] = deque([ASTNode(token) for token in source_object.tokens])

        while nodes:
            node = nodes.popleft()
            tp = node.token.tp

            if tp is TokenType.PrintStmt:
                node.children.append(nodes.popleft())

            if tp is TokenType.Eq:
                node.children.append(node.popleft())
                node.children.append(node.popleft())

        return cls(nodes)

    # Properties

    @property
    def root(self):
        return self._root

    # Public api

    def optimize(self, opt_level: int) -> None:
        pass
