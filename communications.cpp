#include "communications.hpp"

//Transforms hexadecimal array to String readable format
String hexToAscii( String hex )
{
  uint16_t len = hex.length();
  String ascii = "";

  for ( uint16_t i = 0; i < len; i += 2 )
    ascii += (char)strtol( hex.substring( i, i + 2 ).c_str(), NULL, 16 );

  return ascii;
}
