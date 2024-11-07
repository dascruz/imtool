#pragma once

#include <string>
#include <vector>

namespace image {
  constexpr int MIN_COLOR_VALUE       = 1;
  constexpr int MAX_COLOR_VALUE_8BIT  = 255;
  constexpr int MAX_COLOR_VALUE_16BIT = 65535;

  struct Dimensions {
      int width  = 0;
      int height = 0;
  };

  struct MaxColorValue {
      int value = MAX_COLOR_VALUE_8BIT;
  };

  class Image {
    public:
      Image() : width_(0), height_(0), maxColorValue_(MAX_COLOR_VALUE_8BIT) { }

      explicit Image(Dimensions dims, MaxColorValue maxColorValue = {MAX_COLOR_VALUE_8BIT});

      bool readHeader(std::ifstream & file);
      bool writeHeader(std::ofstream & file) const;

    protected:
      [[nodiscard]] int getWidth() const { return width_; }

      [[nodiscard]] int getHeight() const { return height_; }

      [[nodiscard]] int getMaxColorValue() const { return maxColorValue_; }

      void setWidth(int const width) { width_ = width; }

      void setHeight(int const height) { height_ = height; }

      void setMaxColorValue(int const maxColorValue) { maxColorValue_ = maxColorValue; }

    private:
      int width_;
      int height_;
      int maxColorValue_;
  };
}  // namespace image
