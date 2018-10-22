#ifndef PYBASIC_PARSER_H
#define PYBASIC_PARSER_H

int bytecode_parse_source(ByteCodeInterpreter *vm, uint8_t *buf, const char *source);

#endif
