#include "parser.h"

#include <minwindef.h>
#include <winnt.h>

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
