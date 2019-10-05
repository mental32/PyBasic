from typing import Optional


class CodeObject:
    """A concrete translation of a AST with a local symbol table.

    This class is used to merge multiple CodeObjects
    into one final CodeObject that can be assembed into
    a :class:`bytes` object ready to be executed.
    """

    def __init__(self, refrences: Optional[list] = None):
        refrences = refrences or {}
        self._refrences = refrences
        self._body = bytearray()

    @property
    def body(self):
        return bytes(self._body) 

    def merge(self, other) -> None:
        """Merge this instance with another CodeObject.

        .. note ::

            This will mutate the current CodeObject instance.

        Parameters
        ----------
        other : :class:`CodeObject`
            The other CodeObject to merge from.
        """

    def optimize(self, opt_level: int) -> None:
        """Perform low level IR or SSA optimizations

        .. note ::

            This will mutate the current CodeObject instance.

        Parameters
        ----------
        opt_level : :class:`int`
            The optimization level to obey.
        """

    def to_bytes(self) -> bytes:
        """Serialize the CodeObject into a :class:`bytes` object."""
