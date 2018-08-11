
CpuCell struc
		Cpueax dword 0
		Cpuebx dword 0
		Cpuecx dword 0
		Cpuedx dword 0
CpuCell ends

CpuData struc
cpu CpuCell 32 dup ({0,0,0,0})
CpuData ends

