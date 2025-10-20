#include "crashlog/parser.hpp"

#include <stdexcept>

#ifdef CRASHLOG_WIN64
#include "crashlog/win64/parser.hpp"
#endif

using namespace crashlog;

void parser::initialize() {
	bool initialized = false;

	// win64
#ifdef CRASHLOG_WIN64
	crashlog::win64::initialize();
	initialized = true;
#endif

	if (!initialized) {
		throw std::runtime_error("No platform support");
	}
}

#ifdef CRASHLOG_WIN64
crashlog::Exception parse(EXCEPTION_POINTERS* ptr) {
	return crashlog::win64::parse(ptr);
}
#endif
