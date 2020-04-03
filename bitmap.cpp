#include "bitmap.h"

Bitmap::Bitmap()
{
}

std::istream & operator>>(std::istream & in, Bitmap & b)
{
  return in;
}

std::ostream & operator<<(std::ostream & in, const Bitmap & b)
{
  return in;
}

void cellShade(Bitmap & b)
{
}

void grayscale(Bitmap & b)
{
}

void pixelate(Bitmap & b)
{
}

void blur(Bitmap & b)
{
}

void rot90(Bitmap & b)
{
}

void rot180(Bitmap & b)
{
}

void rot270(Bitmap & b)
{
}

void flipv(Bitmap & b)
{
}

void fliph(Bitmap & b)
{
}

void flipd1(Bitmap & b)
{
}

void flipd2(Bitmap & b)
{
}

void scaleUp(Bitmap & b)
{
}

void scaleDown(Bitmap & b)
{
}

BitmapException::BitmapException(const std::string & message, uint32_t position)
{
}

BitmapException::BitmapException(std::string & message, uint32_t position)
{
}

void BitmapException::print_exception()
{
}


