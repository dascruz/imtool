#pragma once

#include <common/image.hpp>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace imagesoa {
  constexpr unsigned char BYTE_SHIFT = 8;
  constexpr unsigned char BYTE_MASK  = 0xFF;

  struct Dimensions {
      unsigned long width;
      unsigned long height;
  };

  struct InterpolationCoords {
      std::uint32_t x_l, y_l, x_h, y_h;
      double x_weight, y_weight;
  };

  class Image : public image::Image {
    public:
      Image() = default;

      explicit Image(Dimensions const & dimensions) {
        setWidth(dimensions.width);
        setHeight(dimensions.height);
        red_.resize(getWidth() * getHeight());
        green_.resize(getWidth() * getHeight());
        blue_.resize(getWidth() * getHeight());
      }

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
      void resize(unsigned long new_width, unsigned long new_height);
      [[nodiscard]] unsigned short calculatePixelColor(InterpolationCoords const & coords,
                                                       char channel) const;
      [[nodiscard]] bool saveToFileCompress(std::string const & filePath) const;
      void cutfreq(uint32_t n);

    private:
      bool readPixelData(std::ifstream & file);
      bool writePixelData(std::ofstream & file) const;
      [[nodiscard]] std::unordered_map<image::Pixel, unsigned long> getColorTable() const;
      bool
          writeColorTable(std::ofstream & file,
                          std::unordered_map<image::Pixel, unsigned long> const & colorTable) const;
      bool writePixelDataCompress(
          std::ofstream & file,
          std::unordered_map<image::Pixel, unsigned long> const & colorTable) const;

      std::vector<unsigned short> red_;
      std::vector<unsigned short> green_;
      std::vector<unsigned short> blue_;

      [[nodiscard]] std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int>
          countColorFrequencies() const;
      static std::vector<std::pair<std::tuple<uint16_t, uint16_t, uint16_t>, int>>
          sortColorsByFrequency(
              std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int> const & colorFrequency);
      static std::pair<std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>,
                       std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>>
          splitColors(std::vector<std::pair<std::tuple<uint16_t, uint16_t, uint16_t>, int>> const &
                          colorFreqVector,
                      uint32_t n);
      static std::tuple<uint16_t, uint16_t, uint16_t> findClosestColor(
          std::tuple<uint16_t, uint16_t, uint16_t> const & colorToRemove,
          std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> const & remainingColors);
      static std::map<std::tuple<uint16_t, uint16_t, uint16_t>,
                      std::tuple<uint16_t, uint16_t, uint16_t>>
          buildReplacementMap(
              std::pair<std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>,
                        std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>> const & colors);
      void replaceColors(std::map<std::tuple<uint16_t, uint16_t, uint16_t>,
                                  std::tuple<uint16_t, uint16_t, uint16_t>> const & replacementMap);
  };

  inline unsigned short Image::getRed(unsigned long const xPos, unsigned long const yPos) const {
    return red_[(yPos * getWidth()) + xPos];
  }

  inline unsigned short Image::getGreen(unsigned long const xPos, unsigned long const yPos) const {
    return green_[(yPos * getWidth()) + xPos];
  }

  inline unsigned short Image::getBlue(unsigned long const xPos, unsigned long const yPos) const {
    return blue_[(yPos * getWidth()) + xPos];
  }

  inline void Image::setRed(unsigned long const xPos, unsigned long const yPos,
                            unsigned short const redValue) {
    red_[(yPos * getWidth()) + xPos] = redValue;
  }

  inline void Image::setGreen(unsigned long const xPos, unsigned long const yPos,
                              unsigned short const greenValue) {
    green_[(yPos * getWidth()) + xPos] = greenValue;
  }

  inline void Image::setBlue(unsigned long const xPos, unsigned long const yPos,
                             unsigned short const blueValue) {
    blue_[(yPos * getWidth()) + xPos] = blueValue;
  }
}  // namespace imagesoa
