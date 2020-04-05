#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <functional>

class Bitmap
{
private:

    /**
     * Read in an image.
     * reads a bitmap in from the stream
     *
     * @param in the stream to read from.
     * @param b the bitmap that we are creating.
     *
     * @return the stream after we've read in the image.
     *
     * @throws BitmapException if it's an invalid bitmap.
     * @throws bad_alloc exception if we failed to allocate memory.
     */
    friend std::istream& operator>>(std::istream& in, Bitmap& b);

    /**
     * Write the binary representation of the image to the stream.
     *
     * @param out the stream to write to.
     * @param b the bitmap that we are writing.
     *
     * @return the stream after we've finished writting.
     *
     * @throws failure if we failed to write.
     */
    friend std::ostream& operator<<(std::ostream& out, const Bitmap& b);

    // Header Data
    // Block 1
    char file_type[2];
    uint32_t file_size;
    uint32_t garbage;
    uint32_t pixel_data_offset;
    // Block 2
    uint32_t header_size;
    int32_t image_width, image_height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t raw_bitmap_size;
    int32_t x_pixels_per_meter, y_pixels_per_meter;
    uint32_t total_colors;
    uint32_t important_colors;
    // Block 3
    uint32_t red_mask, green_mask, blue_mask, alpha_mask;
    std::vector<uint8_t> color_space; // 68 bytes of this
    
    std::vector<unsigned char> data;

public:
    Bitmap();

    void print_header();

    // Pixel getters/setters
    unsigned char& r(uint32_t x, uint32_t y);
    unsigned char& g(uint32_t x, uint32_t y);
    unsigned char& b(uint32_t x, uint32_t y);
    unsigned char& a(uint32_t x, uint32_t y);

    const uint32_t width() {return image_width;}
    const uint32_t height() {return image_height;}
};

/**
 * cell shade an image.
 * for each component of each pixel we round to 
 * the nearest number of 0, 180, 255
 *
 * This has the effect of making the image look like.
 * it was colored.
 */
void cellShade(Bitmap& b);

/**
 * Grayscales an image by averaging all of the components.
 */
void grayscale(Bitmap& b);

/**
 * Pixelats an image by creating groups of 16*16 pixel blocks.
 */
void pixelate(Bitmap& b);

/**
 * Use gaussian bluring to blur an image.
 */
void blur(Bitmap& b);

/**
 * rotates image 90 degrees, swapping the height and width.
 */
void rot90(Bitmap& b);

/**
 * rotates an image by 180 degrees.
 */
void rot180(Bitmap& b);

/**
 * rotates image 270 degrees, swapping the height and width.
 */
void rot270(Bitmap& b);

/**
 * flips and image over the vertical axis.
 */
void flipv(Bitmap& b);

/**
 * flips and image over the horizontal axis.
 */
void fliph(Bitmap& b);

/**
 * flips and image over the line y = -x, swapping the height and width.
 */
void flipd1(Bitmap& b);

/**
 * flips and image over the line y = xr, swapping the height and width.
 */
void flipd2(Bitmap& b);

/**
 * scales the image by a factor of 2.
 */
void scaleUp(Bitmap& b);

/**
 * scales the image by a factor of 1/2.
 */
void scaleDown(Bitmap& b);



/**
 * BitmapException denotes an exception from reading in a bitmap.
 */
class BitmapException : public std::exception
{
    // the message to print out
    std::string _message;

    // position in the bitmap file (in bytes) where the error occured.
    uint32_t _position;

public:
    BitmapException() = delete;

    BitmapException(const std::string& message, uint32_t position);
    BitmapException(std::string& message, uint32_t position);

    const char* what() const noexcept;
    /**
     * prints out the exception in the form:
     *
     * "Error in bitmap at position 0xposition :
     * message"
     */
    void print_exception();
};
