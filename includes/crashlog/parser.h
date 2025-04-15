#pragma once

#include <Windows.h>

#include "exception_info.h"

namespace crashlog {
struct StackDump {
};

void initSym();

ExceptionInfo parseException(EXCEPTION_POINTERS *);

std::string exceptionInfoToString(ExceptionInfo const &);

std::string createStackTraceString(ExceptionInfo const &);

std::string createStackDumpString(EXCEPTION_POINTERS *);
};	// namespace crashlog
