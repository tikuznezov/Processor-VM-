#include "log.cpp"
#include "vm.cpp"

int main(int argc, char *argv[])
{
    int *buf = NULL;
    size_t buf_size = 0;

    ReadBinFile("com1.txt", &buf, &buf_size);
    stack_elem answ = RunBin(buf, buf_size);

    ANSW(answ);

    return 0;
}


// TODO таблица со структурами с командами и двоичный поиск команды по ней
// TODO целочисленные операции можно реализовать умножением double на фиксированное число (1000 например)
// TODO целочисленные команды и double операнды