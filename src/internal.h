#pragma once

#include <Windows.h>

#include <optional>

#include "exception_info.h"

namespace crashlog {
namespace internal {
std::optional<AccessViolationInfo> parseViolationInfo(EXCEPTION_RECORD* record);

AccessViolationType parseViolationType(uint64_t type);
};	// namespace internal
};	// namespace crashlog
