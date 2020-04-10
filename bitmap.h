#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>

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

    // should these be public ?
    // otherwise I just use getter and setter functions with slightly altered variable names
    
    // Header Data
    // Block 1
    char file_type_[2];
    uint32_t file_size_;
    uint32_t garbage_;
    uint32_t data_offset_;

    // Block 2
    uint32_t header_size_;
    int32_t image_width_;
    int32_t image_height_;
    uint16_t planes_;
    uint16_t color_depth_;
    uint32_t compression_;
    uint32_t data_size_;
    int32_t x_pixels_per_meter_;
    int32_t y_pixels_per_meter_;
    uint32_t total_colors_;
    uint32_t important_colors_;

    // Block 3
    uint32_t red_mask_;
    uint32_t green_mask_;
    uint32_t blue_mask_;
    uint32_t alpha_mask_;
    std::vector<uint8_t> color_space_; // 68 bytes of this
    
    std::vector<unsigned char> data_;

    uint32_t padding_;

public:
    Bitmap();

    void print_header();

    // Pixel getters/setters
    unsigned char& r(uint32_t x, uint32_t y);
    unsigned char& g(uint32_t x, uint32_t y);
    unsigned char& b(uint32_t x, uint32_t y);
    unsigned char& a(uint32_t x, uint32_t y);

    // Getters and Setters vs public variables?
    int32_t& width() {return image_width_;}
    int32_t& height() {return image_height_;}
    uint32_t& size() {return data_size_;}
    uint32_t& fsize() {return file_size_;}
    uint32_t& padding() {return padding_;}
    std::vector<unsigned char>& image() {return data_;}

    uint32_t depth() const {return color_depth_;}
    uint32_t data_offset() const {return data_offset_;}
};

void redChannel(Bitmap& b);

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

    /**
     * prints out the exception in the form:
     *
     * "Error in bitmap at position 0xposition :
     * message"
     */
    void print_exception();
};
