#pragma once

#include <Windows.h>
#include <winnt.h>

#include <crashlog/exception.hpp>

namespace crashlog {
namespace win64 {
    void initialize();

    crashlog::Exception parse(EXCEPTION_POINTERS* ptr);
};
};	// namespace crashlog
