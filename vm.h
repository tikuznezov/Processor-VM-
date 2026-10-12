#ifndef _vm
#define _vm

typedef double stack_elem;
#define PRINT_ELEM_T(x) printf("%lg", x);

#define _PRT 0 //   zero is off \ other is on

// #define _DEBUG

#include "/Applications/VsСode/Stack/stack.h"

#include     <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include      <time.h>
#include      <math.h>
#include   <stdbool.h>
#include    <assert.h>
#include  <sys/stat.h>
#include    <unistd.h>
#include     <fcntl.h>
#include <sys/types.h>
#include     <ctype.h>
#include    <stdarg.h>


// ENUMs and STRUCTs
//-------------------------------------------------------------------------------------------------------------------------------------------------

enum COMMANDS
{
    UNKNOWN = 0,
    PUSH,
    POP,
    POPr,
    PUSHr,
    JMP,
    ADD,
    SUB,
    MUL,
    DIV,
    OUT,
    HLT
};

struct Processor
{
    int *code;
    size_t code_size;
    Stack_t stack;
    size_t pc;
    int reg[4];
};

struct Instruction
{
    const char *name;
    int num;
    int (* execute) (Processor *proc);
};


// FUNCTOINS
//-------------------------------------------------------------------------------------------------------------------------------------------------


int InstUNKNOWN(Processor *proc);

int InstPUSH (Processor *proc);
int InstPOP  (Processor *proc);
int InstPOPr (Processor *proc);
int InstPUSHr(Processor *proc);
int InstJMP  (Processor *proc);
int InstADD  (Processor *proc);
int InstSUB  (Processor *proc);
int InstMUL  (Processor *proc);
int InstDIV  (Processor *proc);
int InstOUT  (Processor *proc);
int InstHLT  (Processor *proc);


// CONSTANTS
//-------------------------------------------------------------------------------------------------------------------------------------------------

const int COUNT_OF_INSTRUCTIONS = 12;

const Instruction Instructions[] = 
{
    {.name = "UNKNOWN", .num = UNKNOWN, .execute = InstUNKNOWN}, // 0

    {.name = "PUSH",  .num = PUSH,  .execute = InstPUSH }, //  1
    {.name = "POP",   .num = POP,   .execute = InstPOP  }, //  2
    {.name = "POPr",  .num = POPr,  .execute = InstPOPr }, //  3
    {.name = "PUSHr", .num = PUSHr, .execute = InstPUSHr}, //  4
    {.name = "JMP",   .num = JMP,   .execute = InstJMP  }, //  5
    {.name = "ADD",   .num = ADD,   .execute = InstADD  }, //  6
    {.name = "SUB",   .num = SUB,   .execute = InstSUB  }, //  7
    {.name = "MUL",   .num = MUL,   .execute = InstMUL  }, //  8
    {.name = "DIV",   .num = DIV,   .execute = InstDIV  }, //  9
    {.name = "OUT",   .num = OUT,   .execute = InstOUT  }, // 10
    {.name = "HLT",   .num = HLT,   .execute = InstHLT  }, // 11
};

const size_t BUF_SIZE = 1000;

const int MAX_INSTRUCTION_CALLS = 100;

#endif