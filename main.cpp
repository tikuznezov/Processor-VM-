#include "log.cpp"
#include "vm.cpp"

int main(int argc, char *argv[])
{
    Processor proc = {};

    ReadBinFile("com1.txt", &(proc.code), &(proc.code_size));
    RunBin(proc);

    return 0;
}


// TODO таблица со структурами с командами и двоичный поиск команды по ней
// TODO целочисленные операции можно реализовать умножением double на фиксированное число (1000 например)
// TODO целочисленные команды и double операнды