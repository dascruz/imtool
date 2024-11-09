#pragma once

#include <common/image.hpp>
#include <string>
#include <vector>

namespace imagesoa {
  constexpr unsigned char BYTE_SHIFT = 8;
  constexpr unsigned char BYTE_MASK  = 0xFF;

  class Image : public image::Image {
    public:
      [[nodiscard]] unsigned short getRed(unsigned long xPos, unsigned long yPos) const;
      [[nodiscard]] unsigned short getGreen(unsigned long xPos, unsigned long yPos) const;
      [[nodiscard]] unsigned short getBlue(unsigned long xPos, unsigned long yPos) const;

      void setRed(unsigned long xPos, unsigned long yPos, unsigned short redValue);
      void setGreen(unsigned long xPos, unsigned long yPos, unsigned short greenValue);
      void setBlue(unsigned long xPos, unsigned long yPos, unsigned short blueValue);

      bool loadFromFile(std::string const & filePath);
      [[nodiscard]] bool saveToFile(std::string const & filePath) const;
      void displayMetadata() const;
      void modifyMaxLevel(unsigned short newMaxColorValue);

    private:
      bool readPixelData(std::ifstream & file);
      bool writePixelData(std::ofstream & file) const;

      std::vector<unsigned short> red_;
      std::vector<unsigned short> green_;
      std::vector<unsigned short> blue_;
  };

  inline unsigned short Image::getRed(unsigned long xPos, unsigned long yPos) const {
    return red_[(yPos * getWidth()) + xPos];
  }

  inline unsigned short Image::getGreen(unsigned long xPos, unsigned long yPos) const {
    return green_[(yPos * getWidth()) + xPos];
  }

  inline unsigned short Image::getBlue(unsigned long xPos, unsigned long yPos) const {
    return blue_[(yPos * getWidth()) + xPos];
  }

  inline void Image::setRed(unsigned long xPos, unsigned long yPos, unsigned short redValue) {
    red_[(yPos * getWidth()) + xPos] = redValue;
  }

  inline void Image::setGreen(unsigned long xPos, unsigned long yPos, unsigned short greenValue) {
    green_[(yPos * getWidth()) + xPos] = greenValue;
  }

  inline void Image::setBlue(unsigned long xPos, unsigned long yPos, unsigned short blueValue) {
    blue_[(yPos * getWidth()) + xPos] = blueValue;
  }

}  // namespace imagesoa
