//
//  sortPacket.cpp
//  CommandLineParser
//
//  Created by Alagris on 21/12/2016.
//  Copyright © 2016 alagris. All rights reserved.
//

#include "sortPacket.hpp"
#include <algorithm>

namespace cmdlp{
    bool comparator(const CommandLineArg & arg1,const CommandLineArg & arg2){
        return arg1.aliasIndex<arg2.aliasIndex;
    }
    void sortPacketArguments(CommandLineArgPacket & packet){
        std::sort(packet.args,packet.args+packet.argsCount,comparator);
    }
}
