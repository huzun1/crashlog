# Crashlog

> [!CAUTION]
> This library currently supports only for Windows x64 applications

Simple library to convert exceptions raised at the top level into immediately readable

# Example of output:
```
Creating exception handler -> 00007FF617BE1000
Throwing an exception: memset(nullptr, 0, 1)
Exception occurred!

========= Exception Info ==========
Exception At: memset (VCRUNTIME140D + 0x0x225ee)
Exception Code: c0000005 (EXCEPTION_ACCESS_VIOLATION)
  violation_type_name: WRITE
  violation_type: 1
  address: 0
========= Stack Trace ===========
memset (VCRUNTIME140D + 0x0x225ee)
main (crashlog_test + 0x0x169e) [C:\Users\babar\dev\cpp\crashlog\test\main.cpp:54]
invoke_main (crashlog_test + 0x0x87839) [D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:79]
__scrt_common_main_seh (crashlog_test + 0x0x87972) [D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288]
__scrt_common_main (crashlog_test + 0x0x879fe) [D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:331]
mainCRTStartup (crashlog_test + 0x0x87a1e) [D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:17]
BaseThreadInitThunk (KERNEL32 + 0x0x2e8d7)
RtlUserThreadStart (ntdll + 0x0x8c53c)
=========== Registers ================
rax: 0
rbp: 0
rbx: 0
rsp: 1000ff878
rdi: 0
rcx: 1
rsi: 0
rdx: 0
r8: 1
r9: 7ffab3a325ee
r10: 7ffab3a10000
r11: 0
r12: 0
r13: 0
r14: 0
r15: 0
rip: 7ffab3a325ee
```

# TODO
- Win32 support
- XMM registers? but idk who need it

# Quick Start

- Add these lines to your `CMakeLists.txt`
```cmake
FetchContent_Declare(
  crashlog
  GIT_REPOSITORY https://github.com/huzun1/crashlog.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(crashlog)
~
target_link_libraries(YOUR_PROJECT crashlog)
```

- Add an exception filter by using `SetUnhandledExceptionFilter`
- Now, Just use it 🚀

```cpp
#include <Windows.h>

#include <cstdio>
#include <sstream>
#include <string>

#define CRASHLOG_NO_WINDOWS_H
#include <crashlog/crashlog.hpp>
#include <crashlog/address.hpp>
#include <crashlog/exception.hpp>

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
```
