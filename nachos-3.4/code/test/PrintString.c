#include "syscall.h"

int main()
{
	char str[255];
	PrintString("Nhap string:");
	ReadString(str, 255);
	PrintString("string:");
	PrintString(str);
	PrintChar(' ');
        Halt();
	return 0;
}
