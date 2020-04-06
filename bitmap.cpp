#include "bitmap.h"

//
// Bitmap ----------------------------------------------------
//

Bitmap::Bitmap()
{
}

std::istream & operator>>(std::istream & in, Bitmap & b)
{
  int offset = 0;

  // Read header
  in >> b.file_type_[0] >> b.file_type_[1];
  if (strncmp(b.file_type_, "BM", 2) != 0) {
    throw BitmapException("invalid file type", offset);
  }
  offset += 2;
  in.read((char*)&b.file_size_, 4);
  offset += 4;
  in.read((char*)&b.garbage_, 4);
  offset += 4;
  in.read((char*)&b.data_offset_, 4);
  offset += 4;
  in.read((char*)&b.header_size_, 4);
  offset += 4;
  in.read((char*)&b.image_width_, 4);
  offset += 4;
  in.read((char*)&b.image_height_, 4);
  offset += 4;
  in.read((char*)&b.planes_, 2);
  offset += 2;
  in.read((char*)&b.color_depth_, 2);
  offset += 2;
  in.read((char*)&b.compression_, 4);
  offset += 4;
  in.read((char*)&b.data_size_, 4);
  offset += 4;
  in.read((char*)&b.x_pixels_per_meter_, 4);
  offset += 4;
  in.read((char*)&b.y_pixels_per_meter_, 4);
  offset += 4;
  in.read((char*)&b.total_colors_, 4);
  offset += 4;
  in.read((char*)&b.important_colors_, 4);
  offset += 4;

  // Only read this if there is compression
  if (b.compression_ == 3) {
    in.read((char*)&b.red_mask_, 4);
    offset += 4;
    in.read((char*)&b.green_mask_, 4);
    offset += 4;
    in.read((char*)&b.blue_mask_, 4);
    offset += 4;
    in.read((char*)&b.alpha_mask_, 4);
    offset += 4;
    for (int i = 0; i < 68; ++i) {
      b.color_space_.push_back(in.get());
      offset += 1;
    }
  }

  // Read in pixel data
  for (uint32_t i = 0; i < b.data_size_; ++i) {
    unsigned char pixel;
    in.read((char*)&pixel, 1);
    b.data_.push_back(pixel);
    offset += 1;
  }

  // Add padding if 24 bit image
  if (b.color_depth_ == 24) {
    uint32_t padding = 4 - ((3 * b.image_width_) % 4);
    b.padding_ = (padding == 4) ? 0 : padding;
  }

  std::cout << offset << " bytes written\n";
  b.print_header();
  return in;
}

std::ostream & operator<<(std::ostream & out, const Bitmap & b)
{  
  int offset = 0;

  // Write header
  out << b.file_type_[0] << b.file_type_[1];
  offset += 2;
  out.write((char*)&b.file_size_, 4);
  offset += 4;
  out.write((char*)&b.garbage_, 4);
  offset += 4;
  out.write((char*)&b.data_offset_, 4);
  offset += 4;
  out.write((char*)&b.header_size_, 4);
  offset += 4;
  out.write((char*)&b.image_width_, 4);
  offset += 4;
  out.write((char*)&b.image_height_, 4);
  offset += 4;
  out.write((char*)&b.planes_, 2);
  offset += 2;
  out.write((char*)&b.color_depth_, 2);
  offset += 2;
  out.write((char*)&b.compression_, 4);
  offset += 4;
  out.write((char*)&b.data_size_, 4);
  offset += 4;
  out.write((char*)&b.x_pixels_per_meter_, 4);
  offset += 4;
  out.write((char*)&b.y_pixels_per_meter_, 4);
  offset += 4;
  out.write((char*)&b.total_colors_, 4);
  offset += 4;
  out.write((char*)&b.important_colors_, 4);
  offset += 4;

  // Only write this if there is compression
  if (b.compression_ == 3) {
    out.write((char*)&b.red_mask_, 4);
    offset += 4;
    out.write((char*)&b.green_mask_, 4);
    offset += 4;
    out.write((char*)&b.blue_mask_, 4);
    offset += 4;
    out.write((char*)&b.alpha_mask_, 4);
    offset += 4;
    for (uint8_t color_space_ : b.color_space_) {
      out.write((char*)&color_space_, 1);
      offset += 1;
    }
   
  }
  
  // Write pixel data
  for (unsigned char pixel : b.data_) {
    out.write((char*)&pixel, 1);
    offset += 1;
  }

  std::cout << std::dec <<  offset << " bytes written\n";
  return out;
}

void Bitmap::print_header()
{
  std::cout << "File Type: " << file_type_ << '\n';
  std::cout << "File Size: " << file_size_  << '\n';
  std::cout << "Pixel Data Offset:  " << data_offset_ << '\n';
  std::cout << "Header Size: " << header_size_ << '\n';
  std::cout << "Image Width: " << image_width_ << '\n';
  std::cout << "Image Height: " << image_height_ << '\n';
  std::cout << "Planes: " << planes_ << '\n';
  std::cout << "Color Depth: " << color_depth_ << '\n';
  std::cout << "Compression: " << compression_ << '\n';
  std::cout << "Raw Bitmap Size: " << data_size_ << '\n';
  std::cout << "X Pixels Per Meter: " << x_pixels_per_meter_ << '\n';
  std::cout << "Y Pixels Per Meter: " << y_pixels_per_meter_ << '\n';
  std::cout << "Total Colors: " << total_colors_ << '\n';
  std::cout << "Important Colors: " << important_colors_ << '\n';

  if(color_depth_ == 24)
    std::cout << "Padding: " << padding_ << '\n';

  if (compression_ == 3) {
    std::cout << "Red Mask: " << std::hex << red_mask_ << '\n';
    std::cout << "Gree Mask: " << std::hex << green_mask_ << '\n';
    std::cout << "Blue Mask: " << std::hex << blue_mask_ << '\n';
    std::cout << "Alpha Mask: " << std::hex << alpha_mask_ << '\n';
    std::cout << std::dec << '\n';
  }
}

unsigned char& Bitmap::r(uint32_t x, uint32_t y)
{
  switch(color_depth_) {
    case(24):
      // In order to reference the right pixel we need to add padding
      return data_.at(3 * ((y * image_width_ + x)) + (y * padding_) + 2);
    case(32):
      return data_.at(4 * (y * image_width_ + x) + 3);
  }
  throw BitmapException("invalid bitmap while retrieving red pixel data", 0);
}

unsigned char& Bitmap::g(uint32_t x, uint32_t y)
{
   switch(color_depth_) {
    case(24):
      return data_.at(3 * ((y * image_width_ + x)) + (y * padding_) + 1);
    case(32):
      return data_.at(4 * (y * image_width_ + x) + 2);
   }
   throw BitmapException("invalid bitmap while retrieving green pixel data", 0);
}

unsigned char& Bitmap::b(uint32_t x, uint32_t y)
{
  switch(color_depth_) {
    case(24):
      return data_.at(3 * ((y * image_width_ + x)) + (y * padding_));
    case(32):
      return data_.at(4 * (y * image_width_ + x) + 1);
   }
   throw BitmapException("invalid bitmap while retrieving blue pixel data", 0);
}

unsigned char& Bitmap::a(uint32_t x, uint32_t y)
{
  switch(color_depth_) {
    case(24):
      throw BitmapException("no alpha channel in 24bit image", 4 * (y * image_width_ + x));
    case(32):
      return data_.at(4 * (y * image_width_ + x));
   }
   throw BitmapException("invalid bitmap while retrieving alpha pixel data", 0);
}

//
// BitmapException ------------------------------------------------------
//

BitmapException::BitmapException(const std::string & message, uint32_t position)
{
  _message = message;
  _position = position;
}

BitmapException::BitmapException(std::string & message, uint32_t position)
{
  _message = message;
  _position = position;
}

void BitmapException::print_exception()
{
  std::cout << "Error in bitmap at position 0x" << std::hex << _position;
  std::cout << " : " << _message << '\n';
}

const char* BitmapException::what() const noexcept
{
  return "Reading in the bitmap has failed";
}
