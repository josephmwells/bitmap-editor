#include "bitmap.h"

Bitmap::Bitmap()
{
}

std::istream & operator>>(std::istream & in, Bitmap & b)
{
  int offset = 0;

  // Read in file type
  in >> b.file_type[0] >> b.file_type[1];
  if(strncmp(b.file_type, "BM", 2) != 0) {
    throw BitmapException("invalid file type", offset);
  }
  offset += 2;

  // Read in file size
  in.read((char*)&b.file_size, 4);
  offset += 4;
  // Read in garbage
  in.read((char*)&b.garbage, 4);
  offset += 4;
  // Read in pixel data offset
  in.read((char*)&b.pixel_data_offset, 4);
  offset += 4;
  // Read in header size
  in.read((char*)&b.header_size, 4);
  offset += 4;
  // Read in image width
  in.read((char*)&b.image_width, 4);
  offset += 4;
  // Read in image height
  in.read((char*)&b.image_height, 4);
  offset += 4;
  // Read in planes
  in.read((char*)&b.planes, 2);
  offset += 2;
  // Read in bits per pixel
  in.read((char*)&b.bits_per_pixel, 2);
  offset += 2;
  // Read in compression
  in.read((char*)&b.compression, 4);
  offset += 4;
  // Read in raw bitmap size
  in.read((char*)&b.raw_bitmap_size, 4);
  offset += 4;
  // Read in x pixels per meter
  in.read((char*)&b.x_pixels_per_meter, 4);
  offset += 4;
  // Read in y pixels per meter
  in.read((char*)&b.y_pixels_per_meter, 4);
  offset += 4;
  // Read in total colors
  in.read((char*)&b.total_colors, 4);
  offset += 4;
  // Read in important colors
  in.read((char*)&b.important_colors, 4);
  offset += 4;

  if(b.compression == 3) {
    // Read in red mask
    in.read((char*)&b.red_mask, 4);
    offset += 4;
    // Read in green mask
    in.read((char*)&b.green_mask, 4);
    offset += 4;
    // Read in blue mask
    in.read((char*)&b.blue_mask, 4);
    offset += 4;
    // Read in alpha mask
    in.read((char*)&b.alpha_mask, 4);
    offset += 4;
    // Read in color space
    for(int i = 0; i < 68; ++i)
    {
      b.color_space.push_back(in.get());
      offset += 1;
    }
  }

  // Read in pixel data
  for(int i = 0; i < b.raw_bitmap_size; ++i)
  {
    unsigned char pixel;
    in.read((char*)&pixel, 1);
    b.data.push_back(pixel);
    offset += 1;
  }

  std::cout << offset << " bytes written\n";
  b.print_header();
  return in;
}

std::ostream & operator<<(std::ostream & out, const Bitmap & b)
{  
  int offset = 0;

  // Write file type
  out << b.file_type[0] << b.file_type[1];
  offset += 2;

  // Write file size
  out.write((char*)&b.file_size, 4);
  offset += 4;
  // Write garbage
  out.write((char*)&b.garbage, 4);
  offset += 4;
  // Write pixel data offset
  out.write((char*)&b.pixel_data_offset, 4);
  offset += 4;
  // Write header size
  out.write((char*)&b.header_size, 4);
  offset += 4;
  // Write image width
  out.write((char*)&b.image_width, 4);
  offset += 4;
  // Write image height
  out.write((char*)&b.image_height, 4);
  offset += 4;
  // Write planes
  out.write((char*)&b.planes, 2);
  offset += 2;
  // Write bits per pixel
  out.write((char*)&b.bits_per_pixel, 2);
  offset += 2;
  // Write compression
  out.write((char*)&b.compression, 4);
  offset += 4;
  // Write raw bitmap size
  out.write((char*)&b.raw_bitmap_size, 4);
  offset += 4;
  // Write x pixels per meter
  out.write((char*)&b.x_pixels_per_meter, 4);
  offset += 4;
  // Write y pixels per meter
  out.write((char*)&b.y_pixels_per_meter, 4);
  offset += 4;
  // Write total colors
  out.write((char*)&b.total_colors, 4);
  offset += 4;
  // Write important colors
  out.write((char*)&b.important_colors, 4);
  offset += 4;

  if(b.compression == 3) {
    // Write red mask
    out.write((char*)&b.red_mask, 4);
    offset += 4;
    // Write green mask
    out.write((char*)&b.green_mask, 4);
    offset += 4;
    // Write blue mask
    out.write((char*)&b.blue_mask, 4);
    offset += 4;
    // Write alpha mask
    out.write((char*)&b.alpha_mask, 4);
    offset += 4;
    // Write color space
    for(uint8_t color_space : b.color_space)
    {
      out.write((char*)&color_space, 1);
      offset += 1;
    }
   
  }
  
  // Write pixel data
  for(unsigned char pixel : b.data)
  {
    out.write((char*)&pixel, 1);
    offset += 1;
  }

  std::cout << std::dec <<  offset << " bytes written\n";
  return out;
}

void Bitmap::print_header()
{
  std::cout << "File Type: " << file_type << '\n';
  std::cout << "File Size:: " << file_size  << '\n';
  std::cout << "Pixel Data Offset:  " << pixel_data_offset << '\n';
  std::cout << "Header Size: " << header_size << '\n';
  std::cout << "Image Width: " << image_width << '\n';
  std::cout << "Image Height: " << image_height << '\n';
  std::cout << "Planes: " << planes << '\n';
  std::cout << "Bits Per Pixel: " << bits_per_pixel << '\n';
  std::cout << "Compression: " << compression << '\n';
  std::cout << "Raw Bitmap Size: " << raw_bitmap_size << '\n';
  std::cout << "X Pixels Per Meter: " << x_pixels_per_meter << '\n';
  std::cout << "Y Pixels Per Meter: " << y_pixels_per_meter << '\n';
  std::cout << "Total Colors: " << total_colors << '\n';
  std::cout << "Important Colors: " << important_colors << '\n';

  if(compression == 3)
  {
    std::cout << "Red Mask: " << std::hex << red_mask << '\n';
    std::cout << "Gree Mask: " << std::hex << green_mask << '\n';
    std::cout << "Blue Mask: " << std::hex << blue_mask << '\n';
    std::cout << "Alpha Mask: " << std::hex << alpha_mask << '\n';
    std::cout << std::dec << '\n';
  }
}

unsigned char& Bitmap::r(uint32_t x, uint32_t y)
{
  switch(bits_per_pixel) {
    case(24):
      return data.at(3 * (y * image_width + x) + 2);
    case(32):
      return data.at(4 * (y * image_width + x) + 3);
  }
  throw BitmapException("invalid bitmap", 0);
}

unsigned char& Bitmap::g(uint32_t x, uint32_t y)
{
   switch(bits_per_pixel) {
    case(24):
      return data.at(3 * (y * image_width + x) + 1);
    case(32):
      return data.at(4 * (y * image_width + x) + 2);
   }
   throw BitmapException("invalid bitmap", 0);
}

unsigned char& Bitmap::b(uint32_t x, uint32_t y)
{
  switch(bits_per_pixel) {
    case(24):
      return data.at(3 * (y * image_width + x));
    case(32):
      return data.at(4 * (y * image_width + x) + 1);
   }
   throw BitmapException("invalid bitmap", 0);
}

unsigned char& Bitmap::a(uint32_t x, uint32_t y)
{
  switch(bits_per_pixel) {
    case(24):
      throw BitmapException("no alpha channel in 24bit image", 4 * (y * image_width + x));
    case(32):
      return data.at(4 * (y * image_width + x));
   }
   throw BitmapException("invalid bitmap", 0);
}

void cellShade(Bitmap & b)
{
  for(int y = 0; y < b.height(); ++y) {
    for(int x = 0; x < b.width(); ++x) {
      uint8_t& red = (uint8_t&)b.r(x, y);
      uint8_t& green = (uint8_t&)b.g(x, y);
      uint8_t& blue = (uint8_t&)b.b(x, y);

      if(red < 64)
        red = 0;
      else if(red < 192)
        red = 128;
      else
        red = 255;
      if(green < 64)
        green = 0;
      else if(green < 192)
        green = 128;
      else
        green = 255;

      if(blue < 64)
        blue = 0;
      else if(blue < 192)
        blue = 128;
      else
        blue = 255;
    }
  }
}

void grayscale(Bitmap & b)
{
  for(int y = 0; y < b.height(); ++y) {
    for(int x = 0; x < b.width(); ++x) {
      uint8_t& red = (uint8_t&)b.r(x, y);
      uint8_t& green = (uint8_t&)b.g(x, y);
      uint8_t& blue = (uint8_t&)b.b(x, y);

      uint8_t greyscale = (red + green + blue) / 3;
      red = greyscale;
      blue = greyscale;
      green = greyscale;
    }
  }
}

void pixelate(Bitmap & b)
{
  uint32_t block_size = 16;

  uint32_t block_width = (b.width()/block_size);
  uint32_t block_height = (b.height()/block_size);
  
  for(uint32_t y = 0; y < block_height*block_size; y += block_size) {
    for(uint32_t x = 0; x < block_width*block_size; x += block_size) {
      uint32_t average_red = 0;
      uint32_t average_green = 0;
      uint32_t average_blue = 0;

      // Sum together 16x16 block pixels to find the average
      for(uint32_t y_block = 0; y_block < block_size; ++y_block) {
        for(uint32_t x_block = 0; x_block < block_size; ++x_block) {
          average_red += b.r(x+x_block, y+y_block);
          average_green += b.g(x+x_block, y+y_block);
          average_blue += b.b(x+x_block, y+y_block);
        }
      }
      average_red = average_red/(block_size*block_size);
      average_green = average_green/(block_size*block_size);
      average_blue = average_blue/(block_size*block_size);

      // apply average to 16x16 block
      for(uint32_t y_block =0; y_block < block_size; ++y_block) {
        for(uint32_t x_block = 0; x_block < block_size; ++x_block) {
          b.r(x+x_block, y+y_block) = average_red;
          b.g(x+x_block, y+y_block) = average_green;
          b.b(x+x_block, y+y_block) = average_blue;
        }
      }
    }
  }
}

void blur(Bitmap & b)
{
  std::vector<int> gause_matrix = {1,4,6,4,1,4,16,24,16,4,6,24,36,24,6,4,16,24,16,4,1,4,6,4,1}; 

  for(int y = 0; y < b.height(); ++y) {
    for(int x = 0; x < b.width(); ++x) {
      uint32_t r_new;
      uint32_t g_new;
      uint32_t b_new;
      for(int i = -2; i <= 2; ++i) {
        for(int j = -2; j <= 2; ++j) {
          // If i or j is negative out of bounds, add black pixel
          if(x+j < 0 || y+i < 0) 
          {
            continue;
          }
          // If i or j is positive out of bounds, add black pixel
          if(x+j >= b.width() || y+i >= b.height()) 
          {
            continue;
          }
          r_new += b.r(x+j, y+i) * gause_matrix.at((i+2) * 5 + (j+2)); 
          g_new += b.g(x+j, y+i) * gause_matrix.at((i+2) * 5 + (j+2)); 
          b_new += b.b(x+j, y+i) * gause_matrix.at((i+2) * 5 + (j+2)); 
        }
      }
      r_new /= 256;
      g_new /= 256;
      b_new /= 256;

      b.r(x, y) = r_new;
      b.g(x, y) = g_new;
      b.b(x, y) = b_new;
    }
  }
  
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
