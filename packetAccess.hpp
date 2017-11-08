//
//  packetAccess.hpp
//  MyManager
//
//  Created by Alagris on 19/12/2016.
//  Copyright © 2016 alagris. All rights reserved.
//

#ifndef packetAccess_hpp
#define packetAccess_hpp

#include "objects.hpp"

namespace cmdlp{

    
    void commandLineEngineProcess(
                                  const int argc,
                                  char *  argv[],
                                  const bool allowGlobalArgs,
                                  const size_t aliasesCount,
                                  const CommandLineAlias * aliases,
                                  CommandLineArgPacket & packetOutput
                                  );
    void initParams(CommandLineArg & arg);
    void commandLineEngineDestroy(const CommandLineArgPacket & packet);
   
}
#endif /* packetAccess_hpp */
