#pragma once

#include <common/image.hpp>
#include <string>
#include <vector>

namespace imageaos {
  constexpr int MIN_COLOR_VALUE       = 1;
  constexpr int MAX_COLOR_VALUE_8BIT  = 255;
  constexpr int MAX_COLOR_VALUE_16BIT = 65535;
  constexpr int BIT_SHIFT             = 8;
  constexpr int BYTE_MASK             = 0xFF;

  struct Pixel {
      unsigned char red   = 0;
      unsigned char green = 0;
      unsigned char blue  = 0;

      void setRed(unsigned char const redValue) { red = redValue; }

      void setGreen(unsigned char const greenValue) { green = greenValue; }

      void setBlue(unsigned char const blueValue) { blue = blueValue; }
  };

  struct Dimensions {
      int width  = 0;
      int height = 0;
  };

  struct MaxColorValue {
      int value = MAX_COLOR_VALUE_8BIT;
  };

  class Image : image::Image {
    public:
      Pixel & getPixel(int xPos, int yPos);
      [[nodiscard]] Pixel const & getPixel(int xPos, int yPos) const;

      bool loadFromFile(std::string const & filePath) noexcept;
      [[nodiscard]] bool saveToFile(std::string const & filePath) const noexcept;
      void displayMetadata() const noexcept;
      void modifyMaxLevel(int newMaxColorValue) noexcept;

    private:
      bool readPixelData(std::ifstream & file);
      bool writePixelData(std::ofstream & file) const;

      std::vector<Pixel> pixels_;
  };
}  // namespace imageaos
