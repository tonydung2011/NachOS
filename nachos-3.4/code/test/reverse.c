#include "syscall.h"

void reverse(char* buf){
	int j,i, length; char t, *index;
	index = buf; length = 0; 
	while ( (*index) >=' ' && (*index) <= '~')
	{
		index++;length++;
	}
	for (i = 0;i<length/2;i++){
		j = length - i -1;
		t = buf[i];
		buf[i] = buf[j];
		buf[j] = t;
	}
}

int main(){

	char name1[255];
	char name2[255];
	char buf[255];
	int id1, id2;


	PrintString("nhap vao ten file 1");PrintChar('\n');
	ReadString(name1, 255);
	PrintString("nhap vao ten file 2");PrintChar('\n');
	ReadString(name2, 255);

	PrintString("remember file name");PrintChar('\n');
	
	id1 = OpenFileID(name1, 1);
	PrintInt(id1);
	PrintString(" opened file 1 and read: ");
	ReadFile(buf, 255, id1);
	PrintChar('\n');
	CloseFileID(id1);
	
	reverse(buf);

	id2 = OpenFileID(name2, 1);
	PrintInt(id2);
	PrintString(" opened file 2 and write: ");
	WriteFile(buf, 255, id2);
//	PrintString(" read string: ");
	PrintChar('\n');
	CloseFileID(id2);	

	return 0;

}
