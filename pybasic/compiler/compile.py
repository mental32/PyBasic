from io import IOBase
from typing import Union

from .lexer.lex import lex
from .parser.parse import parse
from .code import CodeObject


def compile(source: Union[IOBase, str], opt_level: int = 0) -> bytes:
    """Compile a source string or a file-like object executable code.

    Parameters
    ----------
    source : :class:`typing.Union`[:class:`str`, :class:`io.IOBase`]
        The source to compile.
    """
    final_code_object = CodeObject()

    for source_object in parse(lex(source, 'foo')):
        print(source_object, source_object.tokens)
        print(source_object.to_ast())
        return
        # source_object_ast = source_object.to_ast()
        # source_object_ast.optimize(opt_level)

        # final_code_object.merge(source_object_ast.to_code_object())

    return

    if opt_level:
        final_code_object.optimize(opt_level)

    # If we have any unresolved
    return final_code_object.to_bytes()
