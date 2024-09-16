#ifndef UTILSC_DEBUG_LOG_H
#define UTILSC_DEBUG_LOG_H

//===================================================================
//__FUNCTION__ __PRETTY_FUNCTION__
#define FUNCTION_NAME __func__

#define USE_VAR(var) (void)var

#if !defined(STRINGIFY)
    #define STRINGIFY(x) #x
#endif

//===================================================================

typedef enum LoggingStatus {
    kLoggingStatus_Ok            = -1,
    
    kLoggingStatus_UninitEnum    =  0,

    kLoggingStatus_NullPassed    =  1,
    kLoggingStatus_CantOpenFile  =  2,
    kLoggingStatus_UninitLog     =  3,
    kLoggingStatus_InternalError =  4,
} LoggingStatus;

typedef enum LoggingMsg {
    kLoggingMsg_info  =  0,
    kLoggingMsg_error =  1, 
    kLoggingMsg_check =  2,
    kLoggingMsg_end   =  3,
} LoggingMsg; 

LoggingStatus LogHidden(const char* source_file_name,
                        const int source_line_num, 
                        const char* source_func_name,
                        const LoggingMsg code_msg,    
                        const char* format_str, 
                        ...) __attribute__(( format(printf, 5, 6)) );

LoggingStatus LoggingSetup(const char* log_file_name);
const char* LogErrorToStr(LoggingStatus log_status);

//===================================================================

#define Log(...) LogHidden(__FILE__, __LINE__, FUNCTION_NAME, __VA_ARGS__)

#define LogFunctionEntry()              Log(kLoggingMsg_check, "Function entry")
#define LogBranchEntry()                Log(kLoggingMsg_check, "Branch entry")

#define LogError(error_)                Log(kLoggingMsg_error, "Error encountered: %s", STRINGIFY(error_))
#define LogBoolResult(expr_)            Log(kLoggingMsg_check, #expr_ ": %d", (int)(expr_))
#define LogVariable(format_, value_)    Log(kLoggingMsg_info,  #value_ ": " format_, value_)

#endif // log_h