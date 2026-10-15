#ifndef _vm
#define _vm

typedef double stack_elem;
#define PRINT_ELEM_T(x) printf("%lg", x);

#define _PRT 0 //   zero is off \ other is on
#define CRUSH_BIN NAN

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
    JB,  // <
    JBE, // <=
    JE,  // ==
    JNE, // !=
    JAE, // >=
    JA,  // >
    ADD,
    SUB,
    MUL,
    DIV,
    OUT,
    HLT
};

enum ERR_CODES
{
    PUSH_CRUSH,
    POP_CRUSH,
    POPr_CRUSH,
    PUSHr_CRUSH,
    JMP_CRUSH,
    ADD_CRUSH,
    SUB_CRUSH,
    MUL_CRUSH,
    DIV_CRUSH,
    OUT_CRUSH,
    HLT_CRUSH
};

const int REG_COUNT = 4;

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
int InstJB   (Processor *proc);
int InstJBE  (Processor *proc);
int InstJE   (Processor *proc);
int InstJNE  (Processor *proc);
int InstJAE  (Processor *proc);
int InstJA   (Processor *proc);
int InstADD  (Processor *proc);
int InstSUB  (Processor *proc);
int InstMUL  (Processor *proc);
int InstDIV  (Processor *proc);
int InstOUT  (Processor *proc);
int InstHLT  (Processor *proc);

size_t SizeofFile(const char *file_name);


// CONSTANTS
//-------------------------------------------------------------------------------------------------------------------------------------------------

const int COUNT_OF_INSTRUCTIONS = 18;

const Instruction Instructions[] = 
{
    {.name = "UNKNOWN", .num = UNKNOWN, .execute = InstUNKNOWN},

    {.name = "PUSH",  .num = PUSH,  .execute = InstPUSH },
    {.name = "POP",   .num = POP,   .execute = InstPOP  },
    {.name = "POPr",  .num = POPr,  .execute = InstPOPr },
    {.name = "PUSHr", .num = PUSHr, .execute = InstPUSHr},
    {.name = "JMP",   .num = JMP,   .execute = InstJMP  },
    {.name = "JB",    .num = JB,    .execute = InstJB   },
    {.name = "JBE",   .num = JBE,   .execute = InstJBE  },
    {.name = "JE",    .num = JE,    .execute = InstJE   },
    {.name = "JNE",   .num = JNE,   .execute = InstJNE  },
    {.name = "JAE",   .num = JAE,   .execute = InstJAE  },
    {.name = "JA",    .num = JA,    .execute = InstJA   },
    {.name = "ADD",   .num = ADD,   .execute = InstADD  },
    {.name = "SUB",   .num = SUB,   .execute = InstSUB  },
    {.name = "MUL",   .num = MUL,   .execute = InstMUL  },
    {.name = "DIV",   .num = DIV,   .execute = InstDIV  },
    {.name = "OUT",   .num = OUT,   .execute = InstOUT  },
    {.name = "HLT",   .num = HLT,   .execute = InstHLT  }
};

const size_t BUF_SIZE = 1000;

const int MAX_INSTRUCTION_CALLS = 500;

#endif