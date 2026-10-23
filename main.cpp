#include "vm.cpp"
#include "log.cpp"


// TODO Керниган и Пайк практика программирования.
// дед будет просить поменять местами сомножетели в факториале 
// n * f(n-1) --- f(n-1) * n

// TODO push [AX+1]

// COMP argc argv
int main(int argc, char *argv[])
{
    // sort hash
    InstHash hash_tab[COUNT_OF_INSTRUCTIONS] = {};
    InitHashT(hash_tab);

    // Init source file names
    File_names fname = {.src   = (argc > 1) ? argv[1] : "src1.asm",
                        .b_out = (argc > 2) ? argv[2] : "code.bin",
                        .t_out = (argc > 3) ? argv[3] : "code.txt",};

    File src = ReadSringsFromFile(fname.src);

    // FIXME label init
    int labels[LABELS_COUNT] = {};
    for (int i = 0; i < LABELS_COUNT; i++)
        labels[i] = -1;

    const int repeat_count = 2;
    for (int a = 0; a < repeat_count; a++)
    {
        AsmReadWrite(&src, fname, labels, hash_tab);
    }

    return 0;
}


// TODO таблица со структурами с командами и двоичный поиск команды по ней
// TODO целочисленные операции можно реализовать умножением double на фиксированное число (1000 например)
// TODO целочисленные команды и double операнды
