#pragma once

#include <string>

namespace image {
  constexpr int MIN_COLOR_VALUE       = 1;
  constexpr int MAX_COLOR_VALUE_8BIT  = 255;
  constexpr int MAX_COLOR_VALUE_16BIT = 65535;

  struct Pixel {
      unsigned short red   = 0;
      unsigned short green = 0;
      unsigned short blue  = 0;

      bool operator==(Pixel const & other) const {
        return red == other.red && green == other.green && blue == other.blue;
      }
  };

  class Image {
    public:
      bool readHeader(std::ifstream & file);
      bool writeHeader(std::ofstream & file) const;
      bool writeHeaderCompress(std::ofstream & file, unsigned long colorTableSize) const;

    protected:
      [[nodiscard]] unsigned long getWidth() const { return width_; }

      [[nodiscard]] unsigned long getHeight() const { return height_; }

      [[nodiscard]] unsigned short getMaxColorValue() const { return maxColorValue_; }

      void setWidth(unsigned long const width) { width_ = width; }

      void setHeight(unsigned long const height) { height_ = height; }

      void setMaxColorValue(unsigned short const maxColorValue) { maxColorValue_ = maxColorValue; }

    private:
      unsigned long width_          = 0;
      unsigned long height_         = 0;
      unsigned short maxColorValue_ = 0;
  };
}  // namespace image

template <>
struct std::hash<image::Pixel> {
    std::size_t operator()(image::Pixel const & pixel) const noexcept {
      std::size_t const hash1 = std::hash<unsigned short>{}(pixel.red);
      std::size_t const hash2 = std::hash<unsigned short>{}(pixel.green);
      std::size_t const hash3 = std::hash<unsigned short>{}(pixel.blue);

      return hash1 ^ (hash2 << 1) ^ (hash3 << 3);
    }
};
