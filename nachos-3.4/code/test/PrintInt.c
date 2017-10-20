#include "syscall.h"
int main()
{
	int number;
	PrintString("Nhap number:");
	number = ReadInt();
	PrintString("number:");
	PrintInt(number);
        Halt();
	return 0;
}
