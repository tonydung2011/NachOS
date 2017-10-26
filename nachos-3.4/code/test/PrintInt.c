#include "syscall.h"

int main()
{
	int number;
	PrintString("Nhap number: ");
	number = ReadInt();
	PrintString("number: ");
	PrintInt(number);
	PrintChar("\n");
        Halt();
	return 0;
}
