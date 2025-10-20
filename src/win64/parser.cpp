#include "crashlog/win64/parser.hpp"

#include <DbgHelp.h>
#include <psapi.h>

#include "crashlog/win64/symbol_resolver.hpp"

#pragma comment(lib, "DbgHelp.lib")

void crashlog::win64::initialize() {
	SymSetOptions(SYMOPT_LOAD_LINES);
	SymInitialize(GetCurrentProcess(), NULL, FALSE);

	crashlog::win64::SymbolResolver::loadAllSymbols();
}

crashlog::Exception crashlog::win64::parse(EXCEPTION_POINTERS* ptr) {
}
