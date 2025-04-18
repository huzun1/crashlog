
#include <crashlog/exception_info.h>
#include <processthreadsapi.h>
#include <windows.h>

#include <cstdint>
#include <iomanip>
#include <map>
#include <optional>
#include <sstream>
#include <format>

#include "DbgHelp.h"

using namespace crashlog;

Address crashlog::parseAddress(void* rawAddress) {
	BYTE symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME] = {};
	SYMBOL_INFO* symbol = (SYMBOL_INFO*)symbolBuffer;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol->MaxNameLen = MAX_SYM_NAME;

	std::optional<std::string> symboled;
	bool result = SymFromAddr(GetCurrentProcess(), reinterpret_cast<uintptr_t>(rawAddress), nullptr, symbol);
	if (result) {
		symboled = symbol->Name;
	}

	std::optional<LineInfo> lineInfo;
	IMAGEHLP_LINE64 line;
	memset(&line, 0, sizeof(IMAGEHLP_LINE64));
	line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
	DWORD dwDisplacement;
	if (SymGetLineFromAddr64(GetCurrentProcess(), reinterpret_cast<uintptr_t>(rawAddress), &dwDisplacement, &line)) {
		lineInfo = LineInfo{
			line.FileName,
			(int)line.LineNumber,
		};
	}

	return {
		rawAddress,
		symboled,
		lineInfo};
};

std::string crashlog::addressToString(Address const& address) {
	std::string rawAddress = rawAddressToString(reinterpret_cast<uintptr_t>(address.rawAddress));

	std::ostringstream str;
	if (address.lineInfo.has_value()) {
		LineInfo info = address.lineInfo.value();
		str << std::format("{} L{} - ", info.fileName, info.lineNumber);
	}
	if (address.symbol.has_value()) {
		str << std::format("{} ({})", address.symbol.value(), address.rawAddress);
	} else {
		str << rawAddress;
	}

	return str.str();
}

std::string crashlog::rawAddressToString(uintptr_t ptr) {
	std::ostringstream str;
	str << "0x" << std::setw(12) << std::setfill('0') << std::hex << ptr;
	return str.str();
}

std::optional<std::string> crashlog::parseExceptionCode(DWORD exCode) {
	std::map<DWORD, std::string> exceptions = {
		{EXCEPTION_ACCESS_VIOLATION, "EXCEPTION_ACCESS_VIOLATION"},
		{EXCEPTION_ARRAY_BOUNDS_EXCEEDED, "EXCEPTION_ARRAY_BOUNDS_EXCEEDED"},
		{EXCEPTION_BREAKPOINT, "EXCEPTION_BREAKPOINT"},
		{EXCEPTION_DATATYPE_MISALIGNMENT, "EXCEPTION_DATATYPE_MISALIGNMENT"},
		{EXCEPTION_FLT_DENORMAL_OPERAND, "EXCEPTION_FLT_DENORMAL_OPERAND"},
		{EXCEPTION_FLT_DIVIDE_BY_ZERO, "EXCEPTION_FLT_DIVIDE_BY_ZERO"},
		{EXCEPTION_FLT_INEXACT_RESULT, "EXCEPTION_FLT_INEXACT_RESULT"},
		{EXCEPTION_FLT_INVALID_OPERATION, "EXCEPTION_FLT_INVALID_OPERATION"},
		{EXCEPTION_FLT_OVERFLOW, "EXCEPTION_FLT_OVERFLOW"},
		{EXCEPTION_FLT_STACK_CHECK, "EXCEPTION_FLT_STACK_CHECK"},
		{EXCEPTION_FLT_UNDERFLOW, "EXCEPTION_FLT_UNDERFLOW"},
		{EXCEPTION_ILLEGAL_INSTRUCTION, "EXCEPTION_ILLEGAL_INSTRUCTION"},
		{EXCEPTION_IN_PAGE_ERROR, "EXCEPTION_IN_PAGE_ERROR"},
		{EXCEPTION_INT_DIVIDE_BY_ZERO, "EXCEPTION_INT_DIVIDE_BY_ZERO"},
		{EXCEPTION_INT_OVERFLOW, "EXCEPTION_INT_OVERFLOW"},
		{EXCEPTION_INVALID_DISPOSITION, "EXCEPTION_INVALID_DISPOSITION"},
		{EXCEPTION_NONCONTINUABLE_EXCEPTION, "EXCEPTION_NONCONTINUABLE_EXCEPTION"},
		{EXCEPTION_PRIV_INSTRUCTION, "EXCEPTION_PRIV_INSTRUCTION"},
		{EXCEPTION_SINGLE_STEP, "EXCEPTION_SINGLE_STEP"},
		{EXCEPTION_STACK_OVERFLOW, "EXCEPTION_STACK_OVERFLOW"}};

	if (!exceptions.contains(exCode)) {
		return std::optional<std::string>();
	}
	return exceptions[exCode];
};

std::string crashlog::parseViolationType(AccessViolationType type) {
	switch (type) {
		case crashlog::AccessViolationType::READ_ACCESS_VIOLATION: {
			return "READ_ACCESS_VIOLATION";
		}
		case crashlog::AccessViolationType::WRITE_ACCESS_VIOLATION: {
			return "WRITE_ACCESS_VIOLATION";
		}
		case crashlog::AccessViolationType::DEP_ACCESS_VIOLATION: {
			return "DEP_ACCESS_VIOLATION";
		}
		default:
			return "INVALID_VIOLATION_TYPE";
	}
}
