#include <Windows.h>

#include <cstdio>

#define CRASHLOG_WIN64
#define CRASHLOG_NO_WINDOWS_H
#include <crashlog/parser.hpp>

LONG WINAPI TopLevelExceptionFilter(struct _EXCEPTION_POINTERS *exceptionInfo) {
	crashlog::parser::initialize();
	return 0;
}

int main() {
	printf("Creating exception handler -> %p\n", &TopLevelExceptionFilter);
	SetUnhandledExceptionFilter(&TopLevelExceptionFilter);

	printf("Throwing an exception: memset(nullptr, 0, 1)\n");
	memset(nullptr, 0, 1);

	return 0;
}
