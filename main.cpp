#include "log.cpp"
#include "vm.cpp"

int main(int argc, char *argv[])
{
    int *buf = NULL;
    size_t buf_size = 0;
    PR("check\n");

    ReadBinFile("com1.txt", &buf, &buf_size);
    stack_elem answ = RunBin(buf, buf_size);

    printf("* Answer: "); PRINT_ELEM_T(answ); printf(";\n");
    return 0;
}