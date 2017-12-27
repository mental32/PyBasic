'''
MIT License

Copyright (c) 2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
'''
##
import logging
import random
import time
import sys
import os
##
try:
    import msvcrt
except ModuleNotFoundError:
    pass
##


class Interpreter:
    def __init__(self):
        '''
        '''
        self.script = {}
        self.memory = {}
        self.split_statement_for = '+ - / * , < > = ! ;'.split()
        self.arithmetic_operators = '+ - / * < > = !'.split()
        self.repl_commands = 'list new old run'.split()

    def _sort_script(self):
        """Sort the script by increasing key value."""
        self.script = {i: self.script[i] for i in sorted(self.script)}

    def is_number(self, string):
        """"""
        if type(string) is int or type(string) is float:
            return True
        elif string.isdigit():
            return True
        else:
            try:
                float(string)
                return True
            except:
                return False

    def _get_number(self, string):
        """"""
        if self.is_number(string):
            return int(float(string)) if float(string).is_integer() else float(string)
        else:
            return string

    def _get_long(self, iterable, predicate):
        """"""
        i = 0
        for c in iterable:
            if not predicate(c):
                break
            i += 1
        return iterable[0:i]

    def _get(self, iterable, predicate):
        """"""
        if len(iterable) > 20:
            return self._get_long(iterable, predicate)
        else:
            a = ''
            for c in iterable:
                if predicate(c):
                    a += c
                else:
                    break
            return a

    def _compile_statement(self, operands):
        """"""
        out, acc, quoted = [[]], '', False
        for i, c in enumerate(operands):
            if c == '"':
                out[-1].append(acc+'"') if quoted else (out[-1].append(acc) if acc else None)
                quoted = not quoted
                acc = '"' if quoted else ''

            elif quoted:
                acc += c

            elif c in self.split_statement_for:
                if c == ';':
                    out[-1].append(self._get_number(acc)) if acc != '' else False
                    out.append([])
                elif acc in self.memory:
                    # refrence is in memory
                    # function or variable
                    if callable(self.memory[acc]):
                        pass
                    else:
                        if type(self.memory[acc]) is int or type(self.memory[acc]) is float:
                            out[-1].append(self._get_number(self.memory[acc]))
                        else:
                            out[-1].append('"{var}"'.format(var=self.memory[acc]))
                    out[-1].append(c)
                else:
                    out[-1].append(self._get_number(acc))
                    out[-1].append(c)
                acc = ''

            elif c == ' ':
                continue

            else:
                acc += c
        else:
            if acc in self.memory:
                var = self.memory[acc]
                out[-1].append('"{var}"'.format(var=var) if type(var) is str else var)
            else:
                out[-1].append(self._get_number(acc))

        for c, i in enumerate(out):
            for d, x in enumerate(i):
                if x == "":
                    del out[c][d]
        return out

    def _eval(self, opers):
        """"""
        out = self._compile_statement(opers)
        if len(out) > 1:
            return ' '.join(str(eval(''.join(str(_) for _ in i))) for i in out)
        else:
            return eval(''.join(str(_) for _ in out[0]))

    def _compile_instruction(self, raw_instruction):
        line = self._get(raw_instruction, lambda f: f.isdigit())
        opcode = self._get(raw_instruction[len(line):].strip(), lambda f: f not in  (' ', '(', ')'))
        if opcode in ('END', 'end'):
            op = str(None)
        else:
            op = raw_instruction[len(raw_instruction[:raw_instruction.index(opcode)])+len(opcode):].strip()

        if line:
            return int(line), opcode, op
        else:
            return -1, opcode, op

    def _execute_instruction(self, instruction):
        l, op, oper = instruction
        if op == 'print':
            print(' '.join(str(_) for _ in [self._eval(oper)]))

        elif op == 'if':
            b = list(reversed(oper))
            for i, c in enumerate(b):
                if b[i:i+4] == list('neht'):
                    break
            ln = int(oper[-i:].strip())
            o = self._eval(oper[:-(i+4)].strip())
            if o:
                return ln
            else:
                return None

        elif op == 'let':
            v, e = oper.split('=')
            v, e = v.strip(), e.strip()
            try:
                end = self._eval(e)
            except:
                print('Error!')
            self.memory[v] = end
            self.memory['_memory'] = self.memory

        elif op == 'goto':
            return int(oper)

        elif op == 'show':
            if oper.strip():
                print(self.memory[oper.strip()])

        elif op == 'input':
            data = input()
            var = oper.strip().split()
            for v in var:
                self.memory[v] = data

        elif op == 'inkey':
            var = oper.strip().split()

            while True:
                if msvcrt.kbhit():
                    char = msvcrt.getch().decode()
                    break

            for v in var:
                self.memory[v] = char

        return None

    def load_file(self, fp):
        if os.path.isfile(fp):
            with open(fp) as file:
                contents = file.readlines()
    
            c = lambda line: self._compile_instruction(line.strip().strip('\n'))
            converted = [c(line) for line in contents if all(c(line))]
            self.script = {c[0]: (c[1], c[2]) for c in converted}
            self._sort_script()
            self.memory['_script'] = self.script
        else:
            raise FileNotFoundError

    def _execute_loop(self):
        INS = tuple(self.script)
        CIR = NIR = PC = 0
        JUMP = False

        while True:
            CIR = (NIR if JUMP else INS[PC])

            try:
                NIR = (INS[INS.index(NIR)+1] if JUMP else INS[PC+1])
            except Exception as e:
                NIR = None
            finally:
                if not NIR:
                    break

            inner = self.script[CIR]
            PC += 1
            GOTO = self._execute_instruction((CIR, *inner))
            NIR = GOTO or NIR
            if GOTO:
                JUMP = True

    def repl(self, argv):
        print('Dartmouth DTSS PyBASIC')
        for file in argv:
            try:
                self.load_file(file)
            except Exception as e:
                pass
        print('READY')
        try:
            while True:
                uin = input('> ')
                a = self._get(uin.strip(), lambda f: f != ' ')
                if a in self.repl_commands:
                    if a == 'list':
                        print('\n'.join(f'{a} {self.script[a][0]} {self.script[a][1]}' for a in self.script))
                    elif a == 'new':
                        self.script = {}
                        if uin.startswith('new ') and len(uin) > 4:
                            open(uin[4:], 'w').close()
                    elif a == 'old':
                        try:
                            self.load_file(uin[4:])
                        except FileNotFoundError:
                            print(f'File not found! ({uin[4:]})')
                    elif a == 'run':
                        self._execute_loop()
                else:
                    x = self._compile_instruction(uin)
                    if x[0] == -1:
                        try:
                            self._execute_instruction(x)
                        except Exception as e:
                            print(str(e))
                    else:
                        self.script[x[0]] = x[1], x[2]
                        self._sort_script()
        finally:
            pass


interpreter = Interpreter()

if __name__ == '__main__':
    interpreter.repl(sys.argv[1:])
