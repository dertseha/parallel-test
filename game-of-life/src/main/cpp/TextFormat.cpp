#include "gol/TextFormat.h"

using gol::Image;
using gol::TextFormat;

TextFormat::TextFormat(char deadCell)
   : deadCell(deadCell)
{
}

TextFormat TextFormat::withDefaults()
{
   return TextFormat('.');
}

void TextFormat::load(Image &out, std::istream &in) const
{
   for (size_t y = 0; y < out.getHeight(); y++)
   {
      char value = 0;
      while (!std::isprint(value) || std::isblank(value))
      {
         in.get(value);
      }
      out.getRow(y)[0] = (value == deadCell) ? 0x00 : 0xFF;
      for (size_t x = 1; x < out.getWidth(); x++)
      {
         in.get(value);
         out.getRow(y)[x] = (value == deadCell) ? 0x00 : 0xFF;
      }
   }
}

void TextFormat::save(std::ostream &out, Image const &in) const
{
   for (size_t y = 0; y < in.getHeight(); y++)
   {
      auto it = in.getRow(y);
      auto end = it + in.getWidth();
      while (it != end)
      {
         out << ((*it == 0xFF) ? 'O' : deadCell);
         it++;
      }
      out << '\n';
   }
}
