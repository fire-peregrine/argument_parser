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
ArgParser* ArgParser_new(char *progName, char *progDesc);
int ArgParser_delete(ArgParser *obj);
int ArgParser_requireFullPosParams(ArgParser *obj);

int ArgParser_addVersion(ArgParser *obj, char *version);
int ArgParser_addDate(ArgParser *obj, char *date);
int ArgParser_addAuthor(ArgParser *obj, char *author);

int ArgParser_addInt(ArgParser *obj,
        int *dest, int defVal, char *sOpt, char *lOpt, const char *name, const char *desc);
int ArgParser_addUInt(ArgParser *obj,
        unsigned int *dest, unsigned int defVal, char *sOpt, char *lOpt, const char *name, const char *desc);
int ArgParser_addString(ArgParser *obj,
        char *dest, char *defVal, unsigned int maxLen, char *sOpt, char *lOpt, const char *name, const char *desc);
int ArgParser_addBool(ArgParser *obj,
        bool *dest, bool defVal, char *sOpt, char *lOpt, const char *name, const char *desc);
int ArgParser_addInt32(ArgParser *obj,
        int32_t *dest, int32_t defVal, char *sOpt, char *lOpt, const char *name, const char *desc);
int ArgParser_addUInt32(ArgParser *obj,
        int32_t *dest, int32_t defVal, char *sOpt, char *lOpt, const char *name, const char *desc);
int ArgParser_addFloat(ArgParser *obj,
        float *dest, float defVal, char *sOpt, char *lOpt, const char *name, const char *desc);
int ArgParser_addDouble(ArgParser *obj,
        double *dest, double defVal, char *sOpt, char *lOpt, const char *name, const char *desc);
int ArgParser_addTrue(ArgParser *obj,
        bool *dest, char *sOpt, char *lOpt, const char *name, const char *desc);
int ArgParser_rest(ArgParser *obj, int *restNum, char ***rest);
int ArgParser_parse(ArgParser *obj,int argc, char **argv);
int ArgParser_dump(ArgParser *obj, FILE *fp);
int ArgParser_printHelp(ArgParser *obj, FILE *fp);
const char* ArgParser_getErrorMsg(ArgParser *obj);

#endif // SRC_ARGUMENT_PARSER_H_


