//
//  commandLineEngine.cpp
//  CommandLineParser
//
//  Created by Alagris on 24/12/2016.
//  Copyright © 2016 alagris. All rights reserved.
//

#include "commandLineEngine.hpp"
#include "packetAccess.hpp"
#include "executePacket.hpp"
namespace cmdlp{
    /**Function func will be executed only if there are no unknown arguments. If there are then validation is skipped and help is automatically printed*/
    void fullEngineExecution(const int argc,char *  argv[],const CommandLineAliasPacket & aliasPacket,const PacketValidatorFunction func){
        CommandLineArgPacket packet;
        commandLineEngineProcess(argc, argv,false, aliasPacket.aliasesCount, aliasPacket.aliases, packet);
        size_t indexOfUnknownArg=containsUnknownArg(packet);
        if(indexOfUnknownArg==size_t(-1) && func(packet)){
            executePacket(packet, aliasPacket);
        }else{
            printHelpBasedOnAliasPacket(aliasPacket);
        }
        commandLineEngineDestroy(packet);
    }
}
