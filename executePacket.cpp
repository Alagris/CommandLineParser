//
//  executePacket.cpp
//  CommandLineParser
//
//  Created by Alagris on 21/12/2016.
//  Copyright © 2016 alagris. All rights reserved.
//

#include "executePacket.hpp"
namespace cmdlp{
    void executePacket(
                       const CommandLineArgPacket & packet,
                       const size_t functionsCountExcludingGlobalFunc,
                       const ProcessorFunction * const functions,
                       const ProcessorFunction functionForGlobalArgs,
                       const size_t aliasesCount,
                       const CommandLineAlias * aliases){
        
        for(size_t i=0;i<packet.argsCount;i++){
            cmdlp::CommandLineArg &arg= packet.args[i];
            if(arg.aliasIndex<0){
                if(arg.aliasIndex==-2&&functionForGlobalArgs!=NULL){
                    functionForGlobalArgs(arg,packet,aliasesCount,aliases);
                }
            }else if(arg.aliasIndex<functionsCountExcludingGlobalFunc && functions!=NULL){
                functions[arg.aliasIndex](arg,packet,aliasesCount,aliases);
            }
        }
    }
    void executeAlias(
                      const CommandLineArg &argToExecute,
                      const CommandLineAlias &aliasToExecute,
                      const bool automaticallyPrintDescription,
                      const CommandLineArgPacket & packet,
                      const CommandLineAliasPacket & aliasPacket
                      ){
        if(aliasToExecute.callbackFunction!=NULL){
            if(aliasToExecute.validatorFunction==NULL){
                aliasToExecute.callbackFunction(argToExecute,packet,aliasPacket.aliasesCount,aliasPacket.aliases);
            }else if(aliasToExecute.validatorFunction(argToExecute)){
                aliasToExecute.callbackFunction(argToExecute,packet,aliasPacket.aliasesCount,aliasPacket.aliases);
            }else if(automaticallyPrintDescription){
                printHelpBasedOnAlias(aliasToExecute);
            }
        }
    }
    /**For every argument in packet runs a corresponding alias and its ProcessorFunction provided that it exists. If
     ValidatorFunction exists too then it will be invoked earlier. If validation fails the description of alias will be automatically printed (unless you set automaticallyPrintDescription=false)*/
    void executePacket(
                       const CommandLineArgPacket & packet,
                       const CommandLineAliasPacket & aliasPacket,
                       const bool automaticallyPrintDescription){
        for(size_t i=0;i<packet.argsCount;i++){
            cmdlp::CommandLineArg &arg= packet.args[i];
            if(arg.aliasIndex<0){
                //ignoring aliasIndex=-1
                if(arg.aliasIndex==-2){
                    executeAlias(arg, aliasPacket.globalAlias,automaticallyPrintDescription,packet,aliasPacket);
                }
            }else if(arg.aliasIndex<aliasPacket.aliasesCount){
                executeAlias(arg, aliasPacket.aliases[arg.aliasIndex],automaticallyPrintDescription,packet,aliasPacket);
            }
        }
    }
    
}
