#include "syscall.h"

void main()
{
    char str[255];
    str=ReadString();
    Halt();
}
