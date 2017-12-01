#include "syscall.h"


int main(){

	char buf[255];
	ReadFile(buf, 255, 0);
	WriteFile(buf, 255, 1);
	PrintChar('\n');
	return 0;
}
