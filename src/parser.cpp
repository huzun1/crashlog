#include "parser.h"

#include <windows.h>

#include <format>
#include <optional>
#include <sstream>

#include "exception_info.h"
#include "internal.h"

using namespace crashlog;

ExceptionInfo crashlog::parseException(EXCEPTION_POINTERS* input) {
	DWORD exceptionCode = input->ExceptionRecord->ExceptionCode;
	std::optional<AccessViolationInfo> violationInfo = crashlog::internal::parseViolationInfo(input->ExceptionRecord);
	bool isNonContinuable = input->ExceptionRecord->ExceptionFlags | EXCEPTION_NONCONTINUABLE;
	Address addressAt = parseAddress(input->ExceptionRecord->ExceptionAddress);

	ExceptionInfo info;
	memset(&info, 0, sizeof(info));
	info.exceptionCode = exceptionCode;
	info.violationInfo = violationInfo;
	info.isNonContinuable = isNonContinuable;
	info.addressAt = addressAt;
	return info;
}

std::string crashlog::exceptionInfoToString(ExceptionInfo const& info) {
	std::stringstream stream;

	Address address = info.addressAt;
	if (address.symbol.has_value()) {
		stream << std::format("Address={} ({})", address.symbol.value(), address.rawAddress) << std::endl;
	} else {
		stream << std::format("Address={}", address.rawAddress) << std::endl;
	}
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

std::string crashlog::createStackDumpString(EXCEPTION_POINTERS* ptr) {
	std::stringstream stream;
	stream << std::format("RAX=0x{:x}, RCX=0x{:x}, RDX=0x{:x}, RBX=0x{:x}", ptr->ContextRecord->Rax, ptr->ContextRecord->Rcx, ptr->ContextRecord->Rdx, ptr->ContextRecord->Rbx) << std::endl;
	stream << std::format("Rsp=0x{:x}, Rbp=0x{:x}, Rsi=0x{:x}, Rdi=0x{:x}", ptr->ContextRecord->Rsp, ptr->ContextRecord->Rbp, ptr->ContextRecord->Rsi, ptr->ContextRecord->Rdi) << std::endl;
	stream << std::format("R8=0x{:x}, R9=0x{:x}, R10=0x{:x}, R11=0x{:x}", ptr->ContextRecord->R8, ptr->ContextRecord->R9, ptr->ContextRecord->R10, ptr->ContextRecord->R11) << std::endl;
	stream << std::format("R9=0x{:x}, R10=0x{:x}, R11=0x{:x}, R12=0x{:x}", ptr->ContextRecord->R9, ptr->ContextRecord->R10, ptr->ContextRecord->R11, ptr->ContextRecord->R12) << std::endl;
	stream << std::format("R13=0x{:x} R14=0x{:x}, R15=0x{:x}, Rip=0x{:x}", ptr->ContextRecord->R13, ptr->ContextRecord->R14, ptr->ContextRecord->R15, ptr->ContextRecord->Rip) << std::endl;
	return stream.str();
}
