/**
 *  @file      ArgumentParser.h
 *  @brief     Argument Parser, local file.
 *  @author    fire-peregrine
 *  @date      2020/11/01
 *  @copyright Copyright (C) peregrine all rights reserved.
 *  @license   Released under the MIT License.
 */

#ifndef SRC_ARGUMENT_PARSER_H_
#define SRC_ARGUMENT_PARSER_H_

#include <stdint.h>
#include <stdbool.h>

/* Typedefs */
typedef struct ArgParser_ ArgParser;

/* Signatures */
/**
 *  @brief Create a new ArgParser object.
 *  @param [in] progName    Program name
 *  @param [in] description Program description
 *  @return A new ArgParser object if success, NULL otherwise.
 */
ArgParser* ArgParser_new(char *progName, char *progDesc);

/**
 *  @brief Delete ArgParser object.
 *  @param [in] obj ArgParser object
 *  @return Execution status
 */
int ArgParser_delete(ArgParser *obj);

/**
 *  @brief Require all positional parameters to be set.
 *         If invoked, user cannot omit positional parameters.
 *  @param [in] obj ArgParser object
 *  @return Execution status
 */
int ArgParser_requireFullPosParams(ArgParser *obj);

/**
 *  @brief Set version string.
 *  @param [in] obj     ArgParser object
 *  @param [in] version Version string
 *  @return Execution status
 */
int ArgParser_addVersion(ArgParser *obj, char *version);

/**
 *  @brief Set release date.
 *  @param [in] obj  ArgParser object
 *  @param [in] date Release date string
 *  @return Execution status
 */
int ArgParser_addDate(ArgParser *obj, char *date);

/**
 *  @brief Set author name.
 *  @param [in] obj    ArgParser object
 *  @param [in] author Release date string
 *  @return Execution status
 */
int ArgParser_addAuthor(ArgParser *obj, char *author);

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
int ArgParser_addInt(ArgParser *obj,
        int *dest, int defVal, char *sOpt, char *lOpt, const char *name, const char *desc);

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
int ArgParser_addUInt(ArgParser *obj,
        unsigned int *dest, unsigned int defVal, char *sOpt, char *lOpt, const char *name, const char *desc);

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
int ArgParser_addString(ArgParser *obj,
        char *dest, char *defVal, unsigned int maxLen, char *sOpt, char *lOpt, const char *name, const char *desc);

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
int ArgParser_addBool(ArgParser *obj,
        bool *dest, bool defVal, char *sOpt, char *lOpt, const char *name, const char *desc);

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
int ArgParser_addInt32(ArgParser *obj,
        int32_t *dest, int32_t defVal, char *sOpt, char *lOpt, const char *name, const char *desc);

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
int ArgParser_addUInt32(ArgParser *obj,
        int32_t *dest, int32_t defVal, char *sOpt, char *lOpt, const char *name, const char *desc);

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
int ArgParser_addFloat(ArgParser *obj,
        float *dest, float defVal, char *sOpt, char *lOpt, const char *name, const char *desc);

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
int ArgParser_addDouble(ArgParser *obj,
        double *dest, double defVal, char *sOpt, char *lOpt, const char *name, const char *desc);

/**
 *  @brief Add switch-type option.
 *  @param [in] obj      ArgParser object
 *  @param [in] dest     Destination
 *  @param [in] defVal   Default value
 *  @return Execution status
 */
int ArgParser_addTrue(ArgParser *obj,
        bool *dest, char *sOpt, char *lOpt, const char *name, const char *desc);

/**
 *  @brief ---
 */
int ArgParser_rest(ArgParser *obj, int *restNum, char ***rest);

/**
 *  @brief Parser command line arguments.
 *  @param [in] obj      ArgParser object
 *  @param [in] argc     Number of command line arguments
 *  @param [in] argv[]   Command line argument array
 *  @return Execution status
 */
int ArgParser_parse(ArgParser *obj,int argc, char **argv);

/**
 *  @brief Print internal variables.
 *  @param [in] obj ArgParser object
 *  @param [in] fp  Output file pointer
 *  @return Execution status
 */
int ArgParser_dump(ArgParser *obj, FILE *fp);

/**
 *  @brief Print help message.
 *  @param [in] obj ArgParser object
 *  @param [in] fp  Output file pointer
 *  @return Execution status
 */
int ArgParser_printHelp(ArgParser *obj, FILE *fp);

/**
 *  @brief Get error message.
 *  @param [in] obj ArgParser object
 *  @return Execution status
 */
const char* ArgParser_getErrorMsg(ArgParser *obj);


#endif // SRC_ARGUMENT_PARSER_H_


