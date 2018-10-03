class BadSyntax(Exception):
    def __init__(self, token, source):
        self.__source = source
        self.__token = token
        super().__init__()

    def __str__(self):
        return '\n{source}\n{padding}^'.format(source=self.__source, padding=' ' * self.__source.index(self.__token))
