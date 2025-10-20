#pragma once

#ifndef CRASHLOG_NO_WINDOWS_H
#include "Windows.h"  // IWYU pragma: keep
#endif

#include "crashlog/exception.hpp" // IWYU pragma: keep

namespace crashlog {
namespace parser {
void initialize();

#ifdef CRASHLOG_WIN64
crashlog::Exception parse(EXCEPTION_POINTERS* ptr);
#endif
};	// namespace parser
};	// namespace crashlog
