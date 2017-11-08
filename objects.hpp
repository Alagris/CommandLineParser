//
//  objects.hpp
//  CommandLineParser
//
//  Created by Alagris on 19/12/2016.
//  Copyright © 2016 alagris. All rights reserved.
//

#ifndef objects_hpp
#define objects_hpp
#include <stddef.h>
#ifdef DEBUG
#include <iostream>
#endif
namespace cmdlp{
    typedef char ** Parameters;
    
    struct CommandLineArg{
        int aliasIndex;
        Parameters params;
        size_t paramsCount;
    };
    struct CommandLineArgPacket{
        CommandLineArg * args;
        size_t argsCount;
    };
    
    extern CommandLineArg *  NULL_COMMAND ;
    extern CommandLineArg EMPTY_ARG;
    /**This is assigned only if alias is not known(.aliasIndex=-1) or there really are no parameters*/
    extern char ** NO_PARAMETERS;
    struct CommandLineAlias;
    /**Callback funtion that is invoked to process parameters.*/
    typedef void (*ProcessorFunction)
    (
    const CommandLineArg & arg,
    const CommandLineArgPacket & packet,
    const size_t aliasesCount,
    const CommandLineAlias * aliases
    );
    /**Invoked before processor function.*/
    typedef bool (*ValidatorFunction)(const CommandLineArg & arg);
    
    /**long name and description are optional (can be NULL) but short name must be provided.
     REMEBER TO TERMINATE EACH STRING WITH '\0'*/
    struct CommandLineAlias{
        const char * shortName;
        const char * longName;
        const char * description;
        ProcessorFunction callbackFunction;
        ValidatorFunction validatorFunction;
        CommandLineAlias(const char * pShortName,
                         const char * pLongName=NULL,
                         const char * pDescription=NULL,
                         ProcessorFunction pCallbackFunction=NULL,
                         ValidatorFunction pValidatorFunction=NULL):
        shortName(pShortName),
        longName(pLongName),
        description(pDescription),
        callbackFunction(pCallbackFunction),
        validatorFunction(pValidatorFunction) {
#ifdef DEBUG
            if(shortName==NULL)std::cerr<<"ATTENTION! Alias with shortName=NULL created!!!!\n";
#endif
        }
    };
    
    
    struct CommandLineAliasPacket{
        const CommandLineAlias * aliases;
        /**Count of aliases excluding the global alias (which is not included in the array)*/
        const size_t aliasesCount;
        CommandLineAlias globalAlias;
        CommandLineAliasPacket(
                               const CommandLineAlias * pAliases,
                               const size_t pAliasesCount,
                               ProcessorFunction pGlobalCallbackFunction=NULL,
                               ValidatorFunction pGlobalValidatorFunction=NULL):
        aliases(pAliases),
        aliasesCount(pAliasesCount),
        globalAlias("",NULL,NULL,pGlobalCallbackFunction,pGlobalValidatorFunction){}
    };
    
    void printHelpBasedOnAliases(const size_t aliasesCount,const CommandLineAlias * aliases);
    void printHelpBasedOnAliasPacket(const CommandLineAliasPacket & aliasPacket);
    
    void printHelpBasedOnAlias(const CommandLineAlias & alias);
    /**
     constraints
     filed.length>1
     because it's impossible to pass '\0' as argument to command line
     */
    bool isParam(const  char * const field);
    
    /**
     constraints
     filed.length>1
     because it's impossible to pass empty string ('\0') as argument to command line
     */
    bool isArg(const char * const field);
    /**Returns index of first encountered unknown argument (not listed among aliases). If none is found the result is the max value (-1 since size_t is unsigned)*/
    size_t containsUnknownArg(const CommandLineArgPacket & packet);
}
#endif /* objects_hpp */
