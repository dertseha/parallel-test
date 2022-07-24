#pragma once

#include <istream>

#include "gol/Image.h"

namespace gol
{
class TextFormat
{
public:
   static TextFormat withDefaults();

   void load(Image &out, std::istream &in) const;
   void save(std::ostream &out, Image const &in) const;

private:
   explicit TextFormat(char deadCell);

   char deadCell;
};
}
