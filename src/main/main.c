/**
 *  @file      main.c
 *  @brief     Main function.
 *  @author    fire-peregrine
 *  @date      2020/11/01
 *  @copyright Copyright (C) peregrine all rights reserved.
 *  @license   Released under the MIT License.
 */

#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include "ArgParser.h"

/* Macros */
/**
 *  @brief Maximum string buffer size
 */
#define MAX_STRING_BUF   32


#define SUCCESS 0
#define FAILURE 1


/* Structs */
typedef struct Config_
{
    int intParam;
    unsigned int uintParam;
    char stringParam[MAX_STRING_BUF];
    float floatParam;
    double doubleParam; 
    int posParam; 
} Config;

/* Signatures */


/* Functions */
/**
 *  @brief Main function
 *  @param [in] argc     Number of command line arguments
 *  @param [in] argv[]   Command line argument array
 *  @return   Execution status
 */
int main(int argc, char *argv[])
{
    int status;
    int intParam1;
    int intParam2;
    unsigned int uintParam;
    bool boolParam;
    bool swParam;
    char stringParam[32];
    float floatParam;
    double doubleParam;

    /* Create ArgParser object. */
    ArgParser *aparser = ArgParser_new(
            "example_program"            /* Program name        */,
            "This is a example program." /* Program description */);
    if(aparser == NULL)
        return FAILURE;

    /* Add version string. */
    status = ArgParser_addVersion(aparser, "v1.0.0");
    if(status != 0)
        goto error;
    
    /* Add release date. */
    status = ArgParser_addDate(aparser, "2020/11/01");
    if(status != 0)
        goto error;
    
    /* Add author name. */
    status = ArgParser_addAuthor(aparser, "John Doe");
    if(status != 0)
        goto error;

    /* Add int-type optional parameter. */
    status = ArgParser_addInt(aparser,
            &intParam1                             /* destination    */,
            100                                    /* default value  */,
            "-i"                                   /* short option   */,
            "--intparam"                           /* long option    */,
            "optional_param"                       /* parameter name */,
            "This is int-type optional parameter." /* parameter description */);
    if(status != 0)
        goto error;
    
    /* Add int-type positional parameter. */
    status = ArgParser_addInt(aparser,
            &intParam2                              /* destination    */,
            200                                     /* default value  */,
            NULL                                    /* no option      */,
            NULL                                    /* no option      */,
            "positional_param"                      /* parameter name */,
            "This is int-type positinal parameter." /* parameter description */);
    if(status != 0)
        goto error;
    
    /* Add unsigned-int-type optional parameter. */
    status = ArgParser_addUInt(aparser,
            &uintParam                                       /* destination    */,
            300                                              /* default value  */,
            "-u"                                             /* short option   */,
            "-uintparam"                                     /* long option    */,
            "uint_param"                                     /* parameter name */,
            "This is unsigned-int-type optional  parameter." /* parameter description */);
    if(status != 0)
        goto error;
    
    /* Add string-type optional parameter. */
    status = ArgParser_addString(aparser,
            stringParam                               /* destination    */,
            "default_string"                          /* default value  */,
            32                                        /* max length     */,
            "-s"                                      /* short option   */,
            "-stringparam"                            /* long option    */,
            "string_param"                            /* parameter name */,
            "This is string-type optional parameter." /* parameter description */);
    if(status != 0)
        goto error;
    
    
    /* Add bool-type optional parameter. */
    status = ArgParser_addBool(aparser,
            &boolParam                              /* destination    */,
            false                                   /* default value  */,
            "-b"                                    /* short option   */,
            "--boolparam"                           /* long option    */,
            "bool_param"                            /* parameter name */,
            "This is bool-type optional parameter." /* parameter description */);
    if(status != 0)
        goto error;
    
    /* Add float-type optional parameter. */
    status = ArgParser_addFloat(aparser,
            &floatParam                              /* destination    */,
            123.45                                   /* default value  */,
            "-f"                                     /* short option   */,
            "--floatparam"                           /* long option    */,
            "float_param"                            /* parameter name */,
            "This is float-type optional parameter." /* parameter description */);
    if(status != 0)
        goto error;
    
    /* Add double-type optional parameter. */
    status = ArgParser_addDouble(aparser,
            &doubleParam                              /* destination    */,
            123.45                                    /* default value  */,
            "-d"                                      /* short option   */,
            "--doubleparam"                           /* long option    */,
            "double_param"                            /* parameter name */,
            "This is double-type optional parameter." /* parameter description */);
    if(status != 0)
        goto error;

    /* Add switch-type optional parameter. */
    status = ArgParser_addTrue(aparser,
            &swParam                                  /* destination    */,
            "-w"                                      /* short option   */,
            "--switchparam"                           /* long option    */,
            "switch_param"                            /* parameter name */,
            "This is switch-type optional parameter." /* parameter description */);
    if(status != 0)
        goto error;

    /* Parse command line arguments. */
    status = ArgParser_parse(aparser, argc, argv);
    if(status != 0)
        goto error;

    /* Print parse result. */
    printf("intParam1 = %d\n", intParam1);
    printf("intParam2 = %d\n", intParam2);
    
    if(aparser != NULL)
    {
        const char *errMsg = ArgParser_getErrorMsg(aparser);
        fprintf(stderr, "Info: %s\n", errMsg);
    }

    ArgParser_delete(aparser);
    
    fprintf(stderr, "***** Config *****\n");
    fprintf(stderr, "intParam1   : %d\n",    intParam1  ); 
    fprintf(stderr, "intParam2   : %d\n",    intParam2  );
    fprintf(stderr, "uintParam   : %u\n",    uintParam  );
    fprintf(stderr, "boolParam   : %d\n",    boolParam  );
    fprintf(stderr, "swParam     : %d\n",    swParam    );
    fprintf(stderr, "stringParam : '%s'\n",  stringParam);
    fprintf(stderr, "floatParam  : %f\n",    floatParam );
    fprintf(stderr, "doubleParam : %lf\n",   doubleParam);
    return 0;

error: /* error handling */

    if(aparser != NULL)
    {
        const char *errMsg = ArgParser_getErrorMsg(aparser);
        fprintf(stderr, "Error: %s\n", errMsg);
    }

    ArgParser_delete(aparser);
    return FAILURE;
}


