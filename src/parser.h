#pragma once

#include <Windows.h>

#include "exception_info.h"

namespace crashlog {
ExceptionInfo parseException(EXCEPTION_POINTERS *);

std::string exceptionInfoToString(ExceptionInfo const &);
};	// namespace crashlog
