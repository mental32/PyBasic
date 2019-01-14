from .grammar import KEYWORDS

from .tokens import (
    CHAR_ENUM, 
    TOKENS,
    _separator,
    _speech_tokens,
    _token_integer,
    _token_ident,
    _token_string,
)

_speech_tokens = tuple(_speech_tokens.values())

def _type(token):
    if token[0] in ('\'', '"'):
        return _token_string

    elif token.isdigit():
        return _token_integer

    elif token in TOKENS:
        return TOKENS[token]

    if sum(bool(ord(char) & 32) for char in token) == 0:
        token = token.lower()

    if token in KEYWORDS:
        return KEYWORDS[token]

    return _token_ident

def _decode(char):
    for group, num in CHAR_ENUM.items():
        if char in group:
            return num
    return -1


def tokenize(source):
    for line in source.strip().split('\n'):
        out = []

        if not line:
            continue

        code = _code = _decode(line.strip()[0])
        y, i = [line[0]], 1

        skip = force = False
        release = None

        for char in line[1:].strip():
            code = _decode(char)

            if skip:
                y += [line[i]]

                if code == release:
                    skip = False
                    release = None

                    out.append((''.join(y[1:-1]), _token_string))

                    _code, y = code, []

                i += 1
                continue

            elif code in _speech_tokens:
                skip = True
                release = code
                force = True

            elif code == _separator:
                force = True

            if (code != _code and y) or force:
                data = ''.join(y).strip()

                if data:
                    out.append((data, _type(data)))

                if code == _separator:
                    out.append((line[i], _type(line[i])))
                else:
                    y = [line[i]]

                if force:
                    force = False
            else:
                y += [line[i]]

            i += 1
            _code = code

        data = ''.join(y).strip()

        if data:
            out.append((data, _type(data)))

        yield out
