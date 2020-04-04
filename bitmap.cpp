#include "bitmap.h"

Bitmap::Bitmap()
{
}

std::istream & operator>>(std::istream & in, Bitmap & b)
{
  int offset{0};

  // Read in file type
  in >> b.file_type[0] >> b.file_type[1];
  b.file_type[2] = '\0';
  if(strncmp(b.file_type, "BM", 2) != 0) {
    std::cout << "Error in file type" << '\n';
    return in;
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

  for(int i = 0; i < b.raw_bitmap_size; ++i)
  {
    uint32_t pixel;
    in.read((char*)&pixel, 4);
    b.data.push_back(pixel);
  }

  std::cout << "Current offset after reading in \"important colors\" field: " << offset << '\n';
  b.print_header();

  return in;
}

std::ostream & operator<<(std::ostream & in, const Bitmap & b)
{  
  int offset{0};

  // Read in file type
  in << b.file_type[0] << b.file_type[1];
  if(strncmp(b.file_type, "BM", 2) != 0) {
    std::cout << "Error in file type" << '\n';
    return in;
  }
  offset += 2;

  // Read in file size
  in.write((char*)&b.file_size, 4);
  offset += 4;
  // Read in garbage
  in.write((char*)&b.garbage, 4);
  offset += 4;
  // Read in pixel data offset
  in.write((char*)&b.pixel_data_offset, 4);
  offset += 4;
  // Read in header size
  in.write((char*)&b.header_size, 4);
  offset += 4;
  // Read in image width
  in.write((char*)&b.image_width, 4);
  offset += 4;
  // Read in image height
  in.write((char*)&b.image_height, 4);
  offset += 4;
  // Read in planes
  in.write((char*)&b.planes, 2);
  offset += 2;
  // Read in bits per pixel
  in.write((char*)&b.bits_per_pixel, 2);
  offset += 2;
  // Read in compression
  in.write((char*)&b.compression, 4);
  offset += 4;
  // Read in raw bitmap size
  in.write((char*)&b.raw_bitmap_size, 4);
  offset += 4;
  // Read in x pixels per meter
  in.write((char*)&b.x_pixels_per_meter, 4);
  offset += 4;
  // Read in y pixels per meter
  in.write((char*)&b.y_pixels_per_meter, 4);
  offset += 4;
  // Read in total colors
  in.write((char*)&b.total_colors, 4);
  offset += 4;
  // Read in important colors
  in.write((char*)&b.important_colors, 4);
  offset += 4;

  if(b.compression == 3) {
    // Read in red mask
    in.write((char*)&b.red_mask, 4);
    offset += 4;
    // Read in green mask
    in.write((char*)&b.green_mask, 4);
    offset += 4;
    // Read in blue mask
    in.write((char*)&b.blue_mask, 4);
    offset += 4;
    // Read in alpha mask
    in.write((char*)&b.alpha_mask, 4);
    offset += 4;
    // Read in color space
    for(uint8_t color_space : b.color_space)
    {
     in << color_space;
     offset += 1;
    }
   
  }

  for(uint32_t pixel : b.data)
  {
    in << pixel;
    offset += 1;
  }

  return in;
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
  }
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

const char* BitmapException::what() const noexcept
{
  return "Reading in the bitmap has failed";
}
