#include "gol/TextFormat.h"

using gol::Buffer2d;
using gol::TextFormat;

TextFormat::TextFormat(std::map<uint8_t, char> const &charactersByValue)
   : charactersByValue(charactersByValue)
{
   for (auto const &kvp : charactersByValue)
   {
      valuesByCharacter[kvp.second] = kvp.first;
   }
}

TextFormat TextFormat::withDefaults()
{
   static std::map<uint8_t, char> const charactersByValue = { { 0x00, '.' }, { 0xFF, 'O' } };
   return TextFormat(charactersByValue);
}

void TextFormat::load(Buffer2d &out, std::istream &in) const
{
   for (size_t y = 0; y < out.getHeight(); y++)
   {
      char value = 0;
      while (!std::isprint(value) || std::isblank(value))
      {
         in.get(value);
      }
      out.getRow(y)[0] = valuesByCharacter.at(value);
      for (size_t x = 1; x < out.getWidth(); x++)
      {
         in.get(value);
         out.getRow(y)[x] = valuesByCharacter.at(value);
      }
   }
}

void TextFormat::save(std::ostream &out, Buffer2d const &in) const
{
   for (size_t y = 0; y < in.getHeight(); y++)
   {
      auto it = in.getRow(y);
      auto end = it + in.getWidth();
      while (it != end)
      {
         out << charactersByValue.at(*it);
         it++;
      }
      out << '\n';
   }
}
