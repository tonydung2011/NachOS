#include "syscall.h"
int main()
{
	int flag;
	if (CreateFile("file1.txt") == 0)
        {
		PrintString("mo file 1 thanh cong");
		PrintChar('\n'); 
	}
        else
        { 
		PrintString("k the mo file\n"); 
		PrintChar('\n');
	}
	if (CreateFile("file2.txt") == 0)
        {
		PrintString("mo file 2 thanh cong");
		PrintChar('\n'); 
	}
        else
        { 
		PrintString("k the mo file\n"); 
		PrintChar('\n');
	}
	flag = OpenFileID("file1.txt", 1);
	PrintInt(flag);
	PrintChar('\n');
	flag = OpenFileID("file2.txt", 1);
	PrintInt(flag);
	PrintChar('\n');
	flag = OpenFileID("file.txt", 1);
	PrintInt(flag);
	PrintChar('\n');
	return 0;
}
