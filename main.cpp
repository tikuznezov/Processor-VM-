#include "vm.cpp"
#include "log.cpp"

// дед будет просить поменять местами сомножетели в факториале 
// n * f(n-1) --- f(n-1) * n

int main(int argc, char *argv[])
{
    int err = 0;
    Processor proc = {};

    // ReadBinFile("code.bin", &(proc.code), &(proc.code_size));
    ReadTxtFile("com1.txt", &(proc.code), &(proc.code_size));
    err = RunBin(proc);

    if (err != 0)
    {
        if (err != HLT_CRUSH) PRED;
        PR("* Completed 'RunBin'.\n");
        PrintErrorCode(err);
        DEF_COL;
    }

    return 0;
}


// TODO таблица со структурами с командами и двоичный поиск команды по ней
// TODO целочисленные операции можно реализовать умножением double на фиксированное число (1000 например)
// TODO целочисленные команды и double операнды