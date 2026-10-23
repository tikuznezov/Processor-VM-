#ifndef _vm
#define _vm

typedef double stack_elem;
#define PRINT_ELEM_T(x) printf("%lg", x);
#define SCANF_ELEM_T(x) scanf("%lg", &x);

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
    UNKNOWN = 0, // 0
    PUSH,        // 1
    POP,         // 2
    JMP,         // 3
    JB,          // 4  (<)
    JBE,         // 5  (<=)
    JE,          // 6  (==)
    JNE,         // 7  (!=)
    JAE,         // 8  (>=)
    JA,          // 9  (>)
    ADD,         // 10
    SUB,         // 11
    MUL,         // 12
    DIV,         // 13
    OUT,         // 14
    HLT,         // 15
    IN,          // 16
    CALL,        // 17
    RET,         // 18
    SQRT         // 19
};

enum ERR_CODES
{
    UNKNOWN_COMMAND = -1,// -1

    UNKNOWN_CRUSH, // 0
    PUSH_CRUSH,    // 1
    POP_CRUSH,     // 2
    JMP_CRUSH,     // 3
    JB_CRUSH,      // 4
    JBE_CRUSH,     // 5
    JE_CRUSH,      // 6
    JNE_CRUSH,     // 7
    JAE_CRUSH,     // 8
    JA_CRUSH,      // 9
    ADD_CRUSH,     // 10
    SUB_CRUSH,     // 11
    MUL_CRUSH,     // 12
    DIV_CRUSH,     // 13
    OUT_CRUSH,     // 14
    HLT_CRUSH,     // 15
    IN_CRUSH,      // 16
    CALL_CRUSH,    // 17
    RET_CRUSH,     // 18
    SQRT_CRUSH     // 19
};

const size_t REG_COUNT = 4;
const size_t MEM_SIZE  = 100;

struct Processor
{
    char *code;
    size_t code_size;
    Stack_t stack;
    Stack_t ret_address;
    size_t pc;
    stack_elem reg[REG_COUNT];
    stack_elem RAM[MEM_SIZE];
};

struct Instruction
{
    const char *name;
    char num;
    int (* execute) (Processor *proc);
};

struct File_names
{
    const char * const src;
    const char * const t_out;
    const char * const b_out;
};


// FUNCTOINS
//-------------------------------------------------------------------------------------------------------------------------------------------------


int InstUNKNOWN(Processor *proc);

int InstPUSH (Processor *proc);
int InstPOP  (Processor *proc);
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
int InstSQRT (Processor *proc);

size_t SizeofFile(const char *file_name);

void PR_PROC (Processor *proc);

void ClearTerm();

int CompInstUp(const void *a, const void *b);

bool IsEqual(double arg1, double arg2);


// CONSTANTS
//-------------------------------------------------------------------------------------------------------------------------------------------------

const Instruction Instructions[] = 
{
    {.name = "UNKNOWN", .num = UNKNOWN, .execute = InstUNKNOWN},

    {.name = "PUSH",  .num = PUSH,  .execute = InstPUSH },
    {.name = "POP",   .num = POP,   .execute = InstPOP  },
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
    {.name = "RET",   .num = RET,   .execute = InstRET  },
    {.name = "SQRT",  .num = SQRT,  .execute = InstSQRT }
};
const int COUNT_OF_INSTRUCTIONS = sizeof(Instructions) / sizeof(Instruction);


const size_t BUF_SIZE = 1000; // FIXME

const double EPSILON = 0.001;

const int MAX_INSTRUCTION_CALLS = 500;

#endif