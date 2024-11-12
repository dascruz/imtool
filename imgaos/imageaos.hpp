#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <common/image.hpp>

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

      bool operator==(Pixel const & other) const {
        return red == other.red && green == other.green && blue == other.blue;
      }
  };

  struct Dimensions {
      unsigned long width;
      unsigned long height;
  };

  struct DimensionsResize {
      double x = 0;
      double y = 0;
  };

  struct InterpolateArgs {
      float x;
      float y;
  };

  class Image : public image::Image {
    public:
      static constexpr unsigned short DEFAULT_MAX_COLOR_VALUE = 255;

      // Constructor predeterminado sin inicialización de maxColorValue_
      Image() = default;

      // Constructor que configura dimensiones y maxColorValue
      explicit Image(Dimensions const dimensions,
                     unsigned short const maxColorValue = DEFAULT_MAX_COLOR_VALUE) {
          setWidth(dimensions.width);
          setHeight(dimensions.height);
          setMaxColorValue(maxColorValue);
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
      void setPixel(unsigned long xPos, unsigned long yPos, Pixel const & pixel);

      [[nodiscard]] unsigned short getMaxColorValue() const { return image::Image::getMaxColorValue(); }
      void setMaxColorValue(unsigned short const value) { image::Image::setMaxColorValue(value); }

      void resize(unsigned long new_width, unsigned long new_height);
      [[nodiscard]] Pixel interpolate(DimensionsResize dims) const;
      [[nodiscard]] Pixel interpolate2(InterpolateArgs const & interpolate_args) const;
      [[nodiscard]] bool saveToFileCompress(std::string const & filePath) const;

    private:
      bool readPixelData(std::ifstream & file);
      bool writePixelData(std::ofstream & file) const;
      [[nodiscard]] std::unordered_map<Pixel, unsigned long> getColorTable() const;
      bool writeColorTable(std::ofstream & file,
                           std::unordered_map<Pixel, unsigned long> const & colorTable) const;
      bool
          writePixelDataCompress(std::ofstream & file,
                                 std::unordered_map<Pixel, unsigned long> const & colorTable) const;

      unsigned short maxColorValue_{};
      std::vector<Pixel> pixels_;
  };
}  // namespace imageaos


