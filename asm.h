

const size_t MAX_COM_LEN = 10;

const int    UNREAD_VAL  = -27394383;

const int   LABELS_COUNT = 20;


// FUNCTIONS
//-------------------------------------------------------------------------------------------------------------------------------------------------

size_t StrLen(const char *str);

int GetCommand(const char *str);

int GetValue(const char *str);

Instruction GetCom2(const char *command);