// PROCESSOR INSTRUCTIONS
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
    SQRT,        // 19
    DISP         // 20
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
    SQRT_CRUSH,    // 19
    DISP_CRUSH     // 20
};

struct Processor;

/// @brief structure with instruction name, bin code (num), exe function.
struct Instruction
{
    const char *name;
    char num;
    int (* execute) (Processor *proc);
};


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
int InstDISP (Processor *proc);

#ifdef _VM
#define exe(func_name) .execute = func_name
#else 
#define exe(func_name) 
#endif

/// @brief All instructions array. Used to find command in asm and RunBin.
Instruction Instructions[] = {
    {.name = "UNKNOWN", .num = UNKNOWN, exe(InstUNKNOWN)},

    {.name = "PUSH",  .num = PUSH,  exe(InstPUSH) },
    {.name = "POP",   .num = POP,   exe(InstPOP)  },
    {.name = "JMP",   .num = JMP,   exe(InstJMP)  },
    {.name = "JB",    .num = JB,    exe(InstJB)   },
    {.name = "JBE",   .num = JBE,   exe(InstJBE)  },
    {.name = "JE",    .num = JE,    exe(InstJE)   },
    {.name = "JNE",   .num = JNE,   exe(InstJNE)  },
    {.name = "JAE",   .num = JAE,   exe(InstJAE)  },
    {.name = "JA",    .num = JA,    exe(InstJA)   },
    {.name = "ADD",   .num = ADD,   exe(InstADD)  },
    {.name = "SUB",   .num = SUB,   exe(InstSUB)  },
    {.name = "MUL",   .num = MUL,   exe(InstMUL)  },
    {.name = "DIV",   .num = DIV,   exe(InstDIV)  },
    {.name = "OUT",   .num = OUT,   exe(InstOUT)  },
    {.name = "HLT",   .num = HLT,   exe(InstHLT)  },
    {.name = "IN",    .num = IN,    exe(InstIN)   },
    {.name = "CALL",  .num = CALL,  exe(InstCALL) },
    {.name = "RET",   .num = RET,   exe(InstRET)  },
    {.name = "SQRT",  .num = SQRT,  exe(InstSQRT) },
    {.name = "DISP",  .num = DISP,  exe(InstDISP) }
};
const int COUNT_OF_INSTRUCTIONS = sizeof(Instructions) / sizeof(Instruction);
