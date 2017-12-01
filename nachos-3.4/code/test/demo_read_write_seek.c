#include "syscall.h"

int main(){

	int isGood = CreateFile("demo.txt");
	PrintInt(isGood);
	isGood = OpenFileID("demo.txt", 0);
	PrintInt(isGood);
	char buff[255];
	if(isGood!=-1)
	{
		WriteFile("Do An 2",7,isGood);
		Seek(2,isGood);
		ReadFile(buff,7,isGood);
		PrintString(buff);
		CloseFileID(isGood);
	}
	Halt();
	return 0;
}