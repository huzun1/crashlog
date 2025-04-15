#include "internal.h"

using namespace crashlog;

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
