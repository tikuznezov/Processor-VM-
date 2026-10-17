#ifndef _LOG
#define _LOG

#include <stdio.h>

#ifdef _DEBUG
const char *LOG_FILE_NAME = "LOG.log";
FILE *const log_file = fopen(LOG_FILE_NAME, "w");

#define LOG(name, ...)  fprintf(log_file, name, ## __VA_ARGS__);
#define LOG_FUNC_INFO   LOG("\n[INFO] [%s:%d] (%s) --> started...\n", file_name, line_num, __FUNCTION__)
#define END_FUNC_LOG    LOG("[INFO] (%s) --> completed.\n\n", __FUNCTION__)
#define CRUSH_FUNC_LOG  LOG("[WARN] (%s) --> CRUSHED!!!\n\n", __FUNCTION__)

#else

#define LOG(name, ...)
#define LOG_FUNC_INFO
#define END_FUNC_LOG
#define CRUSH_FUNC_LOG

#endif

#if (_PRT == 1)
    #define PR(name, ...)  if (_PRT) {printf(name, ## __VA_ARGS__);}
    #define PR_FST if (_PRT) {\
                                printf("----------------------------------------------------------------------------------\n"); \
                                printf("- RUN %s:\n\n", __FUNCTION__);}
    #define PR_FED if (_PRT) {\
                                printf("\n- END %s;\n", __FUNCTION__); ClearTerm();                                  \
                                printf("----------------------------------------------------------------------------------\n");}
#else
    #define PR(name, ...)
    #define PR_FST
    #define PR_FED
#endif

// очищает ввод терминала
void ClearTerm()
{
    int symbol = 0;
    while ((symbol != EOF) && (symbol != '\n'))
    {
        symbol = getchar();
    }
}

#endif