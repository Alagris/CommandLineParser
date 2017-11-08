//
//  packetAccess.cpp
//  MyManager
//
//  Created by Alagris on 19/12/2016.
//  Copyright © 2016 alagris. All rights reserved.
//

#include "packetAccess.hpp"

/**
 How my engine interpters command syntax:
 someProgram globalParam1 globalParam2 -arg1 param1 param2 -arg2 param3 param4 param5 --arg-3 param6 param7
 Fileds "-" and "--" are interpteted as parameter
 Every arg has at leats one dash and one char after it.
 It means that every arg is at least 3 char long (including '\0')
 Also consider this example
 someProgram -arg1 param1 -arg2 param2 -arg1 param3
 the output will be
 -arg1
 param1
 -arg2
 param2
 -arg1
 param3
 Instead of
 -arg1
 param1
 param3
 -arg2
 param2
 So that it reatains infotmation about the sequence, which may be important for programs that work like state machine. Notice if you really need them to be layed out like in second case you can just sort them. Use sortPacket.hpp::sortPacketArguments
 If an argument is not listed among aliases the aliasIndex is -1
 USAGE:
 cmdlp::CommandLineArgPacket packet;
 cmdlp::commandLineEngineProcess(argc,argv,...,packet);
 ...do stuff...
 cmdlp::commandLineEngineDestroy(packet);
 */

namespace cmdlp{
    
    void initParams(CommandLineArg & arg){
        arg.params=new char*[arg.paramsCount];
    }
    void destroyParams(const CommandLineArg & arg){
        if(arg.params==NO_PARAMETERS )return;
        delete[] arg.params;
    }
    
    
    void commandLineEngineDestroy(const CommandLineArgPacket & packet){
        if(packet.args==NULL_COMMAND)return;
        for(size_t i=0;i<packet.argsCount;i++){
            destroyParams(packet.args[i]);
        }
        delete[] packet.args;
    }
    
    size_t getCountOfParameters(const int argc,
                                const char * const argv[],
                                size_t offsetInclusive=1){
        //note that by default the name of program is the very fist value
        //in every argv. That's why offsetInclusive is 1 by defult
        size_t paramsCount=0;
        for(;offsetInclusive<argc;offsetInclusive++){
            if(isParam(argv[offsetInclusive]))paramsCount++;
            else break;
        }
        return paramsCount;
    }
    
    
    size_t getCountOfArgs(const int argc,
                          const char * const argv[]){
        size_t argsCount=0;
        for(size_t i =1;i<argc;i++){
            //note that by default the name of program is the very fist value
            //in every argv. That's why i starts from 1
            if(isArg(argv[i]))argsCount++;
        }
        return argsCount;
    }
    
    /**return array of fileds after (including) offset. */
    void getFields(const int argc,
                   char * const argv[],
                   CommandLineArg & arg,
                   size_t offsetInclusive=1
                   ){
        //note that by default the name of program is the very fist value
        //in every argv. That's why offsetInclusive is 1 by defult
        
        for(size_t i=0;offsetInclusive<argc && i<arg.paramsCount;offsetInclusive++,i++){
            arg.params[i]=argv[offsetInclusive];
        }
    }
    /**Returns length of string excluding the final '\0'. So  length of "hi\0" is 2.
     Provide limit (from 0 to exclusive limit) in case this string was not null terminated.
     @return max value (-1) if something goes wrong*/
    size_t unsafeGetLengthOfCStyleString(const char * const str,const size_t limit=1024){
        for(size_t i=0;i<limit;i++){
            if(str[i]=='\0')return i;
        }
        return -1;
    }
    bool unsafeStringCompare(const char*const str1,const char*const str2,const size_t from1Inclusive,const size_t from2Inclusive,const size_t length){
        for(size_t i=0;i<length;i++){
            if(str1[from1Inclusive+i]!=str2[from2Inclusive+i])return false;
        }
        return true;
    }
    bool unsafeMatchAliasWithArg(const char *const argName,const size_t argLength,const CommandLineAlias alias){
        if(alias.shortName[argLength-1]=='\0'){//minus '-' from -s
            return unsafeStringCompare(alias.shortName,argName,0,1,argLength-1);
        }else if(alias.longName != NULL && alias.longName[argLength-2]=='\0'){//minus '--' from --SomeArg
            return unsafeStringCompare(alias.longName,argName,0,2,argLength-2);
        }
        return false;
    }
    int getAliasIndexByArgName(const size_t aliasesCount,
                               const CommandLineAlias *const aliases,
                               const char *const argName){
        if(aliases==NULL || aliasesCount==0)return -1;
        size_t argLength=unsafeGetLengthOfCStyleString(argName);
        if(argLength!=size_t(-1)){
            //-a is the shortest possible argument which has length 2
            //so the constraint is that argLength>=2
            for(int i=0;i<aliasesCount;i++){
                if(unsafeMatchAliasWithArg(argName,argLength,aliases[i])){
                    return i;
                }
            }
        }
        return -1;
    }
    /**You need to provied array of aliases. For example alias consisting of shost name "t" and long one "title-value" will be used to identify both arguments "-t" and "--title-value" as the same thing. The output of this functions which consists of array of CommandLineArg will contain filed aliasIndex which is the index of corresponding CommandLineAlias in povieded array.If you want to use global agruments set allowGlobalArgs to true.You don't need to provide alias for them. Their index will be -2. Index -1 is for unknown (unspecified among aliases) argument. All parameters of unknown parameter are skipped, however CommandLineArg will retain informtion about count of skipped parameters (paramsCount) just because why not. Maybe it will save someone tons of work in the future.
     */

    void commandLineEngineProcess(
                                  const int argc,
                                  char *  argv[],
                                  const bool allowGlobalArgs,
                                  const size_t aliasesCount,
                                  const CommandLineAlias * aliases,
                                  CommandLineArgPacket & packetOutput
                                  ){
        if(argc<=1 || argv==NULL){//no arguments/parameters passed
            //note that by default the name of program is the very fist value
            //in every argv
            packetOutput.args=NULL_COMMAND;
            packetOutput.argsCount=0;
            return;
        }
        packetOutput.argsCount =getCountOfArgs(argc,argv);
        const size_t countOfGlobalParams=getCountOfParameters(argc,argv);
        size_t currentArg;
        if(countOfGlobalParams>0){
            packetOutput.args = new CommandLineArg[++packetOutput.argsCount];
            CommandLineArg & cmdArg = packetOutput.args[0];
            if(allowGlobalArgs){
                cmdArg.aliasIndex=-2;
                cmdArg.paramsCount=countOfGlobalParams;
                initParams(cmdArg);
                getFields(argc,argv,cmdArg);
            }else{
                cmdArg.aliasIndex=-1;
                cmdArg.params=NO_PARAMETERS;
            }
            currentArg=1;
        }else{
            packetOutput.args = new CommandLineArg[packetOutput.argsCount];
            currentArg=0;
        }
        
        for(size_t currentArgToFiled=1+countOfGlobalParams;currentArg<packetOutput.argsCount;currentArg++){
            CommandLineArg & currentCmdArg = packetOutput.args[currentArg];
            currentCmdArg.aliasIndex=getAliasIndexByArgName(aliasesCount,aliases,argv[currentArgToFiled]);
            size_t paramsCount = getCountOfParameters(argc,argv,currentArgToFiled+1);
            if(currentCmdArg.aliasIndex==-1){
                currentCmdArg.paramsCount=1;
                initParams(currentCmdArg);
                currentCmdArg.params[0]=argv[currentArgToFiled];
            }else if(paramsCount==0){
                 currentCmdArg.paramsCount=0;
                 currentCmdArg.params=NO_PARAMETERS;
            }else{
                currentCmdArg.paramsCount=paramsCount;
                initParams(currentCmdArg);
                getFields(argc,argv,currentCmdArg,currentArgToFiled+1);
            }
            currentArgToFiled+=1+paramsCount;
        }
    }
    
}
