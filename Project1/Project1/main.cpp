

#include <iostream>
#include <windows.h>
#include <Strsafe.h>

#include "classdosheader.h"
#include "definecolors.h"
#include "classdoscode.cpp"
#include "Aes.h"

	using namespace std;

	class DosConsole dos;

	bool ShowTextCpu(int ineax, bool bExtended, struct _CpuData *Cells);
	double CombineC(int a, int b, int c, int d, int e, double f)
	{
		return (a + b + c + d + e) / (f + +1.5);

	}
	
#pragma pack(push ,1)
	typedef struct _CpuCell
	{
		int eax;
		int ebx;
		int ecx;
		int edx;

	}CpuCell;
	__declspec (align (16))

		typedef struct _CpuData
	{
		struct _CpuCell Cpu[32];
	}CpuData,*LPCpuData;
#pragma pack (pop)



	// NOTE: extern “C” needed to prevent C++ name mangling
	extern "C"
	{
		double		CombineA(int a, int b, int c, int d, int e, double f);
		void		FunCpuid(int cpueax, int cpuebx, int cpuecx,int cpuedx);
		void		FuncPassPtr(int *);
		int			FuncCpuPtr(int *);
		double		FuncXmm(double f1, double d1);
		int			FuncCpuAll(struct _CpuData *Standard,struct _CpuData *Extended);
		void		FuncMemory(long long *address);
		double		FuncSums(double a, double b);
		float		FuncFloats(float *af, float *bf, float *cf);
		HANDLE		CreateThreadInAsm(int *i);
		void		FuncThreadStart(int *i);
		LPCpuData	FuncRetInfo(int flag, int *len);
	}

	struct _CpuData Standard;
	struct _CpuData	Extended;

	extern "C" void Showcpu(int a, int b, int c, int d)
	{
		static int scount = 2;
			Extended.Cpu[scount].eax = a;
			Extended.Cpu[scount].ebx = b;
			Extended.Cpu[scount].ecx = c;
			Extended.Cpu[scount].edx = d;		
			scount++;
			if (scount == 4)
			{
				ShowTextCpu(2, true, &Extended);
				printf("%S", dos.wCpuInfo);
				printf("\n\n");
			}
	}
	int main(void)
	{
	
		WORD OldAttrib;
		HANDLE ThreadHandle = 0;
		int ret;
		float *af, *bf,*cf;
		int count;
		int cpueax	= 0x80000002;
		int cpuebx	= 0;
		int cpuecx	= 0;
		int cpuedx	= 0;
		int i = 0;
		int *regs;
		long long *mem;
		int oldeax;
		float answer = 0.0f;
		DWORD AmountEx;
		struct _CpuData Standard;
		struct _CpuData	Extended;
		double a, b, c;
		int flag = 0;
		int Len = 0;
		CpuData *cpudata;
		CpuData *cpudataex;

		dos.SetScreenBackGround(BLUE);
		OldAttrib = dos.SetBackAndText(WHITE, BLUE);

		regs	= new int[sizeof(int) * 4];
		mem		= new long long[sizeof(DWORD) * 1024];
		af		= new float[sizeof(float) * 4];
		bf		= new float[sizeof(float) * 4];
		cf		= new float[sizeof(float) * 4];	
		//cpudata = new CpuData[(sizeof(CpuData))];

		double d1, d2, d3;
		double arand;

		arand = (double) rand()/ rand();
		regs[0] = 0x80000000;
		
		printf("The Value Of i Is now %d\n", i);
		printf("The Value Of i Is now %d\n", i);
		dos.GetStartTime();
		ThreadHandle = CreateThreadInAsm(&i);
		FuncThreadStart(&i);
		if (ThreadHandle != NULL)
		{
			WaitForSingleObject(ThreadHandle, 100); //waiting max 100ms
		}
		printf("hThread was %llX\n", (unsigned long long)ThreadHandle);
		printf("The Value Of i Is now %d\n", i);
		dos.GetEndTime();
		printf("The ASM Program CreateThreadInAsm Took %8.6f ms\n", dos.ElapsedMilliseconds);
		//************************** Timing for The C++ Prpgram *************************
		dos.GetStartTime();
		printf("CombineC (C++ Proc Returned) %8.4f\n", CombineC(1, 2, 3, 4, 5, 6.1));//c++ program
		dos.GetEndTime();
		printf("The C++ Program Took %8.4f us\n", dos.ElapsedMicroseconds);
		//********************************************************************************
		//************************** Timing for the ASM Program **************************
		dos.GetStartTime();

		printf("CombineA (asm Proc Returned) %8.16f\n", CombineA(1, 2, 3, 4, 5, 6.1));//masm64 assembly program Functine2.asm
		dos.GetEndTime();
		printf("The ASM Program Took %8.4f us\n", dos.ElapsedMicroseconds);
		//*********************************************************************************
		for (count = 0; count < 3; count++)
		{
			FunCpuid(cpueax, cpuebx, cpuecx, cpuedx);
			cpueax++;
		}
		FuncPassPtr(regs);
		printf("From FuncPassPtr() eax = %X, ebx = %X, ecx = %X, edx = %X \n", regs[0], regs[1], regs[2], regs[3]);
		regs[0] = 22;
		oldeax	= 0;
		oldeax = FuncCpuPtr(regs);
		printf("From FuncCpuPtr() eax = %X, ebx = %X, ecx = %X, edx = %X Sent Eax = %d\n", regs[0], regs[1], regs[2], regs[3],oldeax);
		d2 = 0.0;
		d1 = -513.68;
		d3 = -660.57;

		dos.GetStartTime();
		d2 = FuncXmm(d3, d1);//only returns xmm0(min) d3 and d1 passed by value
		dos.GetEndTime();
		printf("The ASM FuncXmm Took %8.4f us\n", dos.ElapsedMicroseconds);

		printf("FuncXmm() Min of d3 and d1 %8.4f \n", d2);
		Standard.Cpu[0].eax = 0xff;
		dos.GetStartTime();
		ret = FuncCpuAll(&Standard, &Extended);
		dos.GetEndTime();
		printf("The ASM FuncCpuAll Took %8.4f us\n", dos.ElapsedMicroseconds);
		dos.MaxBasicValue = Standard.Cpu[0].eax;
		dos.MaxExtendedValue = Extended.Cpu[0].eax;
		printf("\n\n");
		printf("******************************** Standard Cpuid ***********************\n");
		for(count = 0; count <= Standard.Cpu[0].eax;count++)
		{
			printf("FuncCpuAll Offset = %d and eax = %X  ebx = %X  ecx = %X  edx = %X \n",
				count, Standard.Cpu[count].eax, Standard.Cpu[count].ebx, Standard.Cpu[count].ecx, Standard.Cpu[count].edx);
		}

		AmountEx = 0;
		AmountEx = Extended.Cpu[0].eax - 0x80000000;
		ShowTextCpu(0, false, &Standard);
		printf("%S\n", dos.wCpuInfo);
		printf("Max Eax To Use With CPUID Is %d\n", dos.MaxBasicValue);
		printf("*********************************************************************\n");
		printf("\n");
		printf("******************************** Extended Cpuid ***********************\n");
		for (count = 0; count <= 1; count++)
		{
			printf("FuncCpuAll Offset = %d and eax = %X ebx = %X ecx = %X edx = %X \n",
				count, Extended.Cpu[count].eax, Extended.Cpu[count].ebx, Extended.Cpu[count].ecx, Extended.Cpu[count].edx);
		
		}

		ShowTextCpu(2, true, &Extended);
		printf("%S\n", dos.wCpuInfo);
		printf("Max Extended value for CPUID Is %8.8X \n", dos.MaxExtendedValue);
		printf("***********************************************************************\n");
		*mem = 0x1234;
		strcpy_s((char *)mem, 1024 * 4, "Hello Brian From C++");

		printf("The Contents of before FuncMemory %s \n",(char *) mem);
		FuncMemory(mem);
		printf("The Contents of after  FuncMemory %s \n",(char *) mem);
		a = 0.0;
		b = 9023.05679;
		c = 1287.0409509;
		dos.GetStartTime();
		a = FuncSums(b, c);
		dos.GetEndTime();
		printf("The ASM FuncSums Took %8.10f us\n", dos.ElapsedMicroseconds);
		printf("The Hypp is %4.4f\n", a);
		af[0] = 100.575f;
		af[1] = 300.564f;
		af[2] = 4556.866f;
		af[3] = 6.9666f;

		bf[0] = 710.5979f;
		bf[1] = 130.52313f;
		bf[2] = 545.8546f;
		bf[3] = 766.9867f;

		cf[0] = 0.0f;
		cf[1] = 0.0f;
		cf[2] = 0.0f;
		cf[3] = 0.0f;
		
		dos.GetStartTime();
		answer = FuncFloats(af, bf, cf);
		dos.GetEndTime();
		printf("The ASM FuncFloats Took %8.4f us\n", dos.ElapsedMicroseconds);
		printf("Floats cf[0] = %6.4f cf[1] = %6.4f cf[2] = %6.4f cf[3] = %6.4f Returned with %8.4f \n",
			cf[0], cf[1], cf[2], cf[3] ,answer );
		//**************************************************************************************************
		flag = 1;

	    cpudata = FuncRetInfo(flag,&Len);
		for (count = 0; count <= Len; count++)
		{
			printf("FuncRetInfo Offset = Eax %X  Ebx %X Ecx %X Edx %X \n", cpudata->Cpu[count].eax, cpudata->Cpu[count].ebx,
				cpudata->Cpu[count].ecx, cpudata->Cpu[count].edx);
		}
		ShowTextCpu(0, false, cpudata);
		printf("Maxium Value of eax in cpuid is %d\n", cpudata->Cpu->eax);
		printf("%S\n", dos.wCpuInfo);
		printf("\n\n");
		
		flag = 0;

		cpudataex = FuncRetInfo(flag, &Len);
		for (count = 0; count <= Len; count++)
		{
			printf("FuncRetInfo Offset = Eax %X  Ebx %X Ecx %X Edx %X \n", cpudataex->Cpu[count].eax, cpudataex->Cpu[count].ebx,
				cpudataex->Cpu[count].ecx, cpudataex->Cpu[count].edx);
		}
		ShowTextCpu(2, true, cpudataex);
		printf("Maxium Value of eax in cpuid is %X\n", cpudataex->Cpu->eax);
		printf("%S\n", dos.wCpuInfo);
		printf("\n\n");
		
		//*************************************************************************************************
		getchar();

		delete[] regs;
		delete[] mem;
		delete[] af;
		delete[] bf;
		delete[] cf;
		

		return 0;
	}
	//************************************ 
	bool ShowTextCpu(int ineax, bool bExtended, CpuData *Cells)
	{
	int count;
	int index;
	int count1;
	char cStrings[64];
	WORD wtemp;

	if ((ineax == 0) && (!bExtended))//do not use EAX for chars copy EBX,EDX,ECX in this order
	{
	index = 0;
	//Ebx
	wtemp = LOWORD(Cells->Cpu[0].ebx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ebx
	wtemp = LOWORD(Cells->Cpu[0].ebx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Ebx
	wtemp = HIWORD(Cells->Cpu[0].ebx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ebx
	wtemp = HIWORD(Cells->Cpu[0].ebx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Edx

	wtemp = LOWORD(Cells->Cpu[0].edx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Edx
	wtemp = LOWORD(Cells->Cpu[0].edx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Edx
	wtemp = HIWORD(Cells->Cpu[0].edx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//add a space
	cStrings[index] = (char)LOBYTE(' ');
	index++;
	//Edx
	wtemp = HIWORD(Cells->Cpu[0].edx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;

	//Ecx
	wtemp = LOWORD(Cells->Cpu[0].ecx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ecx
	wtemp = LOWORD(Cells->Cpu[0].ecx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Ecx
	wtemp = HIWORD(Cells->Cpu[0].ecx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ecx
	wtemp = HIWORD(Cells->Cpu[0].ecx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//14 terminator
	cStrings[index] = 0;
	MultiByteToWideChar(CP_ACP, 0, cStrings, sizeof(cStrings), dos.wCpuInfo, _countof(dos.wCpuInfo));
	}
	index = 0;
	if ((ineax == 2) && (bExtended))
	{
	for (count1 = 0; count1 < 3; count1++)
	{
	//Eax
	wtemp = LOWORD(Cells->Cpu[2 + count1].eax);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Eax
	wtemp = LOWORD(Cells->Cpu[2 + count1].eax);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Eax
	wtemp = HIWORD(Cells->Cpu[2 + count1].eax);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Eax
	wtemp = HIWORD(Cells->Cpu[2 + count1].eax);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;

	//Ebx
	wtemp = LOWORD(Cells->Cpu[2 + count1].ebx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ebx
	wtemp = LOWORD(Cells->Cpu[2 + count1].ebx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Ebx
	wtemp = HIWORD(Cells->Cpu[2 + count1].ebx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ebx
	wtemp = HIWORD(Cells->Cpu[2 + count1].ebx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;

	//Ecx
	wtemp = LOWORD(Cells->Cpu[2 + count1].ecx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ecx
	wtemp = LOWORD(Cells->Cpu[2 + count1].ecx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Ecx
	wtemp = HIWORD(Cells->Cpu[2 + count1].ecx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Ecx
	wtemp = HIWORD(Cells->Cpu[2 + count1].ecx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;

	//Edx
	wtemp = LOWORD(Cells->Cpu[2 + count1].edx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Edx
	wtemp = LOWORD(Cells->Cpu[2 + count1].edx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	//Edx
	wtemp = HIWORD(Cells->Cpu[2 + count1].edx);
	cStrings[index] = (char)LOBYTE(wtemp);
	index++;
	//Edx
	wtemp = HIWORD(Cells->Cpu[2 + count1].edx);
	cStrings[index] = (char)HIBYTE(wtemp);
	index++;
	}
	//terminator
	cStrings[index] = 0;
	MultiByteToWideChar(CP_ACP, 0, cStrings, sizeof(cStrings), dos.wCpuInfo, _countof(dos.wCpuInfo));
	}
	count = 0;
	return true;
	}
	//***************************************
