//
//  executePacket.hpp
//  CommandLineParser
//
//  Created by Alagris on 21/12/2016.
//  Copyright © 2016 alagris. All rights reserved.
//

#ifndef executePacket_hpp
#define executePacket_hpp

#include "objects.hpp"
namespace cmdlp{
    
    /**Index of function corresponds to index of alias. This function will execute one function for every alias processed and stored in packet.*/
    void executePacket(const CommandLineArgPacket & packet,
                       const size_t functionsCountExcludingGlobalFunc,
                       const ProcessorFunction * const functions,
                       const ProcessorFunction functionForGlobalArgs,
                       const size_t aliasesCount,
                       const CommandLineAlias * aliases);
    void executePacket(const CommandLineArgPacket & packet,
                       const CommandLineAliasPacket & aliasPacket,
                       const bool automaticallyPrintDescription=true);
}

#endif /* executePacket_hpp */
