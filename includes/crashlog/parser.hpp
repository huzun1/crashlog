#pragma once

#include "crashlog/exception.hpp" // IWYU pragma: keep

namespace crashlog {
namespace parser {
void initialize();

#ifdef CRASHLOG_WIN64
class EXCEPTION_POINTERS;
crashlog::Exception parse(EXCEPTION_POINTERS* ptr);
#endif
};	// namespace parser
};	// namespace crashlog
