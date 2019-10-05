class BASICTranslationException(Exception):
    """The base exception for all compiler related exception."""


class SyntaxError(BASICTranslationException):
    """Base exception for all syntax related errors."""


class BadStringLiteral(SyntaxError):
    """Raised when there is a bad string literal parsed."""
