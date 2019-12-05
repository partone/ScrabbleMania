//For trimming strings
//Code from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

#include <string>
const std::string whiteSpaces( " \f\n\r\t\v" );
void trimRight( std::string& str, const std::string& trimChars = whiteSpaces );

void trimLeft( std::string& str, const std::string& trimChars = whiteSpaces );

void trim( std::string& str, const std::string& trimChars = whiteSpaces );
