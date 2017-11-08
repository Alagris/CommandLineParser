//
//  commandLineEngine.hpp
//  CommandLineParser
//
//  Created by Alagris on 24/12/2016.
//  Copyright © 2016 alagris. All rights reserved.
//

#ifndef commandLineEngine_hpp
#define commandLineEngine_hpp

#include "objects.hpp"
namespace cmdlp{
    typedef bool (*PacketValidatorFunction)(const CommandLineArgPacket & packet);
    void fullEngineExecution(const int argc,char *  argv[],const CommandLineAliasPacket & aliasPacket,const PacketValidatorFunction func);
}
#endif /* commandLineEngine_hpp */
