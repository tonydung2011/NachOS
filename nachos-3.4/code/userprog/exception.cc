// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int Systype = machine->ReadRegister(2);
    switch (which)
    {
		case SyscallException:
		switch (Systype)
			{
				case SC_Halt:
					DEBUG('a', "Shutdown, initiated by user program.\n");
   				interrupt->Halt();
				break;
				case SC_ReadInt:
				{
				/*Read integer number 
					and return it*/
					DEBUG('a', "Read integer number from console.\n");
					int number = 0;
					int nDigit = 0;
					int i;
					char* bufer = new char[MAX_INT_LENGTH];
					nDigit = gSynchConsole->Read(bufer, MAX_INT_LENGTH);
					i = bufer[0] == '-' ? 1:0 ;
					for (; i < nDigit; ++i)
					{
						number = number*10 + (int) (bufer[i] & MASK_GET_NUM);
					}
					number = bufer[0] == '-' ? -1*number : number;
					machine->WriteRegister(2, number);
					delete bufer;
				}
				break;
				case SC_PrintInt:
				{
					char s[MAX_INT_LENGTH], neg;
					neg = '-';
					int i, n, digitCount = 1, sz;
					i = n = 0;
					DEBUG('a', "Read argument value at r4");
					n = machine->ReadRegister(4);
					if (n < 0)
					{
						gSynchConsole->Write(&neg,1);
						n = -n;
					}
					while (digitCount*10 <= n){
					        digitCount*=10;
					        i++;
					}
					s[i] = '\0'; sz = i;
					do {
						s[i--] = n%10 + '0';
					}	while (( n /= 10) > 0);
					gSynchConsole->Write(s, sz+2);
				}
				break;
				case SC_PrintChar:
				{
					char ch;
					ch = (char) machine->ReadRegister(4);
					//printf("ch = %c",ch);
					gSynchConsole->Write(&ch, 1);
					break;
				}
				case SC_ReadChar:
				{
					int sz;
					char buf[MAX_INT_LENGTH];
					sz = gSynchConsole->Read(buf, MAX_INT_LENGTH);
					machine->WriteRegister(2, buf[sz-1]);
				}
				break;
				case SC_PrintString:
				{
					int bufAddr = machine->ReadRegister(4);
					int i = 0;
					char *buf = new char[LIMIT];
					buf = machine->User2System(bufAddr, LIMIT);
					while (buf[i] != 0 && buf[i] != '\n')
					{
						gSynchConsole->Write(buf+i, 1);
						i++;
					}
					buf[i] = '\n';
					gSynchConsole->Write(buf+i,1);
					delete[] buf;
				}
				break;
				case SC_ReadString:
				{
					char *buf = new char[LIMIT];
					if (buf == 0) break;
					int bufAddrUser = machine->ReadRegister(4);
					int length = machine->ReadRegister(5);
					int sz = gSynchConsole->Read(buf, length);
					machine->System2User(bufAddrUser, sz, buf);
					delete[] buf;
				}
				break;
				case SC_Sort:
				{
					int length = machine->ReadRegister(4);
					int Array[100];
					int tempRegister = machine->ReadRegister(5);
					
					// get data from register space

					for (int i = 0 ;i<length;i++){
						machine->ReadMem(tempRegister, 4, Array+i);
						tempRegister+=4;
					};

					// bubble sort

					for (int i = 0;i < length-1; i++){
						for (int j = i+1;j < length; j++){
							if (Array[i]>Array[j]){
								Array[i] = Array[i]+Array[j];
								Array[j] = Array[i]-Array[j];
								Array[i] = Array[i]-Array[j];
							};
						};
					};
				};
				break;
				case SC_Help:
				{
					char* helpString = "Nhom 1512081 - 1512113\nTran Ngoc Dung - 1512081\nNguyen Minh Dong - 1512113\nASCII: bang ma asciico ban\nSort: thuat toan bubble sort";
					gSynchConsole->Write(helpString, 134);
				};
				break;
				case SC_ASCII:
				{
					char* asciiString = "32	20	spacebar\n33	21	!\n34	22	''\n35	23	#\n36	24	$\n37	25	%\n38	26	&\n39	27	'\n40	28	(\n41	29	)\n42	2A	*\n43	2B	+\n44	2C	,\n45	2D	-\n46	2E	.\n47	2F	/\n48	30	0\n49	31	1\n50	32	2\n51	33	3\n52	34	4\n53	35	5\n54	36	6\n55	37	7\n56	38	8\n57	39	9\n58	3A	:\n59	3B	;\n60	3C	<\n61	3D	=\n62	3E	>\n63	3F	?\n64	40	@\n65	41	A\n66	42	B\n67	43	C\n68	44	D\n69	45	E\n70	46	F\n71	47	G\n72	48	H\n73	49	I\n74	4A	J\n75	4B	K\n76	4C	L\n	4D	M\n78	4E	N\n79	4F	O\n80	50	P\n81	51	Q\n82	52	R\n83	53	S\n84	54	T\n85	55	U\n86	56	V\n87	57	W\n88	58	X\n89	59	Y\n90	5A	Z91	5B	[\n92	5C	\ \n93	5D	]\n94	5E	^\n95	5F	_\n96	60	`\n97	61	a\n98	62	b\n99	63	c\n100	64	d\n101	65	e\n102	66	f\n103	67	g\n104	68	h\n105	69	i\n106	6A	j\n107	6B	k\n108	6C	l\n109	6D	m\n110	6E	n\n111	6F	o\n112	70	p\n113	71	q\n114	72	r\n115	73	s\n116	74	t\n117	75	u\n118	76	v\n119	77	w\n120	78	x\n121	79	y\n122	7A	z\n123	7B	{\n124	7C	|\n125	7D	}\n126	7E	~";
					gSynchConsole->Write(asciiString, 1524);
				};
				break;
			}
    // Advance program counters.
    machine->registers[PrevPCReg] = machine->registers[PCReg];	
    machine->registers[PCReg] = machine->registers[NextPCReg];
    machine->registers[NextPCReg] += 4;
		break;
	case  PageFaultException:    // No valid translation found
		printf("No valid translation found %d %d\n", which, Systype);
		ASSERT(FALSE);
		break;
	case  ReadOnlyException:     // Write attempted to page marked 
		printf("Write attempted tp page marked %d %d\n", which, Systype);
		ASSERT(FALSE);
		break;
	case  BusErrorException:     // Translation resulted in an 
		printf("Translaion resulted in an %d %d\n", which, Systype);
		ASSERT(FALSE);
		break;
	case  AddressErrorException: // Unaligned reference or one that
		printf("Unaligned reference or one that %d %d\n", which, Systype);
		ASSERT(FALSE);
		break;
	case  OverflowException:     // Integer overflow in add or sub.
		printf("Integer overflow in add or sub %d %d\n", which, Systype);
		ASSERT(FALSE);
		break;
	case  IllegalInstrException: // Unimplemented or reserved instr.
		printf("Unimplemented or reserved instr %d %d\n", which, Systype);
		ASSERT(FALSE);
		break;
	case NumExceptionTypes:
		printf("an handle exception at %d %d\n", which, Systype);
		ASSERT(FALSE);
		break;
	case NoException:
		break;
   }
}
