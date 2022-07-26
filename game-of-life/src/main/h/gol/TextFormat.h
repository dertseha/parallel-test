#pragma once

#include <istream>
#include <map>

#include "gol/Buffer2d.h"

namespace gol
{
class TextFormat
{
public:
   static TextFormat withDefaults();

   void load(Buffer2d &out, std::istream &in) const;
   void save(std::ostream &out, Buffer2d const &in) const;

private:
   explicit TextFormat(std::map<uint8_t, char> const &charactersByValue);

   std::map<uint8_t, char> charactersByValue;
   std::map<char, uint8_t> valuesByCharacter;
};
}
