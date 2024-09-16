/**
 * My assert 
 * * has color output in stderr, with more information and with option to make ending more harmless(not abort())
 * * Idea to have error_list.gen, where developer writes NAME_OF_ERROR and it's message, and after preprocessing we have enum and array of messages!
 * * USER_ERROR can't turn off and prints only message of error to user
 * @param check condition that checks for 0
 * @param error_code name of code error, defined in EnumOfErrors
 * @param ending instruction what to do at the end of assert(abort(),exit(),return)
*/

#ifndef UTILSC_DEBUG_MYASSERT_H
#define UTILSC_DEBUG_MYASSERT_H

#include <assert.h>
#include "../colors.h"
#include "errors_codegen.h"

#ifdef NDEBUG
#define MYASSERT(check, error_code, ending)                                                                                             \
        do                                                                                                                              \
        {                                                                                                                               \
            if (!(check))                                                                                                               \
            {                                                                                                                           \
                fprintf(stderr, RED "[Error]" MAGENTA " ## %s ## " RESET "\n", ArrayOfErrors[error_code]);                              \
                fprintf(stderr, RED "[%s:%d][%s]" RESET "\n",__FILE__,__LINE__,__PRETTY_FUNCTION__);    \
                fprintf(stderr, RED "[Condition]: {%s}\n" RESET, #check);                                                              \
                ending;                                                                                                                 \
            }                                                                                                                           \
        }                                                                                                                               \
        while(0);
#else   
#define MYASSERT(check, error_code, ending)
#endif

//===============================================================================================================
//Write Error unclosed
#define USER_ERROR(check, error_code, ending)                                                               \
        do                                                                                                  \
        {                                                                                                   \
            if (!(check))                                                                                   \
            {                                                                                               \
                fprintf(stderr, RED "[Error]" MAGENTA " ## %s ## " RESET "\n", ArrayOfErrors[error_code]);  \
                ending;                                                                                     \
            }                                                                                               \
        }                                                                                                   \
        while(0);
#endif