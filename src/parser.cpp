#include <crashlog/exception_info.h>
#include <crashlog/internal.h>
#include <crashlog/parser.h>
#include <windows.h>

#include <format>
#include <optional>
#include <sstream>

#include "DbgHelp.h"

#pragma comment(lib, "DbgHelp.lib")

using namespace crashlog;

void crashlog::initSym() {
	SymSetOptions(SYMOPT_LOAD_LINES);
	SymInitialize(GetCurrentProcess(), NULL, TRUE);
}

ExceptionInfo crashlog::parseException(EXCEPTION_POINTERS* input) {
	DWORD exceptionCode = input->ExceptionRecord->ExceptionCode;
	std::optional<AccessViolationInfo> violationInfo = crashlog::internal::parseViolationInfo(input->ExceptionRecord);
	bool isNonContinuable = input->ExceptionRecord->ExceptionFlags | EXCEPTION_NONCONTINUABLE;
	Address addressAt = parseAddress(input->ExceptionRecord->ExceptionAddress);

	ExceptionInfo info;
	memset(&info, 0, sizeof(info));
	info.exceptionCode = exceptionCode;
	info.violationInfo = violationInfo;
	info.stacktrace = crashlog::internal::createStackTrace(input);
	info.isNonContinuable = isNonContinuable;
	info.addressAt = addressAt;
	return info;
}

std::string crashlog::exceptionInfoToString(ExceptionInfo const& info) {
	std::stringstream stream;

	Address address = info.addressAt;
	stream << std::format("Address={}", addressToString(address)) << std::endl;

	auto exceptionName = crashlog::parseExceptionCode(info.exceptionCode);
	if (!exceptionName.has_value()) {
		exceptionName = "UNKNOWN";
	}
	stream << std::format("Type={} (0x{:x})", exceptionName.value(), info.exceptionCode) << std::endl;
	if (info.violationInfo.has_value()) {
		AccessViolationInfo violationInfo = info.violationInfo.value();
		stream << std::format(" - ViolationName={}", crashlog::parseViolationType(violationInfo.violationType)) << std::endl;
		stream << std::format(" - ViolationAddress={}", violationInfo.address) << std::endl;
	}

	return stream.str();
}

std::string crashlog::createStackTraceString(ExceptionInfo const& info) {
	std::stringstream stream;
	for (Address const& addr : info.stacktrace) {
		stream << addressToString(addr) << std::endl;
	}
	return stream.str();
}

std::string crashlog::createStackDumpString(EXCEPTION_POINTERS* ptr) {
	std::stringstream stream;
	stream << std::format("RAX={}, RCX={}, RDX={}, RBX={}", rawAddressToString(ptr->ContextRecord->Rax), rawAddressToString(ptr->ContextRecord->Rcx), rawAddressToString(ptr->ContextRecord->Rdx), rawAddressToString(ptr->ContextRecord->Rbx)) << std::endl;
	stream << std::format("Rsp={}, Rbp={}, Rsi={}, Rdi={}", rawAddressToString(ptr->ContextRecord->Rsp), rawAddressToString(ptr->ContextRecord->Rbp), rawAddressToString(ptr->ContextRecord->Rsi), rawAddressToString(ptr->ContextRecord->Rdi)) << std::endl;
	stream << std::format("R8={}, R9={}, R10={}, R11={}", rawAddressToString(ptr->ContextRecord->R8), rawAddressToString(ptr->ContextRecord->R9), rawAddressToString(ptr->ContextRecord->R10), rawAddressToString(ptr->ContextRecord->R11)) << std::endl;
	stream << std::format("R9={}, R10={}, R11={}, R12={}", rawAddressToString(ptr->ContextRecord->R9), rawAddressToString(ptr->ContextRecord->R10), rawAddressToString(ptr->ContextRecord->R11), rawAddressToString(ptr->ContextRecord->R12)) << std::endl;
	stream << std::format("R13={} R14={}, R15={}, Rip={}", rawAddressToString(ptr->ContextRecord->R13), rawAddressToString(ptr->ContextRecord->R14), rawAddressToString(ptr->ContextRecord->R15), rawAddressToString(ptr->ContextRecord->Rip)) << std::endl;
	return stream.str();
}
