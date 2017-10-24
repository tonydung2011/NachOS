#include "syscall.h"
int main()
{
	char str[255];
	PrintString("Nhap string:");
	ReadString(str, 255);
	PrintString("string:");
	PrintInt(str);
	PrintChar(" ");
        Halt();
	return 0;
}
