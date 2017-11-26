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
#include "string.h"

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
					char* bufer = new char(9);
					nDigit = gSynchConsole->Read(bufer, 9);
					i = bufer[0] == '-' ? 1:0 ;
					for (; i < nDigit; ++i)
					{
						number = number*10 + bufer[i] - '0';
					}
					number = bufer[0] == '-' ? -1*number : number;
					machine->WriteRegister(2, number);
					delete bufer;
				}
				break;
				case SC_PrintInt:
				{
					char s[9], neg;
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
					while (digitCount <= n){
					        digitCount*=10;
					        i++;
					}
					s[i] = '\0'; sz = i--;
					if (n==0){
						s[0] = '0';
						gSynchConsole->Write(s, 1);					
						break;
					}
					do {
						s[i--] = n%10 + '0';
					}	while (( n /= 10) > 0);
					gSynchConsole->Write(s, sz);					

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
					char *buf = new char[LIMIT];
					if (buf == 0) break;
					int bufAddrUser = machine->ReadRegister(4);
					int length = machine->ReadRegister(5);
					int sz = gSynchConsole->Read(buf, length);
					machine->System2User(bufAddrUser, sz, buf);
					delete[] buf;
				}
				break;
				case SC_PrintString:
				{
					int bufAddr = machine->ReadRegister(4);
					int i = 0;
					char *buf = new char[LIMIT];
					buf = machine->User2System(bufAddr, LIMIT);					
					while (buf[i] >= 32 && buf[i] <= 126)
					{
						gSynchConsole->Write(&buf[i], 1);
						i++;
					}				
					delete[] buf;
				}
				break;
				case SC_ReadString:
				{
					char *InputString = new char[LIMIT];
					int Size;
					int VirtualAddress = machine->ReadRegister(4); // doc dia chi thanh ghi so 4 
					Size = gSynchConsole->Read(InputString, LIMIT);// lay size va luu chuoi vao size va int
					machine->System2User(VirtualAddress, Size, InputString);//chuyen string tu kernel sang user
					delete[] InputString;
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
                                case SC_CreateFile:
                                {
                                        int fileNamePointer = machine->ReadRegister(4);
                                        char * fileName = new char [LIMIT];
                                        fileName = machine->User2System(fileNamePointer, LIMIT);
                                        if (!fileSystem->Create(fileName , 0))
                                        {
                                               machine->WriteRegister(2,-1);
                                               break;
                                        }
                                        machine->WriteRegister(2,0);
                                        delete [] fileName;                      
                                }
                                break;
				case SC_OpenFileID:
				{
					int fileNamePtr = machine->ReadRegister(4);
					int typeOpen = machine->ReadRegister(5);
					char* fileName = new char[255];
					if (fileSystem->numFileTable == 10) {
						
						machine->WriteRegister(2, -1);
						printf("not enough space to open file");
						break;

					}
					fileName = machine->User2System(fileNamePtr, 255);
					if (fileSystem->Open(fileName, typeOpen)!=NULL){
						machine->WriteRegister(2, fileSystem->numFileTable-1);

					}else{
						machine->WriteRegister(2, -1);

					}				
					delete[] fileName;
				}
				break;
				case SC_CloseFileID:
				{
					int idFile = machine->ReadRegister(4);
					if (fileSystem->fileTable[idFile]!=NULL){
						delete fileSystem->fileTable[idFile];
						fileSystem->fileTable[idFile] = NULL;
					}
					machine->WriteRegister(2, idFile);
				}
				break;
				case SC_ReadFile:
				{
					int buffer=machine->ReadRegister(4);
					int charCount=machine->ReadRegister(5);
					int numFile=machine->ReadRegister(6);
					char *buf= new char[charCount];
					//Kiem tra file co nam trong 0-9 file hoac chua duoc tao
					if(numfile<0||numfile>10||fileSystem->fileTable[numfile]==NULL)
					{
						machine->WriteRegister(2,-1);
						delete [] buf;
						break;
					}
					buf=machine->User2System(buffer,charCount);
					//Doc tu console
					if(fileSystem->fileTable[numfile]->typeOpen==2)
					{
						int sz;//so byte doc duoc
						sz=gSynchConsole->Read(buf,charCount);
						machine->System2User(buffer,sz,buf);
						delete []buf;
						break;
					}
					//Doc tu file
					int Pos1,Pos2;
					Pos1=fileSystem->fileTable[numfile]->GetCurrentPos();//vi tri dau file
					if(fileSystem->fileTable[numfile]->Read(buf,charCount)>0)
					{
						Pos2=fileSystem->fileTable[numfile]->GetCurrentPos();//vi tri cuoi file
						machine->System2User(buffer,Pos2-Pos1+1,buff);
						machine->WriteRegister(2,Pos2-Pos1+1)
					}
					else
					{
						machine->WriteRegister(2,-1);
						delete []buf;
						break;
					}
				}
				break;
				case SC_WriteFile:
				{
					int buffer=machine->ReadRegister(4);
					int charCount=machine->ReadRegister(5);
					int numFile=machine->ReadRegister(6);
					char *buf= new char[charCount];
					//Kiem tra file co nam trong 0-9 file hoac chua duoc tao
					if(numfile<0||numfile>10||fileSystem->fileTable[numfile]==NULL)
					{
						machine->WriteRegister(2,-1);
						delete [] buf;
						break;
					}
					buf=machine->User2System(buffer,charCount);
					//ghi file ra console
					if(fileSystem->fileTable[numfile]->typeOpen==3)
					{
						int i=0;
						while(buf[i]!=0)
						{
							gSynchConsole->Write(buf+i,1);
							i++;
						}
						machine->WriteRegister(2,i-1);
						delete []buf;
						break;
					}
					//ghi ra file
					int Pos1,Pos2;
					Pos1=fileSystem->fileTable[numfile]->GetCurrentPos();//vi tri dau file
					if(fileSystem->fileTable[numfile]->Write(buf,charCount)>0)
					{
						Pos2=fileSystem->fileTable[numfile]->GetCurrentPos();//vi tri cuoi file
                        machine->WriteRegister(2,Pos2-Pos1+1);
                        delete []buf;
                        break;
					}
				}
				break;
				case SC_Seek:
				{
					int pos=machine->ReadRegister(4);
					int numfile=machine->ReadRegister(5);
					//Kiem tra file co nam trong 0-9 file hoac chua duoc tao hoac pos k hop le
					if(numfile<0||numfile>10||fileSystem->fileTable[numfile]==NULL||pos<-1)
					{
						machine->WriteRegister(2,-1);
						break;
					}
					//seek tren console
					if(fileSystem->fileTable[numfile]->typeOpen==2||fileSystem->fileTable[numfile]->typeOpen==3)
					{
						machine->WriteRegister(2,-1);
						break;
					}
					//seek cuoi file
					if(pos==-1)
					{
						fileSystem->fileTable[numfile]->SeekF(fileSystem->fileTable[numfile]->length());
						machine->WriteRegister(2,fileSystem->fileTable[numfile]->length());
						break;
					}
					fileSystem->fileTable[numfile]->SeekF(pos);
					machine->WriteRegister(2,pos);
				}
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
