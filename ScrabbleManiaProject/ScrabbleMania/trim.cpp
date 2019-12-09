/*
   Eric Parton
   Luis Barajas
   
   For trimming strings
    Code from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
*/
#include "trim.h"

void trimRight( std::string& str, const std::string& trimChars)
{
   std::string::size_type pos = str.find_last_not_of( trimChars );
   str.erase( pos + 1 );
}


void trimLeft( std::string& str, const std::string& trimChars)
{
   std::string::size_type pos = str.find_first_not_of( trimChars );
   str.erase( 0, pos );
}


void trim( std::string& str, const std::string& trimChars)
{
   trimRight( str, trimChars );
   trimLeft( str, trimChars );
}
