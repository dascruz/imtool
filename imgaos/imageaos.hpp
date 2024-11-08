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

  class Image : image::Image {
    public:
      Pixel & getPixel(unsigned long xPos, unsigned long yPos);
      [[nodiscard]] Pixel const & getPixel(unsigned long xPos, unsigned long yPos) const;

      bool loadFromFile(std::string const & filePath);
      [[nodiscard]] bool saveToFile(std::string const & filePath) const;
      void displayMetadata() const;
      void modifyMaxLevel(unsigned short newMaxColorValue);

    private:
      bool readPixelData(std::ifstream & file);
      bool writePixelData(std::ofstream & file) const;

      std::vector<Pixel> pixels_;
  };
}  // namespace imageaos
