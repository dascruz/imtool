#pragma once
#include <common/image.hpp>
#include <string>
#include <vector>


namespace imageaos {
  constexpr unsigned char BYTE_SHIFT = 8;
  constexpr unsigned char BYTE_MASK  = 0xFF;

  struct Pixel {
      unsigned short red   = 0;
      unsigned short green = 0;
      unsigned short blue  = 0;

      void setRed(unsigned short const redValue) { red = redValue; }

      void setGreen(unsigned short const greenValue) { green = greenValue; }

      void setBlue(unsigned short const blueValue) { blue = blueValue; }
  };

  struct Dimensions {
    unsigned long width;
    unsigned long height;
  };

  class Image : image::Image {
    public:
      static constexpr unsigned short DEFAULT_MAX_COLOR_VALUE = 255;
      Image() : maxColorValue_(DEFAULT_MAX_COLOR_VALUE) {}

      explicit Image(Dimensions const dimensions,
            unsigned short const maxColorValue = DEFAULT_MAX_COLOR_VALUE)
          : maxColorValue_(maxColorValue) {
            setWidth(dimensions.width);
            setHeight(dimensions.height);
            pixels_.resize(dimensions.width * dimensions.height);
      }

      [[nodiscard]] unsigned long getWidth() const { return image::Image::getWidth(); }
      [[nodiscard]] unsigned long getHeight() const { return image::Image::getHeight(); }

      Pixel & getPixel(unsigned long xPos, unsigned long yPos);
      [[nodiscard]] Pixel const & getPixel(unsigned long xPos, unsigned long yPos) const;

      bool loadFromFile(std::string const & filePath);
      [[nodiscard]] bool saveToFile(std::string const & filePath) const;
      void displayMetadata() const;
      void modifyMaxLevel(unsigned short newMaxColorValue);
      void setPixel(unsigned long xPos, unsigned long yPos, const Pixel& pixel);

      [[nodiscard]] unsigned short getMaxColorValue() const { return maxColorValue_; }
      void setMaxColorValue(unsigned short const value) { maxColorValue_ = value; }

    private:
      bool readPixelData(std::ifstream & file);
      bool writePixelData(std::ofstream & file) const;

      unsigned short maxColorValue_;
      std::vector<Pixel> pixels_;
  };
}  // namespace imageaos
