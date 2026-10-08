#ifndef _LOG
#define _LOG

#define _PRT 0

#include <stdio.h>

const char *LOG_FILE_NAME = "LOG.log";
FILE *const log_file = fopen(LOG_FILE_NAME, "w");

#define PR(name, ...)  if (_PRT) {printf(name, ## __VA_ARGS__);}
#define PR_FST if (_PRT) {printf("\n- RUN %s:\n", __FUNCTION__);}
#define PR_FED if (_PRT) {printf("- END %s:\n", __FUNCTION__);}

#define LOG(name, ...)  fprintf(log_file, name, ## __VA_ARGS__);
#define LOG_FUNC_INFO   LOG("\n[INFO] [%s:%d] (%s) --> started...\n", file_name, line_num, __FUNCTION__)
#define END_FUNC_LOG    LOG("[INFO] (%s) --> completed.\n\n", __FUNCTION__)
#define CRUSH_FUNC_LOG  LOG("[WARN] (%s) --> CRUSHED!!!\n\n", __FUNCTION__)


#endif