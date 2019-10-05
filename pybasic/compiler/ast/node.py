from dataclasses import dataclass, field

from pybasic.compiler.parser import Token


@dataclass(frozen=True)
class ASTNode:
    """A node belonging to an AST."""
    token: Token
    children: List['ASTNode'] = field(default_factory=list)
