#include "syscall.h"
int main(){
	int length = 100;
	int array[100];
	for (int i=0; i<100; i++){
		array[i] = (100-i)/2;
	};
	Sort(length, array);
	for (int i=0;i<100;i++){
		PrintInt(array[i]);
		PrintChar(' ');
	};
	Halt();
	return 0;
}
