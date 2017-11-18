#include "syscall.h"
int main()
{
	if (CreateFile("file") == 0)
            PrintString("mo file thanh cong");
        else
            PrintString("k the mo file");
	return 0;
}
