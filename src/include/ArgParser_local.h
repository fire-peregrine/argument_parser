/**
 *  @file      ArgParser_local.h
 *  @brief     Argument Parser, local headee file.
 *  @author    fire-peregrine
 *  @date      2020/10/26
 *  @copyright Copyright (C) fire-peregrine all rights reserved.
 */

#ifndef SRC_ARG_PARSER_LOCAL_H_
#define SRC_ARG_PARSER_LOCAL_H_

#include <stdbool.h>

/* Macros */
/**
 *  @brief Buffer size to store string data.
 */
#define APARSER_MAX_BUF      0x1000

/**
 *  @brief Maximum size of an error message.
 */
#define APARSER_MAX_ERROR_MSG   256

/**
 *  @brief Maximum number of optional/positional parameters.
 */
#define APARSER_MAX_ARG_PRMS     32


/* Enums */
/**
 *  @brief Variable type definition
 */
typedef enum VarType_
{
    VarType_Int    = 0, ///< int type
    VarType_UInt   = 1, ///< unsigned int type
    VarType_String = 2, ///< string type (char *)
    VarType_Bool   = 3, ///< bool type
    VarType_Int32  = 4, ///< int32_t type
    VarType_UInt32 = 5, ///< uint32_t type
    VarType_Float  = 6, ///< float type
    VarType_Double = 7, ///< double type
    VarType_True   = 8, ///< Switch type
    VarType_Num    = 9  ///< Number of definitions

} VarType;


/**
 *  @brief ArgumentType
 */
typedef enum ArgType_
{
    ArgType_Opt   = 0, ///< Option. A token which starts with '-' or '--' and has one or more characters.
    ArgType_NoOpt = 1, ///< Not an option. A token which doesn't start with either '-' or '--'.
    ArgType_Error = 2  ///< Invalid argument.

} ArgType;


/* Unions */
/**
 *  @brief Union to store variable-type data.
 */
typedef union Val_
{
    int i;                  ///< for int-type
    unsigned int u;         ///< for unsigned int-type
    struct str
    {
        char *data;         ///< string data
        unsigned int len;   ///< string length
    } s;                    ///< for string-type
    bool b;                 ///< for bool-type
    int32_t i32;            ///< for int32_t-type
    uint32_t u32;           ///< for uint32_t-type
    float f;                ///< for float-type
    double d;               ///< for double-type

} Val;


/* Structs */
/**
 *  @brief Parameter definition structure
 */
typedef struct PrmDef_
{
    char    *sOpt;    ///< Short option
    char    *lOpt;    ///< Long option
    char    *name;    ///< Name
    char    *desc;    ///< Description
    VarType  varType; ///< Variable type
    void    *dest;    ///< Destinationp pointer
    Val      defVal;  ///< Default value
} PrmDef;


/* Class */
/**
 *  @brief   Argument parser object structure
 */
struct ArgParser_
{
    /* Program Definitions */
    char *progName;                          ///< Program name
    char *progDesc;                          ///< Program description
    char *version;                           ///< Version
    char *date;                              ///< Release date
    char *author;                            ///< Author name
    
    /* Special Options */
    bool isHelpSpecified;                    ///< Show help message.
    bool isVerSpecified;                     ///< Show version string.

    bool reqFullPosParams;                   ///< If set, the parser requires all positional parameters.

    /* Argument Definitions */
    unsigned int numOptPrms;                 ///< Number of optional parameters.
    PrmDef optPrms[APARSER_MAX_ARG_PRMS];     ///< Optional parameters.
    unsigned int numPosPrms;                 ///< Number of positional parameters.
    PrmDef posPrms[APARSER_MAX_ARG_PRMS];     ///< Positional parameters.
    
    /* Error stauts */
    bool hasError;                           ///< Error flag.
    char errorMsg[APARSER_MAX_ERROR_MSG];    ///< Error message.


    /* Temporary Buffer */
    unsigned int bufIdx;                     ///< Current buffer index
    char buf[APARSER_MAX_BUF];               ///< Temporary buffer to store string data.

};


#endif // SRC_ARG_PARSER_LOCAL_H_


