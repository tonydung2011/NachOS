#include "syscall.h"

int main(){

	int isGood = CreateFile("demo.txt");
	PrintInt(isGood);
	isGood = OpenFileID("demo.txt", 0);
	PrintInt(isGood);
	Halt();
	return 0;
}
