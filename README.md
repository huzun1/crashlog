# Crashlog

> [!CAUTION]
> This library currently supports only for Windows x64 applications

Simple library to convert exceptions raised at the top level into immediately readable

# Example of output:
```
Creating exception handler -> 00007FF7926C1000
Throwing an exception: memset(nullptr, 0, 1)

======================================
An exception has been occured!
Address=memset (0x7ffa0ee402fe)
Type=EXCEPTION_ACCESS_VIOLATION (0xc0000005)
 - ViolationName=WRITE_ACCESS_VIOLATION
 - ViolationAddress=0x0

Stack Trace:
memset (0x7ffa0ee402fe)
C:\Users\ericdoa\Documents\Proj\Cpp\crashlog\test\main.cpp L29 - main (0x7ff7926c11a9)
D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl L79 - invoke_main (0x7ff792728039)
D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl L288 - __scrt_common_main_seh (0x7ff792728172)
D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl L331 - __scrt_common_main (0x7ff7927281fe)
D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp L17 - mainCRTStartup (0x7ff79272821e)
BaseThreadInitThunk (0x7ffa49af7374)
RtlUserThreadStart (0x7ffa49c3cc91)

Stack Dumps:
RAX=0x000000000000, RCX=0x000000000001, RDX=0x000000000000, RBX=0x000000000000
Rsp=0x00770c8ff938, Rbp=0x000000000000, Rsi=0x000000000000, Rdi=0x000000000000
R8=0x000000000001, R9=0x7ffa0ee402fe, R10=0x7ffa0ee20000, R11=0x000000000000
R9=0x7ffa0ee402fe, R10=0x7ffa0ee20000, R11=0x000000000000, R12=0x000000000000
R13=0x000000000000 R14=0x000000000000, R15=0x000000000000, Rip=0x7ffa0ee402fe
```

# Quick Start

- Add these lines to your `CMakeLists.txt`
```cmake
FetchContent_Declare(
  crashlog
  GIT_REPOSITORY https://github.com/0liteyear/crashlog.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(crashlog)
~
target_link_libraries(YOUR_PROJECT crashlog)
```
- Add an exception filter by using `SetUnhandledExceptionFilter`
- Just use it.
```cpp
#include <crashlog/parser.h>
#include <windows.h>

#include <cstdio>

LONG WINAPI TopLevelExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo) {
	crashlog::initSym();
	crashlog::loadSym();

	auto info = crashlog::parseException(ExceptionInfo);
	printf("\n======================================\n");
	printf("An exception has been occured!\n");
	printf("%s\n", crashlog::exceptionInfoToString(info).c_str());
	printf("Stack Trace:\n");
	printf("%s\n", crashlog::createStackTraceString(info).c_str());
	printf("Stack Dumps:\n");
	printf("%s\n", crashlog::createStackDumpString(ExceptionInfo).c_str());

	return 0;
}

int main() {
	printf("Creating exception handler -> %p\n", &TopLevelExceptionFilter);
	SetUnhandledExceptionFilter(&TopLevelExceptionFilter);

	printf("Throwing an exception: memset(nullptr, 0, 1)\n");
	memset(nullptr, 0, 1);

	return 0;
}
```
