#include "syscall.h"

void main()
{
    char str[255];
    ReadString(str, 255);
    Halt();
    return; 
}
