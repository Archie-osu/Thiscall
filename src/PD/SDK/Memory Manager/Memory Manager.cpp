#include "Memory Manager.hpp"
#include "../Void.hpp"
#include <Windows.h>
#include <Psapi.h>

MODULEINFO GetModuleInfo()
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(NULL);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

unsigned long CMemoryManager::GetFuncAddress(const char* ModuleName, const char* FunctionName)
{
	HMODULE Module = GetModuleHandleA(ModuleName);

	if (!Module)
		return 0;

	return ReCa<unsigned long>(GetProcAddress(Module, FunctionName));
}

unsigned long CMemoryManager::PatternScan(const char* Pattern, const char* Mask, bool StringMode)
{
	//Get all module related information
	MODULEINFO mInfo = GetModuleInfo();

	DWORD base, size;

	//Assign our base and module size
	base = (DWORD)mInfo.lpBaseOfDll;
	size = (DWORD)mInfo.SizeOfImage;

	//Get length for our mask, this will allow us to loop through our array
	DWORD patternLength = strlen(Mask);

	for (unsigned i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			//if we have a ? in our mask then we have true by default,
			//or if the bytes match then we keep searching until finding it or not
			found &= Mask[j] == '?' || Pattern[j] == *(char*)(base + i + j);
		}

		//found = true, our entire pattern was found
		if (found)
		{
			if (StringMode && *(char*)(base + i - 1) != '\x00')
				continue;

			return (base + i);
		}
	}
	return 0;
}