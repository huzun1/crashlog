#pragma once

#include <Windows.h>
#include <bcrypt.h>
#include <minwindef.h>

#include <optional>
#include <string>

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

struct Address {
	void* rawAddress;
	std::optional<std::string> symbol;
};
struct ExceptionType {
	DWORD exceptionCode;
	std::optional<std::string> exceptionName;
};
struct ExceptionInfo {
	DWORD exceptionCode;
	std::optional<AccessViolationInfo> violationInfo;
	bool isNonContinuable;
	Address addressAt;
};

Address parseAddress(void*);

std::optional<std::string> parseExceptionCode(DWORD);

std::string parseViolationType(AccessViolationType);
};	// namespace crashlog
