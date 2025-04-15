#include "internal.h"

#include <DbgHelp.h>
#include <processthreadsapi.h>

#include <vector>

#include "exception_info.h"

#pragma comment(lib, "DbgHelp.lib")

using namespace crashlog;

std::vector<Address> crashlog::internal::createStackTrace(EXCEPTION_POINTERS* ptr) {
	std::vector<Address> addresses;

	CONTEXT context = *ptr->ContextRecord;
	STACKFRAME64 stackframe;
	ZeroMemory(&stackframe, sizeof(stackframe));
	stackframe.AddrPC.Offset = context.Rip;
	stackframe.AddrPC.Mode = AddrModeFlat;
	stackframe.AddrFrame.Offset = context.Rbp;
	stackframe.AddrFrame.Mode = AddrModeFlat;
	stackframe.AddrStack.Offset = context.Rsp;
	stackframe.AddrStack.Mode = AddrModeFlat;

	while (StackWalk64(
		IMAGE_FILE_MACHINE_AMD64,
		GetCurrentProcess(),
		GetCurrentThread(),
		&stackframe,
		&context,
		NULL,
		NULL,
		NULL,
		NULL)) {
		addresses.push_back(parseAddress((void*&)stackframe.AddrPC.Offset));
	}
	return addresses;
}

std::optional<AccessViolationInfo> crashlog::internal::parseViolationInfo(EXCEPTION_RECORD* record) {
	bool isAccessViolation = record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION;
	bool isInPageError = record->ExceptionCode == EXCEPTION_IN_PAGE_ERROR;
	if (!(isAccessViolation || isInPageError) || record->NumberParameters < 1) {
		return std::optional<AccessViolationInfo>();
	}

	AccessViolationType viloationType = parseViolationType(record->ExceptionInformation[0]);
	void* address = reinterpret_cast<void*>(record->ExceptionInformation[1]);
	NTSTATUS statusCode = -1;
	if (isInPageError) {
		statusCode = record->ExceptionInformation[2];
	}
	return AccessViolationInfo{viloationType, address, statusCode};
}

AccessViolationType crashlog::internal::parseViolationType(uint64_t type) {
	switch (type) {
		case 0:
			return AccessViolationType::READ_ACCESS_VIOLATION;
		case 1:
			return AccessViolationType::WRITE_ACCESS_VIOLATION;
		case 8:
			return AccessViolationType::DEP_ACCESS_VIOLATION;
		default:
			return AccessViolationType::INVALID;
	}
}
