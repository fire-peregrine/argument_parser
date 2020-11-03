/**
 *  @file      ArgParser.c
 *  @brief     Argument Parser
 *  @author    fire-peregrine
 *  @date      2020/11/01
 *  @copyright Copyright (C) peregrine all rights reserved.
 *  @license   Released under the MIT License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ArgParser.h"
#include "ArgParser_local.h"

/* Signatures */

static int addParam(ArgParser *obj,
        VarType varType, void *dest, Val *defVal, const char *sOpt, const char *lOpt, const char *name, const char *desc);
static char* copyStr(ArgParser *obj, const char *str);
static bool isOptParam(const char *sOpt, const char *lOpt);
static bool isPosParam(const char *sOpt, const char *lOpt);
static PrmDef* findOptionalParam(ArgParser *obj, const char *arg);
static int writeDefaultParams(ArgParser *obj);
static int writeDefaultValue(PrmDef *pdef);
static int writeArg(const char *arg, PrmDef *pdef);
static ArgType determineArgType(const char *arg);
static bool isHelpOption(const char *arg);
static bool isVerOption(const char *arg);
static int setErrorMsg(ArgParser *obj, char *fmt, ...);
static int printHelp(ArgParser *obj, FILE *fp);
static int printVersion(ArgParser *obj, FILE *fp);
static int printParamDescription(PrmDef *pdef, FILE *fp);


/* Functions */
/**
 *  @brief Create a new ArgParser object.
 *  @param [in] progName    Program name
 *  @param [in] description Program description
 *  @return A new ArgParser object if success, NULL otherwise.
 */
ArgParser* ArgParser_new(char *progName, char *progDesc)
{
    int status;
    Val v;

    /* Create new object. */
    ArgParser *obj = (ArgParser *) malloc(sizeof(ArgParser));
    if(obj == NULL)
    {
        fprintf(stderr, "Error: Cannot allocate memory.\n");
        goto error;
    }
    memset(obj, 0x00, sizeof(ArgParser));

    /* Initialize the created object. */
    obj->progName = NULL;
    obj->progDesc = NULL;
    obj->version  = NULL;
    obj->date     = NULL;
    obj->author   = NULL;

    obj->numOptPrms = 0;
    obj->numPosPrms = 0;

    obj->hasError = false;
    strcpy(obj->errorMsg, "OK.");

    obj->bufIdx = 0;
    obj->buf[0] = '\0';
  
    // Add the program name
    obj->progName = copyStr(obj, progName);
    if(obj->progName == NULL)
        goto error;
    
    // Add the program description
    obj->progDesc = copyStr(obj, progDesc);
    if(obj->progDesc == NULL)
        goto error;

    // Initialize the version string.
    obj->version = copyStr(obj, "");
    if(obj->version == NULL)
        goto error;

    // Initialize release date.
    obj->date    = copyStr(obj, "");
    if(obj->date == NULL)
        goto error;

    // Initialize author name.
    obj->author  = copyStr(obj, "");
    if(obj->author == NULL)
        goto error;

    /* Register help option. */
    v.b = false;
    status = addParam(obj, VarType_True, &(obj->isHelpSpecified), &v, "-h", "--help",  "help", "Show help message.");
    if(status != 0)
    {
        setErrorMsg(obj, "Cannot set help option.");
        goto error;
    }
    
    /* Register version option. */
    status = addParam(obj, VarType_True, &(obj->isVerSpecified), &v, "-v", "--version",  "version", "Show version string.");
    if(status != 0)
    {
        setErrorMsg(obj, "Cannot set version option.");
        goto error;
    }

    return obj;

error: /* error handling */

    if(obj == NULL)
        return NULL;

    free(obj);

    return NULL;
}


/**
 *  @brief Delete ArgParser object.
 *  @param [in] obj ArgParser object
 *  @return Execution status
 */
int ArgParser_delete(ArgParser *obj)
{
    free(obj);
    return 0;
}


/**
 *  @brief Require all positional parameters to be set.
 *         If invoked, user cannot omit positional parameters.
 *  @param [in] obj ArgParser object
 *  @return Execution status
 */
int ArgParser_requireFullPosParams(ArgParser *obj)
{
    obj->reqFullPosParams = true;
    return 0;
}


/**
 *  @brief Set version string.
 *  @param [in] obj     ArgParser object
 *  @param [in] version Version string
 *  @return Execution status
 */
int ArgParser_addVersion(ArgParser *obj, char *version)
{
    obj->version = copyStr(obj, version);
    if(obj->version == NULL)
    {
        setErrorMsg(obj, "Cannot add version string.");
        return 1;
    }

    return 0;
}


/**
 *  @brief Set release date.
 *  @param [in] obj  ArgParser object
 *  @param [in] date Release date string
 *  @return Execution status
 */
int ArgParser_addDate(ArgParser *obj, char *date)
{
    obj->date = copyStr(obj, date);
    if(obj->date == NULL)
    {
        setErrorMsg(obj, "Cannot add release date.");
        return 1;
    }

    return 0;
}


/**
 *  @brief Set author name.
 *  @param [in] obj    ArgParser object
 *  @param [in] author Release date string
 *  @return Execution status
 */
int ArgParser_addAuthor(ArgParser *obj, char *author)
{
    obj->author = copyStr(obj, author);
    if(obj->author == NULL)
    {
        setErrorMsg(obj, "Cannot add author name.");
        return 1;
    }

    return 0;
}


/**
 *  @brief Add int-type option.
 *  @param [in] obj    ArgParser object
 *  @param [in] dest   Destination
 *  @param [in] defVal Default value
 *  @param [in] sOpt   Short option
 *  @param [in] lOpt   Long option
 *  @param [in] name   Parameter name
 *  @param [in] desc   Parameter description
 *  @return Execution status
 */
int ArgParser_addInt(ArgParser *obj, int *dest, int defVal, char *sOpt, char *lOpt, const char *name, const char *desc)
{
    Val v;
    v.i = defVal;
    return addParam(obj, VarType_Int, dest, &v, sOpt, lOpt, name, desc);
}


/**
 *  @brief Add unsigned-int-type option.
 *  @param [in] obj    ArgParser object
 *  @param [in] dest   Destination
 *  @param [in] defVal Default value
 *  @param [in] sOpt   Short option
 *  @param [in] lOpt   Long option
 *  @param [in] name   Parameter name
 *  @param [in] desc   Parameter description
 *  @return Execution status
 */
int ArgParser_addUInt(ArgParser *obj, unsigned int *dest, unsigned int defVal, char *sOpt, char *lOpt, const char *name, const char *desc)
{
    Val v;
    v.u = defVal;
    return addParam(obj, VarType_UInt, dest, &v, sOpt, lOpt, name, desc);
}


/**
 *  @brief Add string-type option.
 *  @param [in] obj    ArgParser object
 *  @param [in] dest   Destination
 *  @param [in] defVal Default value
 *  @param [in] maxLen Maximum string length
 *  @param [in] sOpt   Short option
 *  @param [in] lOpt   Long option
 *  @param [in] name   Parameter name
 *  @param [in] desc   Parameter description
 *  @return Execution status
 */
int ArgParser_addString(ArgParser *obj, char *dest, char *defVal, unsigned int maxLen, char *sOpt, char *lOpt, const char *name, const char *desc)
{
    Val v;
    v.s.data = defVal; 
    v.s.len  = maxLen;
    return addParam(obj, VarType_String, dest, &v, sOpt, lOpt, name, desc);
}


/**
 *  @return   Execution status
 */
/**
 *  @brief Add bool-type option.
 *  @param [in] obj      ArgParser object
 *  @param [in] dest     Destination
 *  @param [in] defVal   Default value
 *  @param [in] sOpt   Short option
 *  @param [in] lOpt   Long option
 *  @param [in] name   Parameter name
 *  @param [in] desc   Parameter description
 *  @return Execution status
 */
int ArgParser_addBool(ArgParser *obj, bool *dest, bool defVal, char *sOpt, char *lOpt, const char *name, const char *desc)
{
    Val v;
    v.b = defVal;
    return addParam(obj, VarType_Bool, dest, &v, sOpt, lOpt, name, desc);
}


/**
 *  @brief Add int32_t-type option.
 *  @param [in] obj      ArgParser object
 *  @param [in] dest     Destination
 *  @param [in] defVal   Default value
 *  @param [in] sOpt   Short option
 *  @param [in] lOpt   Long option
 *  @param [in] name   Parameter name
 *  @param [in] desc   Parameter description
 *  @return Execution status
 */
int ArgParser_addInt32(ArgParser *obj, int32_t *dest, int32_t defVal, char *sOpt, char *lOpt, const char *name, const char *desc)
{
    Val v;
    v.i32 = defVal;
    return addParam(obj, VarType_Int32, dest, &v, sOpt, lOpt, name, desc);
}


/**
 *  @brief Add uint32_t-type option.
 *  @param [in] obj      ArgParser object
 *  @param [in] dest     Destination
 *  @param [in] defVal   Default value
 *  @param [in] sOpt   Short option
 *  @param [in] lOpt   Long option
 *  @param [in] name   Parameter name
 *  @param [in] desc   Parameter description
 *  @return Execution status
 */
int ArgParser_addUInt32(ArgParser *obj, int32_t *dest, int32_t defVal, char *sOpt, char *lOpt, const char *name, const char *desc)
{
    Val v;
    v.u32 = defVal;
    return addParam(obj, VarType_UInt32, dest, &v, sOpt, lOpt, name, desc);
}


/**
 *  @brief Add float-type option.
 *  @param [in] obj    ArgParser object
 *  @param [in] dest   Destination
 *  @param [in] defVal Default value
 *  @param [in] sOpt   Short option
 *  @param [in] lOpt   Long option
 *  @param [in] name   Parameter name
 *  @param [in] desc   Parameter description
 *  @return Execution status
 */
int ArgParser_addFloat(ArgParser *obj, float *dest, float defVal, char *sOpt, char *lOpt, const char *name, const char *desc)
{
    Val v;
    v.f = defVal;
    return addParam(obj, VarType_Float, dest, &v, sOpt, lOpt, name, desc);
}


/**
 *  @brief Add double-type option.
 *  @param [in] obj    ArgParser object
 *  @param [in] dest   Destination
 *  @param [in] defVal Default value
 *  @param [in] sOpt   Short option
 *  @param [in] lOpt   Long option
 *  @param [in] name   Parameter name
 *  @param [in] desc   Parameter description
 *  @return Execution status
 */
int ArgParser_addDouble(ArgParser *obj, double *dest, double defVal, char *sOpt, char *lOpt, const char *name, const char *desc)
{
    Val v;
    v.d = defVal;
    return addParam(obj, VarType_Double, dest, &v, sOpt, lOpt, name, desc);
}


/**
 *  @brief Add switch-type option.
 *  @param [in] obj      ArgParser object
 *  @param [in] dest     Destination
 *  @param [in] defVal   Default value
 *  @return   Execution status
 */
int ArgParser_addTrue(ArgParser *obj, bool *dest, char *sOpt, char *lOpt, const char *name, const char *desc)
{
    Val v;
    v.b = false;
    return addParam(obj, VarType_True, dest, &v, sOpt, lOpt, name, desc);
}


/**
 *  @brief ---
 */
int ArgParser_rest(ArgParser *obj, int *restNum, char ***rest)
{
    return 0;
}


/**
 *  @brief Parser command line arguments.
 *  @param [in] obj      ArgParser object
 *  @param [in] argc     Number of command line arguments
 *  @param [in] argv[]   Command line argument array
 *  @return Execution status
 */
int ArgParser_parse(ArgParser *obj, int argc, char **argv)
{
    int status;
    ArgType argType;
    int posIdx = 0;
    int i = 1;

    /* Write default parameter values. */
    status = writeDefaultParams(obj);
    if(status != 0)
        goto error;

    while(i < argc)
    {
        /* Determine argument type. */
        argType = determineArgType(argv[i]);

        // Invalid argument
        if(argType == ArgType_Error)
        {
            setErrorMsg(obj, "Irregal argument type: Near the arg '%s'.", argv[i]);
            goto error;
        }

        // Normal argument
        if(argType == ArgType_NoOpt)
        {
            // Too many positional parameters.
            if(posIdx == obj->numPosPrms)
            {
                setErrorMsg(obj, "Too many positonal arguments: Near the arg '%s'. Needs %d positional args. But has more args.", argv[i], obj->numPosPrms);
                goto error;
            }

            // Write positional parameter to destination.
            PrmDef *pdef = &(obj->posPrms[posIdx]);
            if(writeArg(argv[i], pdef) != 0)
            {
                setErrorMsg(obj, "Invalid value: arg %s, %s", argv[i], pdef->name);
                goto error;
            }
            posIdx++;

            i++;
            continue;
        }

        /* Find Option infomation */
        PrmDef *pdef = findOptionalParam(obj, argv[i]);
        if(pdef == NULL)
        {
            setErrorMsg(obj, "Unknown option: Near the arg. %s", argv[i]);
            return 1;
        }

        /* Check if the help option is specified. */
        if(isHelpOption(argv[i]) == true)
        {
            printHelp(obj, stdout);
            exit(0);   
        }
        
        /* Check if the version option is specified. */
        if(isVerOption(argv[i]) == true)
        {
            printVersion(obj, stdout);
            exit(0);   
        }

        /* Switch-type option. */
        if(pdef->varType == VarType_True)
        {
            if(writeArg("1", pdef) != 0)
            {
                setErrorMsg(obj, "Invalid value: arg %s, %s", argv[i], pdef->name);
                return 1;    
            }
            i++;
            continue;
        }

        if(i == (argc - 1))
        {
            setErrorMsg(obj, "Lack of the last argument: Near the arg %s.", argv[i]);
            return 1;
        }
        i++;

        if(writeArg(argv[i], pdef) != 0)
        {
            setErrorMsg(obj, "Invalid value: arg %s, %s", argv[i], pdef->name);
            return 1;    
        }
        i++;
    }

    /* Too few arguments. */
    if((posIdx < obj->numPosPrms) && (obj->reqFullPosParams == true))
    {
        setErrorMsg(obj, "Too few positonal arguments: Needs %d args. But has only %d args.", obj->numPosPrms, posIdx);
        return 1;
    }

    return 0;

error: /* error handling */
    return 1;
}


/**
 *  @brief ---
 */
int ArgParser_dump(ArgParser *obj, FILE *fp)
{
    fprintf(fp, "*** ArgParser ***\n");
    fprintf(fp, "progName = '%s' \n", obj->progName);
    fprintf(fp, "progDesc = '%s' \n", obj->progDesc);
    fprintf(fp, "hasError = '%d' \n", obj->hasError);
    fprintf(fp, "errorMsg = '%s' \n", obj->errorMsg);

    return 0;
}


/**
 *  @brief Print help message.
 *  @param [in] obj ArgParser object
 *  @param [in] fp  Output file pointer
 *  @return Execution status
 */
int ArgParser_printHelp(ArgParser *obj, FILE *fp)
{
    return printHelp(obj, fp);
}


/**
 *  @brief Get error message.
 *  @param [in] obj ArgParser object
 *  @return Execution status
 */
const char* ArgParser_getErrorMsg(ArgParser *obj)
{
    return obj->errorMsg;
}




/**
 *  @brief Add a new optional/positional parameter.
 *  @param [in] obj     ArgParser object
 *  @param [in] varType Variable type
 *  @param [in] dest    Destination
 *  @param [in] defVal  Default value
 *  @param [in] sOpt    Short option
 *  @param [in] lOpt    Long option
 *  @param [in] name    Parameter name
 *  @param [in] desc    Parameter description
 *  @return 
 */
static int addParam(ArgParser *obj,
        VarType varType, void *dest, Val *defVal, const char *sOpt, const char *lOpt, const char *name, const char *desc)
{
    unsigned int bufIdx = obj->bufIdx;
    PrmDef *pdef = NULL;

    if(isOptParam(sOpt, lOpt) == true) // Optional parameter
    {
        if(obj->numOptPrms >= APARSER_MAX_ARG_PRMS)
        {
            setErrorMsg(obj, "Maximum number of optional parameters reached.\n");
            goto error;
        }

        pdef = &(obj->optPrms[obj->numOptPrms]);
    }
    else // Positional parameter
    {
        if(obj->numPosPrms >= APARSER_MAX_ARG_PRMS)
        {
            setErrorMsg(obj, "Maximum number of positional parameters reached.\n");
            goto error;
        }

        pdef = &(obj->posPrms[obj->numPosPrms]);
    }
  
    pdef->varType = varType;
    pdef->dest    = dest;
    
    switch(varType)
    {
        case VarType_String:
            pdef->defVal.s.len  = (*defVal).s.len;
            pdef->defVal.s.data = copyStr(obj, (*defVal).s.data);
            if(pdef->defVal.s.data == NULL)
            {
                setErrorMsg(obj, "Cannot store string-type default value. '%s'\n", (*defVal).s.data);
                goto error;
            }
            break;
        
        case VarType_Int:
            pdef->defVal.i = (*defVal).i;
            break;

        case VarType_UInt:  
            pdef->defVal.u = (*defVal).u;
            break;

        case VarType_Bool: 
            pdef->defVal.b = (*defVal).b;
            break;

        case VarType_Int32: 
            pdef->defVal.i32 = (*defVal).i32;
            break;

        case VarType_UInt32:
            pdef->defVal.u32 = (*defVal).u32;
            break;

        case VarType_Float:
            pdef->defVal.f = (*defVal).f;
            break;

        case VarType_Double:
            pdef->defVal.d = (*defVal).d;
            break;

        case VarType_True:
            pdef->defVal.b = false;
            break;

        default:
            goto error;
    }

    // Short option
    pdef->sOpt = copyStr(obj, sOpt);
    if(pdef->sOpt == NULL)
    {
        setErrorMsg(obj, "Cannot store short option. '%s'\n", sOpt);
        goto error;
    }

    // Long option
    pdef->lOpt = copyStr(obj, lOpt);
    if(pdef->lOpt == NULL)
        goto error;

    // Parameter name
    pdef->name = copyStr(obj, name);
    if(pdef->name == NULL)
        goto error;

    // Parameter description 
    pdef->desc = copyStr(obj, desc);
    if(pdef->desc == NULL)
        goto error;

    if(isOptParam(sOpt, lOpt) == true) // Optional parameter
        obj->numOptPrms++;
    else
        obj->numPosPrms++; // Positional parameter

    return 0;

error: /* error handling */

    // Roll back the temporary buffer index.
    obj->bufIdx = bufIdx;
    return 1;
}


/**
 *  @brief Copy string to the internal buffer and returns the copied one.
 *         If the string is NULL, An empty string ("") is copied.
 *  @param [in] obj   ArgParser object
 *  @param [in] str   String to copy.
 *  @return   A new copied string if success, NULL otherwise.
 */
static char* copyStr(ArgParser *obj, const char *str)
{
    const char *tmp = (str != NULL) ? str : "";

    /* Calculate the rest of the internal buffer. */
    int bufRest = APARSER_MAX_BUF - obj->bufIdx;
   
    int len = strlen(tmp) + 1 /* null termination */;
   
    /* Check the string size. */
    if(bufRest < len)
    {
        setErrorMsg(obj, "Cannot store the string '%s'.", tmp);
        goto error;
    }

    /* Copy the string and returns the copied one. */
    char *p = &(obj->buf[obj->bufIdx]);
    strcpy(p, tmp);
    obj->bufIdx += len;

    // fprintf(stderr, "copyStr: str= '%s' len= %d idx= %5d -> %5d\n", tmp, len, obj->bufIdx - len, obj->bufIdx);
    return p;

error: /* error handling */

    return NULL;
}


/**
 *  @brief Check if the parameter is a optional parameter.
 *  @param [in] sOpt   Short option
 *  @param [in] lOpt   Long option
 *  @retval true  The patameter is a optional patameter.
 *  @retval false The patameter is not a optional patameter.
 */
static bool isOptParam(const char *sOpt, const char *lOpt)
{
    if((sOpt != NULL) && (strlen(sOpt) != 0))
        return true;

    if((lOpt != NULL) && (strlen(lOpt) != 0))
        return true;

    return false;
}


/**
 *  @brief Check if the parameter is a positional parameter.
 *  @param [in] def Parameter definition
 *  @retval true  The patameter is a positional patameter.
 *  @retval false The patameter is not a positional patameter.
 */
static bool isPosParam(const char *sOpt, const char *lOpt)
{
    return !isOptParam(sOpt, lOpt);
}


/**
 *  @brief Find optional parameters.
 *  @param [in] obj ArgParser object
 *  @param [in] arg Commend line argument.
 *  @return Parameter definition if found. NULL otherwise.
 */
static PrmDef* findOptionalParam(ArgParser *obj, const char *arg)
{
    unsigned int i = 0;
    for(i = 0; i < obj->numOptPrms; i++)
    {
        PrmDef *pdef = &(obj->optPrms[i]);
        if((strcmp(arg, pdef->sOpt) == 0) || (strcmp(arg, pdef->lOpt) == 0))
            return pdef;
    }

    return NULL;
};



/**
 *  @brief Write defult parameters.
 *  @param [in] obj   ArgParser object
 *  @return Execution status
 */
static int writeDefaultParams(ArgParser *obj)
{
    int status = 0;
    
    /* Write default values for the optional parameters */
    unsigned int i;
    for(i = 0; i < obj->numOptPrms; i++)
    {
        PrmDef *pdef = &(obj->optPrms[i]);
        status = writeDefaultValue(pdef);
        if(status != 0)
        {
            setErrorMsg(obj, "Cannot write default value. near the parameter '%s',", pdef->name);
            goto error;
        }
    }

    /* Write default values for the positional parameters */
    for(i = 0; i < obj->numPosPrms; i++)
    {
        PrmDef *pdef = &(obj->posPrms[i]);
        status = writeDefaultValue(pdef);
        if(status != 0)
        {
            setErrorMsg(obj, "Cannot write default value. near the parameter '%s',", pdef->name);
            goto error;
        }
    }

    return 0;

error: /* error handling */

    return 1;
}


/**
 *  @brief Convert a value into the specified type and store to the destination.
 *  @param [in]  val  Value
 *  @param [in]  type Type
 *  @param [out] dest Destination
 *  @return Execution status
 */
static int writeDefaultValue(PrmDef *pdef)
{
    switch(pdef->varType)
    {
        case VarType_Int:
            *(int *) pdef->dest = pdef->defVal.i;
            return 0;

        case VarType_UInt:
            *(unsigned int *) pdef->dest = pdef->defVal.u;
            return 0;

        case VarType_String:
        {
            char *p = NULL;
            p = (char *) pdef->dest; 
            strncpy(p, pdef->defVal.s.data, pdef->defVal.s.len);
            p[pdef->defVal.s.len - 1] = '\0';
            return 0;
        }

        case VarType_Bool:
            *(bool *) pdef->dest = pdef->defVal.b;
            return 0;
        
        case VarType_Int32:
            *(int32_t *) pdef->dest = pdef->defVal.i32;
            return 0;
        
        case VarType_UInt32:
            *(uint32_t *) pdef->dest = pdef->defVal.u32;
            return 0;

        case VarType_Float:
            *(float *) pdef->dest = pdef->defVal.f;

            return 0;
        
        case VarType_Double:
            *(double *) pdef->dest = pdef->defVal.d;
            return 0;
        
        case VarType_True:
            *(bool*) pdef->dest = pdef->defVal.b;
            return 0;

        default:
            /* Unreachable */
            return 1;
    }
}


/**
 *  @brief Convert single command line argument into the specified type
 *         and store to the destination.
 *  @param [in]  arg     Command line argument
 *  @param [in]  podef   Parameter definition
 *  @param [out] dest    Destination
 *  @return Execution status
 */
static int writeArg(const char *arg, PrmDef *pdef)
{
    char *errPtr = NULL; // Error pointer
    switch(pdef->varType)
    {
        case VarType_Int:
            *(int *) pdef->dest = strtoul(arg, &errPtr, 0 /* auto-radix */);
            if(*errPtr != '\0')
                return 1;

            return 0;

        case VarType_UInt:

            *(unsigned int *) pdef->dest = strtoul(arg, &errPtr, 0 /* auto-radix */);
            if(*errPtr != '\0')
                return 1;

            return 0;

        case VarType_String:
        {
            char *p = (char *) pdef->dest; 
            int len = pdef->defVal.s.len;
            strncpy(p, arg, len);
            p[len - 1] = '\0';
            return 0;
        }

        case VarType_Bool:
            *(bool *) pdef->dest = (bool) strtoul(arg, &errPtr, 0 /* auto-radix */);
            if(*errPtr != '\0')
                return 1;

            return 0;
        
        case VarType_Int32:

            *(int32_t *) pdef->dest = strtoul(arg, &errPtr, 0 /* auto-radix */);
            if(*errPtr != '\0')
                return 1;

            return 0;
        
        case VarType_UInt32:
            *(uint32_t *) pdef->dest = strtoul(arg, &errPtr, 0 /* auto-radix */);
            if(*errPtr != '\0')
                return 1;

            return 0;

        case VarType_Float:
            *(float *) pdef->dest = strtof(arg, &errPtr);
            if(*errPtr != '\0')
                return 1;

            return 0;
        
        case VarType_Double:
            *(double *) pdef->dest = strtod(arg, &errPtr);
            if(*errPtr != '\0')
                return 1;

            return 0;

        case VarType_True:
            *(bool *) pdef->dest = (bool) strtoul(arg, &errPtr, 0 /* auto-radix */);
            if(*errPtr != '\0')
                return 1;

            return 0;

        default:
            /* Unreachable */
            return 0;
    }
}


/**
 *  @brief Determine argument type
 *  @param [in] arg Command line argument.
 *  @return Argument type
 */
static ArgType determineArgType(const char *arg)
{
    if((strlen(arg) >= 3) && (arg[0] == '-') && (arg[1] == '-'))
    {
        if(arg[2] != '-')
            return ArgType_Opt; // An argument which starts with '--'.

        return ArgType_Error; //  An argument which starts with '---' (invalid).
    }
    else if((strlen(arg) >= 2) && (arg[0] == '-'))
    {
        if(arg[1] != '-')
            return ArgType_Opt; // A token which starts with '--'.

        return ArgType_Error; // '--' (invalid).
    }
    else
    {
        if(arg[0] != '-')
            return ArgType_NoOpt; // Normal Argument.

        return ArgType_Error; // '-' (invalid).
    }
};


/**
 *  @brief Check if the help opetion is specified.
 */
static bool isHelpOption(const char *arg)
{
    return (strcmp(arg, "-h") == 0) || (strcmp(arg, "--help") == 0);
};


/**
 *  @brief Check if the version opetion is specified.
 */
static bool isVerOption(const char *arg)
{
    return (strcmp(arg, "-v") == 0) || (strcmp(arg, "--version") == 0);
};


/**
 *  @brief Store error message.
 *  @param [in] obj      ArgParser object
 *  @param [in] errorMsg Format string
 *  @param [in] ...      
 */
static int setErrorMsg(ArgParser *obj, char *fmt, ...)
{
    va_list va;

    va_start(va, fmt);
    vsnprintf(obj->errorMsg, APARSER_MAX_ERROR_MSG, fmt, va);
    va_end(va);

    return 0;
}


/**
 *  @brief Print help message.
 *  @param [in] obj ArgParser object
 *  @param [in] fp  Output file pointer
 *  @return Execution status
 */
static int printHelp(ArgParser *obj, FILE *fp)
{
    fprintf(fp, "\n");
    fprintf(fp, "Usage   : %s [-h/--help] [-v/--version] (optional_parameters ...) ", obj->progName);

    if(obj->numPosPrms != 0)
    {
        int i;
        for(i = 0; i < obj->numPosPrms; i++)
        {  
            PrmDef *pdef = &(obj->posPrms[i]);
            fprintf(fp, "[%s] ", pdef->name);
        }
    }
    fprintf(fp, "\n");
    fprintf(fp, "\n");

    // Print optional parameter descriptions.
    if(obj->numOptPrms != 0)
    {
        fprintf(fp, "Optional Parameter%s:\n", (obj->numOptPrms != 1) ? "s" : "");
        fprintf(fp, "\n");

        int i;
        for(i = 0; i < obj->numOptPrms; i++)
        {
            PrmDef *pdef = &(obj->optPrms[i]);
            printParamDescription(pdef, fp);
        }
    }
    
    // Print positional parameter descriptions.
    if(obj->numPosPrms != 0)
    {
        fprintf(fp, "Positional Parameter%s:\n", (obj->numPosPrms != 1) ? "s" : "");
        fprintf(fp, "\n");

        int i;
        for(i = 0; i < obj->numPosPrms; i++)
        {
            PrmDef *pdef = &(obj->posPrms[i]);
            printParamDescription(pdef, fp);
        }
    }
    
    return 0;
}


/**
 *  @brief Print parameter description.
 *  @param [in] pdef Parameter definition
 *  @param [in] fp   Output file pointer
 */
static int printParamDescription(PrmDef *pdef, FILE *fp)
{
    const char *typeNames[VarType_Num] =
    {
        "[int]"    , // VarType_Int    
        "[uint]"   , // VarType_UInt   
        "[string]" , // VarType_String 
        "[0/1]"    , // VarType_Bool   
        "[int32]"  , // VarType_Int32  
        "[uint32]" , // VarType_UInt32 
        "[float]"  , // VarType_Float  
        "[double]" , // VarType_Double 
        ""           // VarType_True   
    };

    // Indent
    fprintf(fp, "    ");

    // Short option
    if(strlen(pdef->sOpt) != 0)
    {
        fprintf(fp, "%s ", pdef->sOpt);

        if(strlen(typeNames[pdef->varType]) != 0)
            fprintf(fp, "%s ", typeNames[pdef->varType]);
    }
    
    // Delimiter
    if((strlen(pdef->sOpt) != 0) && (strlen(pdef->lOpt) != 0)) 
        fprintf(fp, "/ ");
    
    // Long option
    if(strlen(pdef->lOpt) != 0)
    {
        fprintf(fp, "%s ", pdef->lOpt);

        if(strlen(typeNames[pdef->varType]) != 0)
            fprintf(fp, "%s ", typeNames[pdef->varType]);
    }
  
    if((strlen(pdef->sOpt) == 0) && (strlen(pdef->lOpt) == 0)) 
        fprintf(fp, "%s ", typeNames[pdef->varType]);
 
    // Print parameter name.
    fprintf(fp, ": %s\n", pdef->name);
   
    // Print description.
    int i;
    fprintf(fp, "    | description:\n");
    fprintf(fp, "    |    ");
    for(i = 0; pdef->desc[i] != '\0'; i++)
    {
        fprintf(fp, "%c", pdef->desc[i]);

        if(pdef->desc[i] == '\n')
            fprintf(fp, "    |    ");
    }
    fprintf(fp, "\n");
    fprintf(fp, "\n");

    return 0;
}



/**
 *  @brief Print version information.
 *  @param [in] obj ArgParser object
 *  @param [in] fp  Output file pointer
 *  @return Execution status
 */
static int printVersion(ArgParser *obj, FILE *fp)
{
    fprintf(fp, "%s %s\n", obj->progName, obj->version); 
    fprintf(fp, "written by %s\n", obj->author); 
    fprintf(fp, "released on %s\n", obj->date); 
    fprintf(fp, "\n");
    return 0;
}


