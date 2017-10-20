#include "syscall.h"
int main()
{
	char character;
	PrintString("Nhap character:");
	character = ReadChar();
	PrintString("character:");
	PrintChar(character);
        Halt();
	return 0;
}
