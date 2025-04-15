#pragma once

#include <Windows.h>
#include <bcrypt.h>
#include <minwindef.h>

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace crashlog {
enum class AccessViolationType {
	INVALID,
	READ_ACCESS_VIOLATION,
	WRITE_ACCESS_VIOLATION,
	DEP_ACCESS_VIOLATION
};
struct AccessViolationInfo {
	AccessViolationType violationType;
	void* address;
	NTSTATUS statusCode;
};

struct LineInfo {
	std::string fileName;
	int lineNumber;
};
struct Address {
	void* rawAddress;
	std::optional<std::string> symbol;
	std::optional<LineInfo> lineInfo;
};
struct ExceptionType {
	DWORD exceptionCode;
	std::optional<std::string> exceptionName;
};
struct ExceptionInfo {
	DWORD exceptionCode;
	std::optional<AccessViolationInfo> violationInfo;
	std::vector<Address> stacktrace;
	bool isNonContinuable;
	Address addressAt;
};

Address parseAddress(void*);

std::string addressToString(Address const&);

std::string rawAddressToString(uintptr_t);

std::optional<std::string> parseExceptionCode(DWORD);

std::string parseViolationType(AccessViolationType);
};	// namespace crashlog
