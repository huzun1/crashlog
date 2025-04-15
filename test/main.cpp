#include <windows.h>

#include <cstdio>

#include "../src/parser.h"

LONG WINAPI TopLevelExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo) {
	auto info = crashlog::parseException(ExceptionInfo);
	printf("\n======================================\n");
	printf("An exception has been occured!\n");
	printf("%s\n", crashlog::exceptionInfoToString(info).c_str());

	return 0;
}

int main() {
	printf("Creating exception handler -> %p\n", &TopLevelExceptionFilter);
	SetUnhandledExceptionFilter(&TopLevelExceptionFilter);

	printf("Throwing an exception: memset(nullptr, 0, 1)\n");
	memset(nullptr, 0, 1);

	return 0;
}
