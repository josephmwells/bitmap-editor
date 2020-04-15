#include "bitmap.h"

// Added this in for testing
void redChannel(Bitmap & b)
{
  for (int y = 0; y < b.height(); ++y) {
    for (int x = 0; x < b.width(); ++x) {
      b.g(x, y) = 0;
      b.b(x, y) = 0;
    }
  }
}

void cellShade(Bitmap & b)
{
  for (int y = 0; y < b.height(); ++y) {
    for (int x = 0; x < b.width(); ++x) {
      uint8_t& red = (uint8_t&)b.r(x, y);
      uint8_t& green = (uint8_t&)b.g(x, y);
      uint8_t& blue = (uint8_t&)b.b(x, y);

      // Round pixel to nearest 3rd division of 255
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

      // Average each color channel to get greyscale
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
  
  for (int y = 0; y < block_height*block_size; y += block_size) {
    for (int x = 0; x < block_width*block_size; x += block_size) {
      uint32_t average_red = 0;
      uint32_t average_green = 0;
      uint32_t average_blue = 0;

      // Sum together 16x16 block pixels to find the average color
      for (int y_block = 0; y_block < block_size; ++y_block) {
        for (int x_block = 0; x_block < block_size; ++x_block) {
          average_red += b.r(x+x_block, y+y_block);
          average_green += b.g(x+x_block, y+y_block);
          average_blue += b.b(x+x_block, y+y_block);
        }
      }
      average_red = average_red/(block_size*block_size);
      average_green = average_green/(block_size*block_size);
      average_blue = average_blue/(block_size*block_size);

      // apply average to 16x16 block
      for (int y_block =0; y_block < block_size; ++y_block) {
        for (int x_block = 0; x_block < block_size; ++x_block) {
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
      uint32_t r_blur{0};
      uint32_t g_blur{0};
      uint32_t b_blur{0};
      // apply blur to pixels in a 5x5 block
      for(int i = -2; i <= 2; ++i) {
        for(int j = -2; j <= 2; ++j) {
          // If i or j is negative out of bounds, add black pixel
          if(x+j < 0 || y+i < 0) {
            continue;
          }
          // If i or j is positive out of bounds, add black pixel
          if(x+j >= b.width() || y+i >= b.height()) {
            continue;
          }
          r_blur += b.r(x+j, y+i) * gause_matrix.at((i+2) * 5 + (j+2)); 
          g_blur += b.g(x+j, y+i) * gause_matrix.at((i+2) * 5 + (j+2)); 
          b_blur += b.b(x+j, y+i) * gause_matrix.at((i+2) * 5 + (j+2)); 
        }
      }
      r_blur /= 256;
      g_blur /= 256;
      b_blur /= 256;

      b.r(x, y) = r_blur;
      b.g(x, y) = g_blur;
      b.b(x, y) = b_blur;
    }
  }
  
}

void rot90(Bitmap & b)
{
  uint32_t padding = 4 - (3 * b.height()) % 4;
  padding = (padding == 4) ? 0 : padding;
  std::vector<unsigned char> rotate;

  for(int x = b.width() - 1; x >= 0; --x) {
    for(int y = 0; y < b.height(); ++y) {
      if(b.depth() == 32)
        rotate.push_back(b.a(x, y));
      rotate.push_back(b.b(x, y)); 
      rotate.push_back(b.g(x, y));
      rotate.push_back(b.r(x, y));
    }
    if(b.depth() == 24)
      for (int i = 0; i < padding; ++i)
        rotate.push_back(0);
  }

  b.image() = rotate;
  int32_t width = b.width();
  int32_t height = b.height();
  b.width() = height;
  b.height() = width;
  b.size() = rotate.size();
  b.fsize() = b.size() + b.data_offset();
}

void rot180(Bitmap & b)
{
  std::vector<unsigned char> rotate;

  for(int y = b.height() - 1; y >= 0; --y) {
    for(int x = b.width() - 1; x >= 0; --x) {
      if(b.depth() == 32)
        rotate.push_back(b.a(x, y));
      rotate.push_back(b.b(x, y)); 
      rotate.push_back(b.g(x, y));
      rotate.push_back(b.r(x, y));
    }
    if(b.depth() == 24)
      for (int i = 0; i < b.padding(); ++i)
        rotate.push_back(0);
  }

  b.image() = rotate;
}

void rot270(Bitmap & b)
{
  uint32_t padding = 4 - (3 * b.height()) % 4;
  padding = (padding == 4) ? 0 : padding;
  std::vector<unsigned char> rotate;

  for(int x = 0; x < b.width(); ++x) {
    for(int y = b.height() - 1; y >= 0; --y) {
      if(b.depth() == 32)
        rotate.push_back(b.a(x, y));
      rotate.push_back(b.b(x, y)); 
      rotate.push_back(b.g(x, y));
      rotate.push_back(b.r(x, y));
    }
    if(b.depth() == 24)
      for (int i = 0; i < padding; ++i)
        rotate.push_back(0);
  }

  b.image() = rotate;
  int32_t width = b.width();
  int32_t height = b.height();
  b.width() = height;
  b.height() = width;
  b.size() = rotate.size();
  b.fsize() = b.size() + b.data_offset();
}

void flipv(Bitmap & b)
{
  std::vector<unsigned char> flipped;

  for (int y = b.height() - 1; y >= 0; --y) {
    for(int x = 0; x < b.width(); ++x) {
      if(b.depth() == 32)
        flipped.push_back(b.a(x, y));
      flipped.push_back(b.b(x, y));
      flipped.push_back(b.g(x, y));
      flipped.push_back(b.r(x, y));
    }
    for (int i = 0; i < b.padding(); i++)
      flipped.push_back(0);
  }
  b.image() = flipped;
}

void fliph(Bitmap & b)
{
  std::vector<unsigned char> flipped;

  for (int y = 0; y < b.height(); ++y) {
    for(int x = b.width() - 1; x >= 0; --x) {
      if(b.depth() == 32)
        flipped.push_back(b.a(x, y));
      flipped.push_back(b.b(x, y));
      flipped.push_back(b.g(x, y));
      flipped.push_back(b.r(x, y));
    }
    if(b.depth() == 24)
      for (int i = 0; i < b.padding(); i++)
        flipped.push_back(0);
  }
  b.image() = flipped;
}

void flipd1(Bitmap & b)
{
  uint32_t padding = 4 - (3 * b.height()) % 4;
  padding = (padding == 4) ? 0 : padding;
  std::vector<unsigned char> flipped;

  for(int x = b.width() -1; x >= 0; --x) {
    for (int y = b.height() - 1; y >= 0; --y) {
      if(b.depth() == 32)
        flipped.push_back(b.a(x, y));
      flipped.push_back(b.b(x, y));
      flipped.push_back(b.g(x, y));
      flipped.push_back(b.r(x, y));
    }
    if(b.depth() == 24)
      for (int i = 0; i < padding; i++)
        flipped.push_back(0);
  }
  b.image() = flipped;
  int32_t width = b.width();
  int32_t height = b.height();
  b.width() = height;
  b.height() = width;
  b.size() = flipped.size();
  b.fsize() = b.size() + b.data_offset();
}

void flipd2(Bitmap & b)
{
  uint32_t padding = 4 - (3 * b.height()) % 4;
  padding = (padding == 4) ? 0 : padding;
  std::vector<unsigned char> flipped;

  for(int x = 0; x < b.width(); ++x) {
    for (int y = 0; y < b.height(); ++y) {
      if(b.depth() == 32)
        flipped.push_back(b.a(x, y));
      flipped.push_back(b.b(x, y));
      flipped.push_back(b.g(x, y));
      flipped.push_back(b.r(x, y));
    }
    if(b.depth() == 24)
      for (int i = 0; i < padding; i++)
        flipped.push_back(0);
  }
  b.image() = flipped;
  int32_t width = b.width();
  int32_t height = b.height();
  b.width() = height;
  b.height() = width;
  b.size() = flipped.size();
  b.fsize() = b.size() + b.data_offset();
}

void scaleUp(Bitmap & b)
{ 
  std::vector<unsigned char> scaled;

  uint32_t padding = 4 - ((3 * b.width()*2) % 4);
  padding = (padding == 4) ? 0 : padding;

  for(int y = 0; y < b.height(); ++y) {
    std::vector<unsigned char> row;
    for(int x = 0; x < b.width(); ++x) {
      unsigned char red = b.r(x, y);
      unsigned char green = b.g(x, y);
      unsigned char blue = b.b(x, y);

      // Duplicate a pixel
      for (int i = 0; i < 2; i++) {
        if(b.depth() == 32) {
          unsigned char alpha = b.a(x, y);
          row.push_back(alpha);
        }
        row.push_back(blue);
        row.push_back(green);
        row.push_back(red);
      }
    }
    if(b.depth() == 24) {
      for (int i = 0; i < padding; ++i) {
        row.push_back(0);
      }
    }

    // Duplicate a row
    scaled.insert(scaled.end(), row.begin(), row.end());
    scaled.insert(scaled.end(), row.begin(), row.end());
  }

  // update header values to reflect scaling
  b.image() = scaled;
  b.height() *= 2;
  b.width() *= 2;
  b.size() = scaled.size();
  b.fsize() = b.size() + b.data_offset();
  b.padding() = padding;
}

void scaleDown(Bitmap & b)
{
  std::vector<unsigned char> scaled;

  uint32_t padding = 4 - ((3 * (b.width()/2)) % 4);
  padding = (padding == 4) ? 0 : padding;

  uint32_t trunc_width = (b.width()/2)*2;
  uint32_t trunc_height = (b.height()/2)*2;

  // Skip every other pixel and row
  for(int y = 0; y < trunc_height; y += 2) {
    for(int x = 0; x < trunc_width; x += 2) {
      if(b.depth() == 32)
        scaled.push_back(b.a(x, y));
      scaled.push_back(b.b(x, y));
      scaled.push_back(b.g(x, y));
      scaled.push_back(b.r(x, y));
    }
    if(b.depth() == 24){
      for (int i = 0; i < padding; ++i) {
        scaled.push_back(0);
      }
    }
  }
  b.image() = scaled;
  b.height() /= 2;
  b.width() /= 2;
  b.size() = scaled.size();
  b.fsize() = b.size() + b.data_offset();
  b.padding() = padding;
}
