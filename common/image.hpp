#pragma once

#include <string>

namespace image {
  constexpr int MIN_COLOR_VALUE       = 1;
  constexpr int MAX_COLOR_VALUE_8BIT  = 255;
  constexpr int MAX_COLOR_VALUE_16BIT = 65535;

  class Image {
    public:
      bool readHeader(std::ifstream & file);
      bool writeHeader(std::ofstream & file) const;

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
