#include "crashlog/win64/symbol_resolver.hpp"

#include <Windows.h>
#include <DbgHelp.h>
#include <psapi.h>
#include <cstdint>

void crashlog::win64::SymbolResolver::loadAllSymbols() {
	HMODULE modules[1024];
	DWORD capacityBytesNeeded;
	if (EnumProcessModules(GetCurrentProcess(), modules, sizeof(modules), &capacityBytesNeeded)) {
		DWORD numModules = capacityBytesNeeded / sizeof(HMODULE);
		for (uint32_t i = 0; i < numModules; i++) {
			TCHAR moduleFileName[MAX_PATH];
			if (!GetModuleFileNameEx(GetCurrentProcess(), modules[i], moduleFileName, sizeof(moduleFileName) / sizeof(TCHAR))) {
				continue;
			}

			DWORD64 baseAddr = (DWORD64)modules[i];
			SymLoadModuleEx(
				GetCurrentProcess(),
				NULL,
				moduleFileName,
				NULL,
				baseAddr,
				0,
				NULL,
				0);

			IMAGEHLP_MODULE64 modInfo;
			memset(&modInfo, 0, sizeof(modInfo));
			modInfo.SizeOfStruct = sizeof(modInfo);
			SymGetModuleInfo64(GetCurrentProcess(), baseAddr, &modInfo);
		}
	}
}
