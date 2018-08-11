
//********************************
typedef struct _DISPLAY
{
	ULARGE_INTEGER Number;
	BYTE AmountSet;
	WCHAR Bufferset[128];
	WCHAR formatbuffer[64];
	BYTE numberformat;
	BYTE Digits[32];
}DISPLAY, *LPDISPLAY;

//Dos Class For ConSole
#define MAX_CONSOLEBUFFERS 2

class DosConsole
{

public:
	bool CpuidOk;
	int MaxBasicValue;
	int MaxExtendedValue;

	WCHAR wCpuInfo[64];
	DISPLAY display;
	int ActiveBuffers;
	char ConsoleTitleStd[256];
	HANDLE NewScreenBuffer[8];
	BOOL SetActiveBuffer(int Buffer);
	CONSOLE_CURSOR_INFO CursorInfoStd;	
	BOOLEAN StdFlag;
	HANDLE hOut;
	BOOLEAN StdInFlag;
	HANDLE hIn;
	DWORD LastError;
	DWORD AmountReadInput;
	LARGE_INTEGER StartingTime;
	LARGE_INTEGER EndingTime;
	double ElapsedNanoseconds;
	double ElapsedMicroseconds;
	double ElapsedMilliseconds;
	double ElapsedSeconds;
	LARGE_INTEGER Frequency;
	LARGE_INTEGER LastDelay;
	DosConsole();
	~DosConsole();
	UCHAR ShowDigits(DISPLAY *Display);
	CONSOLE_SCREEN_BUFFER_INFO GetBufferInfo();
	CONSOLE_SCREEN_BUFFER_INFO GetBufferInfoActive(int Buffer);

	BOOLEAN GetConsoleCursorInfoStd(CONSOLE_CURSOR_INFO *ScreenCurorStd);
	BOOLEAN GetConsoleTitleStd(char *string);
	DWORD   GetMessageErrorString(DWORD ErrorNumber);
	bool	GetStartTime();
	bool	GetEndTime();
	int MakeScreenBuffer();
	BOOLEAN SetConsoleTitleStd(char *string);
	//bool	ShowTextCpu(int ineax, bool bflag);//struct _CPUDATA data);
	WORD    SetColorText(int TextColor);
	WORD    SetBackGroundText(int BackGroundColor);
	WORD    SetBackAndText(int TextColor ,int BackGroundColor);
	WORD    SetBackAndTextActive(int TextColor ,int BackGroundColor,int Buffer);
	BOOLEAN SetConsoleCursorInfoStd(CONSOLE_CURSOR_INFO *ScreenCursorInfo);
	DWORD   SetScreenBackGroundActive(int BackGroundColor,int Buffer);
	DWORD   SetScreenBackGround(int BackGroundColor);
	BOOLEAN SetPos(int x,int y);
	BOOLEAN SetPosActive(int x,int y,int Buffer);

	BOOLEAN ReadInputRecordStd(INPUT_RECORD * InputRecord,BOOLEAN FlushFlag,DWORD *Read);
	BOOLEAN ReadConseleOutputAttribeStd(WORD *BufferAttribe,DWORD Amount,COORD StartRead,DWORD *Read);
	
	BOOLEAN WriteConsoleOutputAttribsStd(WORD *BufferAttribe,DWORD Amount,COORD StartRead,DWORD *Read);
}; 