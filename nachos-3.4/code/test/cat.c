#include "syscall.h"
int main(){
	// these bunch of code is also used to test ReadFile and WriteFile
	char name[255];
	char buf[1024];
	int id;

	PrintString("nhap vao ten file");PrintChar('\n');
	ReadString(name, 255);
	id = OpenFileID(name, 1);
	PrintInt(id);
	PrintString(" opened file: ");
	ReadFile(buf, 255, id);
//	PrintString(" read string: ");
	PrintString(buf);
	PrintChar('\n');
	CloseFileID(3);	

	return 0;

}
