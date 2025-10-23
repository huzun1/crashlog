#include <Windows.h>

#include <cstdio>
#include <sstream>
#include <string>

#include "crashlog/address.hpp"
#include "crashlog/exception.hpp"

#define CRASHLOG_NO_WINDOWS_H
#include <crashlog/crashlog.hpp>

LONG WINAPI TopLevelExceptionFilter(struct _EXCEPTION_POINTERS* exceptionInfo) {
	printf("Exception occurred!\n\n");

	auto info = crashlog::parse(exceptionInfo);

	printf("========= Exception Info ==========\n");
	crashlog::ExceptionMetadata metadata = info.exceptionMetadata;
	printf("Exception At: %s\n", crashlog::addressToString(metadata.address).c_str());
	printf("Exception Code: %x (%s)\n", metadata.exceptionCode, metadata.exceptionName.c_str());
	for (const auto& [key, value] : metadata.additionalInfo) {
		std::string valString = std::visit([](auto&& arg) -> std::string {
			std::ostringstream oss;
			oss << arg;
			return oss.str();
		}, value);
		printf("  %s: %s\n", key.c_str(), valString.c_str());
	}

	printf("========= Stack Trace ===========\n");
	crashlog::StackTrace stackTrace = info.stacktrace;
	for (const auto& frame : stackTrace) {
		printf("%s\n", crashlog::addressToString(frame).c_str());
	}

	printf("=========== Registers ================\n");
	for (const auto& [regName, regValue] : info.registers) {
        printf("%s: %llx\n", regName.c_str(), regValue);
    }

	return 0;
}

int main() {
   	crashlog::initialize();

	printf("Creating exception handler -> %p\n", &TopLevelExceptionFilter);
	SetUnhandledExceptionFilter(&TopLevelExceptionFilter);

	printf("Throwing an exception: memset(nullptr, 0, 1)\n");
	memset(nullptr, 0, 1);

	return 0;
}
