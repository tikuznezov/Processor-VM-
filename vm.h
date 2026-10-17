#ifndef _vm
#define _vm

typedef double stack_elem;
#define PRINT_ELEM_T(x) printf("%lg", x);
#define SCANF_ELEM_T(x) scanf("%lg", &x);

#define _PRT 1 //   zero is off \ other is on
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
    UNKNOWN = 0, // 0
    PUSH,        // 1
    POP,         // 2
    POPr,        // 3
    PUSHr,       // 4
    JMP,         // 5
    JB,          // 6  (<)
    JBE,         // 7  (<=)
    JE,          // 8  (==)
    JNE,         // 9  (!=)
    JAE,         // 10 (>=)
    JA,          // 11 (>)
    ADD,         // 12
    SUB,         // 13
    MUL,         // 14
    DIV,         // 15
    OUT,         // 16
    HLT,         // 17
    IN,          // 18
    CALL,        // 19
    RET          // 20
};

enum ERR_CODES
{
    UNKNOWN_CRUSH,
    PUSH_CRUSH,
    POP_CRUSH,
    POPr_CRUSH,
    PUSHr_CRUSH,
    JMP_CRUSH,
    JB_CRUSH,
    JBE_CRUSH,
    JE_CRUSH,
    JNE_CRUSH,
    JAE_CRUSH,
    JA_CRUSH,
    ADD_CRUSH,
    SUB_CRUSH,
    MUL_CRUSH,
    DIV_CRUSH,
    OUT_CRUSH,
    HLT_CRUSH,
    IN_CRUSH,
    CALL_CRUSH,
    RET_CRUSH
};

const size_t REG_COUNT = 4;
const size_t MEM_SIZE  = 100;

struct Processor
{
    int *code;
    size_t code_size;
    Stack_t stack;
    Stack_t ret_address;
    size_t pc;
    int reg[REG_COUNT];
    stack_elem RAM[MEM_SIZE];
};

struct Instruction
{
    const char *name;
    int num;
    int (* execute) (Processor *proc);
};

struct File_names
{
    const char * const src;
    const char *t_out;
    const char *b_out;
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
int InstIN   (Processor *proc);
int InstCALL (Processor *proc);
int InstRET  (Processor *proc);

size_t SizeofFile(const char *file_name);

void PR_PROC (Processor *proc);

void ClearTerm();


// CONSTANTS
//-------------------------------------------------------------------------------------------------------------------------------------------------

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
    {.name = "HLT",   .num = HLT,   .execute = InstHLT  },
    {.name = "IN",    .num = IN,    .execute = InstIN   },
    {.name = "CALL",  .num = CALL,  .execute = InstCALL },
    {.name = "RET",   .num = RET,   .execute = InstRET  }
};
const int COUNT_OF_INSTRUCTIONS = sizeof(Instructions) / sizeof(Instruction);


const size_t BUF_SIZE = 1000;

const int MAX_INSTRUCTION_CALLS = 500;

#endif