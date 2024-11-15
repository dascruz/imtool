#pragma once

#include <common/image.hpp>
#include <string>
#include <unordered_map>
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

      bool operator==(Pixel const & other) const {
        return red == other.red && green == other.green && blue == other.blue;
      }
  };

  class Image : image::Image {
    public:
      Pixel & getPixel(unsigned long xPos, unsigned long yPos);
      [[nodiscard]] Pixel const & getPixel(unsigned long xPos, unsigned long yPos) const;

      bool loadFromFile(std::string const & filePath);
      [[nodiscard]] bool saveToFile(std::string const & filePath) const;
      void displayMetadata() const;
      void modifyMaxLevel(unsigned short newMaxColorValue);
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

      std::vector<Pixel> pixels_;
  };
}  // namespace imageaos
