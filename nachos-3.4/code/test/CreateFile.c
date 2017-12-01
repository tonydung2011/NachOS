#include "syscall.h"
int main()
{
	PrintString("begin\n");
	CreateFile("file1.txt");
	PrintInt(OpenFileID("file1.txt", 0));
	PrintString("\n");
	CreateFile("file2.txt");
	PrintInt(OpenFileID("file2.txt", 0));
	PrintString("\n");
	CreateFile("file3.txt");
	PrintInt(OpenFileID("file3.txt", 0));
	Halt();

}
