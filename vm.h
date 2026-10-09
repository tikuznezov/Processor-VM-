#ifndef _vm
#define _vm


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

//-------------------------------------------------------------------------------------------------------------------------------------------------

enum COMMANDS
{
    PUSH = 501,
    ADD,
    DIV,
    OUT,
    HLT,
    SUB,
    UNKNOWN = 13
};

const size_t BUF_SIZE = 1000;



#endif