/**
 *  @file      main.c
 *  @brief     Main function.
 *  @author    fire-peregrine
 *  @date      2020/11/01
 *  @copyright Copyright (C) peregrine all rights reserved.
 *  @license   Released under the MIT License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArgParser.h"

/* Macros */
/**
 *  @brief Maximum string buffer size
 */
#define MAX_STRING_BUF   32

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
static int printConfig(Config *config);


/**
 *  @brief Main function
 *  @param [in] argc     Number of command line arguments
 *  @param [in] argv[]   Command line argument array
 *  @return   Execution status
 */
int main(int argc, char *argv[])
{
    int status;
    Config config;
    memset(&config, 0xAA, sizeof(Config));

    /* Create ArgParser object. */
    ArgParser *aparser = ArgParser_new("program", "description");
    if(aparser == NULL)
        goto error;

    /* Add program version string. */
    status = ArgParser_addVersion(aparser, "v1.0.0");
    if(status != 0)
        goto error;

    /* Add program author name. */
    status = ArgParser_addAuthor(aparser, "Your Name");
    if(status != 0)
        goto error;

    /* Add release date. */
    status = ArgParser_addDate(aparser, "2020/11/01/Mon");
    if(status != 0)
        goto error;

    /* Add int-type optional parameter. */
    status = ArgParser_addInt(aparser,
            &config.intParam, -123, "-i", "--iparam", "int_param",
            "This is int type parameter.");
    if(status != 0)
        goto error;

    /* Add uint-type optional parameter. */
    status = ArgParser_addUInt(aparser,
            &config.uintParam, 9999, "-u", "--uparam", "uint_param",
            "This is unsigned int type parameter.");
    if(status != 0)
        goto error;

    /* Add string-type optional parameter. */
    status = ArgParser_addString(aparser,
            config.stringParam, "hello", MAX_STRING_BUF, "-s", "--sparam", "string_param",
            "This is string type parameter.");
    if(status != 0)
        goto error;

    /* Add float-type optional parameter. */
    status = ArgParser_addFloat(aparser,
            &config.floatParam, 222.22, "-f", "--fparam", "float_param",
            "This is float type parameter.");
    if(status != 0)
        goto error;
    
    /* Add double-type optional parameter. */
    status = ArgParser_addDouble(aparser,
            &config.doubleParam, 333.33, "-d", "--dparam", "double_param",
            "This is double type parameter.");
    if(status != 0)
        goto error;


    /* Add int-type positional parameter. */
    status = ArgParser_addInt(aparser,
            &config.posParam, -123, NULL, NULL, "int_pos_param",
            "This is int type positional parameter.");
    if(status != 0)
        goto error;


    /* Parse command line arguments. */
    status = ArgParser_parse(aparser, argc, argv);
    if(status != 0)
        goto error;

    ArgParser_delete(aparser);
    fprintf(stderr, "OK!\n");
    printConfig(&config);
    return 0;

error: /* error handling */

    fprintf(stderr, "Error: %s\n", ArgParser_getErrorMsg(aparser));

    ArgParser_delete(aparser);
    // printConfig(&config);
    return 1;
}


static int printConfig(Config *config)
{
    fprintf(stderr, "***** Config *****\n");
    fprintf(stderr, "intParam    : %d\n",  config->intParam   );
    fprintf(stderr, "uintParam   : %d\n",  config->uintParam  );
    fprintf(stderr, "stringParam : %s\n",  config->stringParam);
    fprintf(stderr, "floatParam  : %f\n",  config->floatParam );
    fprintf(stderr, "doubleParam : %lf\n", config->doubleParam);
    fprintf(stderr, "posParam    : %d\n",  config->posParam   );
    return 0;    
}

