//
//  packetTest.cpp
//  CommandLineParser
//
//  Created by Alagris on 22/12/2016.
//  Copyright © 2016 alagris. All rights reserved.
//

#include "packetTest.hpp"

#include <iostream>
#include "packetAccess.hpp"
#include "sortPacket.hpp"
#include "executePacket.hpp"
namespace cmdlp{
    bool validator1(const CommandLineArg & arg){
        std::cout<<"arg1 validator\n";
        return true;
    }
    
    bool validator2(const CommandLineArg & arg){
        std::cout<<"arg2 validator\n";
        return false;
    }
    void function1(const CommandLineArg & arg,
                   const CommandLineArgPacket & packet,
                   const size_t aliasesCount,
                   const CommandLineAlias * aliases){
        std::cout<<"arg1 processor\n";
    }
    
    void function2(const CommandLineArg & arg,
                   const CommandLineArgPacket & packet,
                   const size_t aliasesCount,
                   const CommandLineAlias * aliases){
        std::cout<<"arg2 processor\n";
    }
    
    CommandLineArgPacket packet;
    int argc=16;
    char * argv[]{
        (char*)"/this/is/execution/path",
        (char *)"gl",
        (char *)"gl2",
        (char *)"-arg0",
        (char *)"param01",
        (char *)"param02",
        (char *)"-arg_1",
        (char *)"param11",
        (char *)"--arg_-1",
        (char *)"-",
        (char*)"--arg-0",
        (char*)"param03",
        (char*)"--unknwon",
        (char*)"dontCareAboutThisParam",
        (char*)"--arg-0",
        (char*)"param04"};
    CommandLineAlias aliases[]{
        CommandLineAlias("arg0","arg-0","description 0",function1,validator1),
        CommandLineAlias("arg_1","arg_-1","description 1",function2,validator2)
    };
    
    void test(){
        
        commandLineEngineProcess(argc,argv,true, 2, aliases,packet);
        for(size_t i=0;i<packet.argsCount;i++){
            std::cout<<packet.args[i].aliasIndex<<" "<<packet.args[i].paramsCount<<std::endl;
            for(size_t ii=0;ii<packet.args[i].paramsCount;ii++){
                std::cout<<"\t"<<packet.args[i].params[ii]<<std::endl;
            }
        }
        std::cout<<"\n\n";
        sortPacketArguments(packet);
        for(size_t i=0;i<packet.argsCount;i++){
            std::cout<<packet.args[i].aliasIndex<<" "<<packet.args[i].paramsCount<<std::endl;
            for(size_t ii=0;ii<packet.args[i].paramsCount;ii++){
                std::cout<<"\t"<<packet.args[i].params[ii]<<std::endl;
            }
        }
        
        std::cout<<"\n\n";
        ProcessorFunction e[] = {function1,function2};
        executePacket(packet, 2, e, NULL, 2, aliases);
        
        std::cout<<"\n\n";
        CommandLineAliasPacket aliasPacket(aliases,2);
        executePacket(packet, aliasPacket);
        commandLineEngineDestroy(packet);
    }
    
}
