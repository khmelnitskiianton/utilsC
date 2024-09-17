# utilsC
some patterns/utils for C projects

+ Universal Makefile
+ `colors.h`
+ [Guide how to make cool terminal!!!](./Terminal/README_TERMINAL.md)

Turn on with `NDEBUG`

## MY_ASSERT (Debug/myassert.h)

Has color output in stderr, with more information and with option to make ending more harmless(not abort()) \
Idea to have error_list.gen, where developer writes NAME_OF_ERROR and it's message, and after preprocessing we have enum and array of messages!

> USER_ERROR can't turn off and prints only message of error to user

+ @param check condition that checks for 0
+ @param error_code name of code error, defined in EnumOfErrors
+ @param ending instruction what to do at the end of assert(abort(),exit(),return)

## DUMP (Debug/dump.h)

Just many macros of colorful print to `stderr`

```cpp
DUMP_PRINT(...)   
DUMP_VALUE(...)   
DUMP_ERROR(...)   
DUMP_SUCCESS(...) 
IF_DUMP_DEBUG(...)
```

## LOGGER (Debug/log.cpp, Debug/log.h)

Logger writes all in log file with much information

**Debug macros:**
```cpp
//Main macro
#define Log(...) // expects: enum msg, format, value 

//Helpful
//Points
#define LogFunctionEntry()
#define LogBranchEntry()

//Msg
#define LogError(error_)
#define LogBoolResult(expr_)
#define LogVariable(format_, value_)
```

**Example:**
```cpp
#include "Debug/log.h"
int main()
{
    //Logger   
    LoggingSetup("logs.txt");
    int   x = 0;
    char* p = "abba";
    const char *string = LogErrorToStr(LogFunctionEntry());
    LogVariable("%d", x);
    LogVariable("%s", p);   
    return 0;
}
```
*Result:*
```cpp
[ check][15:31:39:120:586][{main.cpp}:{int main()}:{11}]:[Function entry]
[  info][15:31:39:120:776][{main.cpp}:{int main()}:{13}]:[x: 0]
[  info][15:31:39:120:954][{main.cpp}:{int main()}:{14}]:[p: abba]
[   end][15:31:39:121:124][{./Debug/log.cpp}:{void LoggingDtor()}:{128}]:[Destruction Complete]
```
