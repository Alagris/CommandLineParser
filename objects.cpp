//
//  objects.cpp
//  CommandLineParser
//
//  Created by Alagris on 19/12/2016.
//  Copyright © 2016 alagris. All rights reserved.
//
#include <iostream>
#include "objects.hpp"

namespace cmdlp {
    CommandLineArg * NULL_COMMAND = {};
    /**This is assigned only if alias is not known(.aliasIndex=-1) or there really are no parameters*/
    Parameters NO_PARAMETERS=NULL;
    CommandLineArg EMPTY_ARG{-1,NO_PARAMETERS,0};
    void printHelpBasedOnAlias(const CommandLineAlias & alias){
        
        std::cout<<"\t-"<<alias.shortName;
        if(alias.longName!=NULL){
            std::cout<<"\t\t--"<<alias.longName;
        }
        std::cout<<'\n';
        if(alias.description!=NULL){
            std::cout<<alias.description<<'\n';
        }
        std::cout<<'\n';//extra line for clarity
    }
    
    void printHelpBasedOnAliases(const size_t aliasesCount,const CommandLineAlias * aliases){
        for(size_t i=0;i<aliasesCount;i++){
           printHelpBasedOnAlias(aliases[i]);
        }
    }
    void printHelpBasedOnAliasPacket(const CommandLineAliasPacket & aliasPacket){
        printHelpBasedOnAliases(aliasPacket.aliasesCount,aliasPacket.aliases);
    }

    
    /**
     constraints
     filed.length>1
     because it's impossible to pass '\0' as argument to command line
     */
    bool isParam(const  char * const field){
        //    filed[0]=='\0' is never true
        if(field[1]=='\0')return true;
        if(field[2]=='\0')return field[0]!='-' || field[1]=='-';
        return field[0]!='-';
    }
    
    
    /**
     constraints
     filed.length>1
     because it's impossible to pass empty string ('\0') as argument to command line
     */
    bool isArg(const char * const field){
        //    filed[0]=='\0' is never true
        if(field[1]=='\0')return false;
        if(field[2]=='\0')return field[0]=='-' && field[1]!='-';
        return field[0]=='-';
    }
    
    size_t containsUnknownArg(const CommandLineArgPacket & packet){
        for(size_t i=0;i<packet.argsCount;i++){
            if(packet.args[i].aliasIndex==-1){
                return i;
            }
        }
        return size_t(-1);
    }


}
