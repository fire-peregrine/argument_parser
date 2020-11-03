# Argument parser for C language.

This is a simple argument parser module for C language.

## The aim of this parser

1. Simple, easy-to-use argument parsing.
2. Readability of the source code.
3. Maintainability. Easy to add new options and easy to remove the deprecated options, through entire software development.
4. Automatic help message generation, because it's a bother.
5. Removing dependencies to other components such as 'getopt', with only a few source/header files.
6. (Just for my study, using github...)

## Example
The following code is a example program which takes one *int-type* optional parameter and one *int-type* positional parameter.

```C
#include <stdio.h>
#include "ArgParser.h"

int main(int argc, char **argv)
{
    int status;
    int intParam1;
    int intParam2;

    /* Create ArgParser object. */
    ArgParser *aparser = ArgParser_new(
            "example_program"            /* program name        */,
            "This is a example program." /* program description */);
    if(aparser == NULL)
        return FAILURE;

    /* Add int-type optional parameter. */
    status = ArgParser_addInt(aparser,
            &intParam1                             /* destination    */,
            100                                    /* default value  */,
            "-i"                                   /* short option   */,
            "--intparam"                           /* long option    */,
            "optional_param"                       /* parameter name */,
            "This is int-type optional parameter." /* parameter description */);
    if(status != SUCCESS)
        goto error;
    
    /* Add int-type positional parameter. */
    status = ArgParser_addInt(aparser,
            &intParam2                              /* destination    */,
            200                                     /* default value  */,
            NULL                                    /* no option      */,
            NULL                                    /* no option      */,
            "positional_param"                      /* parameter name */,
            "This is int-type positinal parameter." /* parameter description */);
    if(status != SUCCESS)
        goto error;

    /* Parse command line arguments. */
    status = ArgParser_parse(aparser, argc, argv);
    if(status != 0)
        goto error;

    /* Print parse result. */
    printf("intParam1 = %d\n", intParam1);
    printf("intParam2 = %d\n", intParam2);

    ArgParser_delete(aparser);

    /* ... */
    return SUCCESS;

error: /* error handling */

    ArgParser_delete(aparser);
    return FAILURE;
}
```

## Displaying help message
Compile the program, and run with `-h` or `--help` option.
Help message is automatically formatted and diaplayed in the terminal.

```SHELL
% ./example_program

Usage   : example_program [-h/--help] [-v/--version] (optional_parameters ...) [positional_param] 

Optional Parameters:

    -h / --help : help
    | description:
    |    Show help message.

    -v / --version : version
    | description:
    |    Show version string.

    -i [int] / --intparam [int] : optional_param
    | description:
    |    This is int-type optional parameter.

Positional Parameter:

    [int] : positional_param
    | description:
    |    This is int-type positinal parameter.
```

## Supported data types
This argument parser supports the following data types.
1. `int` type
2. `unsigned int` type
3. `string (char *)` type
4. `bool` type
5. `int32_t` type
6. `uint32_t` type
7. `float` type
8. `double` type
9. *switch* type, which works as a flag option and doesn't take following arguments.

## Detailed usage

### Creating the Argument Parser
Before using the argument parser, a new argument parser object must be created, via the foillowing function with program name and program description.
The program name and its description are displayed in the help message.
```C
    /* Create ArgParser object. */
    ArgParser *aparser = ArgParser_new(
            "example_program"            /* program name        */,
            "This is a example program." /* program description */);
```

### Adding version string.
```C
    /* Add version string. */
    status = ArgParser_addVersion(aparser, "v1.0.0" /* version string */);
```

### Adding author name.
```C
    /* Add author name. */
    status = ArgParser_addAuthor(aparser, "John Doe" /* author name */);
```

### Adding release date.
```C
    /* Add release date. */
    status = ArgParser_addDate(aparser, "2020/11/01" /* release date */);
```
    
### Displaying version information

Run with `-v` or `--version` option.
Version information is automatically formatted and shown in the terminal.

```SHELL
% ./bin/arg_parser_test -v
example_program v1.0.0
written by John Doe
released on 2020/11/01
```

### Adding int type optional/positional parameter
```C
    /* Add int-type optional parameter. */
    int intParam;
    status = ArgParser_addInt(aparser,
            &intParam                              /* destination    */,
            100                                    /* default value  */,
            "-i"                                   /* short option   */,
            "--intparam"                           /* long option    */,
            "optional_param"                       /* parameter name */,
            "This is int-type optional parameter." /* parameter description */);
```

### Adding unsigned int type optional/positional parameter
```C
    /* Add unsigned-int-type optional parameter. */
    unsigned int uintParam;
    status = ArgParser_addUInt(aparser,
            &uintParam                                      /* destination    */,
            300                                             /* default value  */,
            "-u"                                            /* short option   */,
            "-uintparam"                                    /* long option    */,
            "positional_param"                              /* parameter name */,
            "This is unsigned-int-type optional parameter." /* parameter description */);
```

### Adding string type optional/positional parameter
```C
    /* Add string-type optional parameter. */
    char stringParam[32];
    status = ArgParser_addString(aparser,
            &stringParam                              /* destination    */,
            "default_string"                          /* default value  */,
            32                                        /* max length     */,
            "-s"                                      /* short option   */,
            "-stringparam"                            /* long option    */,
            "string_param"                            /* parameter name */,
            "This is string-type optional parameter." /* parameter description */);
```

### Adding bool type optional/positional parameter
```C
    /* Add bool-type optional parameter. */
    bool boolParam;
    status = ArgParser_addBool(aparser,
            &boolParam                              /* destination    */,
            false                                   /* default value  */,
            "-b"                                    /* short option   */,
            "--boolparam"                           /* long option    */,
            "bool_param"                            /* parameter name */,
            "This is bool-type optional parameter." /* parameter description */);
```

### Adding float type optional/positional parameter
```C
    /* Add float-type optional parameter. */
    float floatParam;
    status = ArgParser_addFloat(aparser,
            &floatParam                              /* destination    */,
            123.45                                   /* default value  */,
            "-f"                                     /* short option   */,
            "--floatparam"                           /* long option    */,
            "float_param"                            /* parameter name */,
            "This is float-type optional parameter." /* parameter description */);
```

### Adding double type optional/positional parameter
```C
    /* Add double-type optional parameter. */
    double doubleParam;
    status = ArgParser_addDouble(aparser,
            &doubleParam                             /* destination    */,
            123.45                                   /* default value  */,
            "-d"                                     /* short option   */,
            "--doubleparam"                          /* long option    */,
            "double_param"                           /* parameter name */,
            "This is double-type optional parameter." /* parameter description */);
```

### Adding switch type optional/positional parameter
```C
    /* Add switch-type optional parameter. */
    bool swParam;
    status = ArgParser_addTrue(aparser,
            &swParam                                  /* destination    */,
            "-s"                                      /* short option   */,
            "--switchparam"                           /* long option    */,
            "switch_param"                            /* parameter name */,
            "This is switch-type optional parameter." /* parameter description */);
```


### Executing parsing operation.
```C
    /* Parse command line arguments. */
    status = ArgParser_parse(aparser, argc, argv);
```

### Checking parsing status.
Execution status code ('status' in the sample code) is returned by each function.
The status code is 0 if success and 1 otherwise.

When fails, detailed message can be obtained via the following function.

```C
    /* Parse command line arguments. */
    status = ArgParser_parse(aparser, argc, argv);
    if(status != 0)
    {
        const char *errMsg = ArgParser_getErrorMsg(aparser);
        fprintf(stderr, "Info: %s\n", errMsg);
        exit(1); 
    }
```

For example, if we give the unregistered option, we get the following error message.
```SHELL
% ./example_program --unknown_option
Error: Unknown option: Near the arg. --unknown_option
```

### Finally, deleting argument parser.
```C
    ArgParser_delete(aparser);
```


