#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "log.h"

static FILE* log_file = NULL;
static void LoggingDtor(void);

static const char* LoggingMsgArr[] = {
    [kLoggingMsg_info]  = "  info",
    [kLoggingMsg_error] = " error",
    [kLoggingMsg_check] = " check",
    [kLoggingMsg_end]   = "   end",
};

LoggingStatus LoggingSetup(const char *log_file_name) {
    assert(log_file_name); // for debug
    if (log_file_name == NULL) { return kLoggingStatus_NullPassed; }

    FILE* try_open_log_file = fopen(log_file_name, "w");
    if (try_open_log_file == NULL) { return kLoggingStatus_CantOpenFile; }

    int set_buf_status = setvbuf(try_open_log_file, NULL, _IONBF, 0);
    if (set_buf_status) {
        fclose(try_open_log_file);
        return kLoggingStatus_InternalError; 
    }

    int atexit_status = atexit(LoggingDtor);
    if (atexit_status) {
        fclose(try_open_log_file);
        return kLoggingStatus_InternalError;
    }

    log_file = try_open_log_file;

    return kLoggingStatus_Ok;
}

LoggingStatus LogHidden(const char* source_file_name, 
                        const int source_line_num, 
                        const char* source_func_name, 
                        const LoggingMsg code_msg,    
                        const char* format_str,  
                        ...) 
{
    assert(format_str       != NULL);
    assert(source_file_name != NULL);
    assert(source_line_num  >= 0);
    assert(source_func_name != NULL);

    if ((format_str == NULL)
        || (source_file_name == NULL)
        || (source_func_name == NULL)
        || (source_line_num < 0)) 
    { return kLoggingStatus_NullPassed; }
    
    if (log_file == NULL) { return kLoggingStatus_UninitLog; }

#if defined (NDEBUG)
    //time
    //time_t current_time = time(NULL);
    //if (current_time == (time_t)-1) {
    //    return kLoggingStatus_InternalError;
    //}
    //struct tm* current_tm = localtime(&current_time);
    
    // Get current time
    struct timespec ts = {};
    if (clock_gettime(CLOCK_REALTIME, &ts)) return kLoggingStatus_InternalError;
    // Extract hours, minutes, seconds, milliseconds, and microseconds
    long int hours, minutes, seconds, milliseconds, microseconds;
    seconds      = (ts.tv_sec % 60);
    minutes      = (ts.tv_sec / 60) % 60;
    hours        = (ts.tv_sec / 3600) % 24;
    milliseconds = (ts.tv_nsec / 1000000);
    microseconds = (ts.tv_nsec % 1000000) / 1000;

    //fprintf
    va_list args;
    va_start(args, format_str);

    fprintf(log_file, 
            "[%s][%02ld:%02ld:%02ld:%03ld:%03ld][{%s}:{%s}:{%d}]:[",
            LoggingMsgArr[code_msg],
            hours,
            minutes,
            seconds,
            milliseconds,
            microseconds,
            source_file_name, 
            source_func_name, 
            source_line_num);
    vfprintf(log_file, format_str, args);
    fprintf(log_file, "]\n");

    va_end(args);
#else 
    // do nothing    
#endif // NDEBUG
    return kLoggingStatus_Ok;
}

const char* LogErrorToStr(LoggingStatus log_status) {
    switch (log_status) {
        case kLoggingStatus_Ok:            return STRINGIFY(kLoggingStatus_Ok);

        case kLoggingStatus_UninitEnum:    return STRINGIFY(kLoggingStatus_UninitEnum);
        
        case kLoggingStatus_NullPassed:    return STRINGIFY(kLoggingStatus_NullPassed);
        case kLoggingStatus_CantOpenFile:  return STRINGIFY(kLoggingStatus_CantOpenFile);
        case kLoggingStatus_UninitLog:     return STRINGIFY(kLoggingStatus_UninitLog);
        case kLoggingStatus_InternalError: return STRINGIFY(kLoggingStatus_InternalError); 
        default: assert(0 && "unknown enum value"); return "unknown enum value";
    }
}

static void LoggingDtor(void) {
    if (log_file == NULL) { return ; }

    Log(kLoggingMsg_end, "Destruction Complete");

    fclose(log_file);
    log_file = NULL;
}
