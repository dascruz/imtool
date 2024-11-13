#include <algorithm>
#include <fstream>
#include <imgsoa/imagesoa.hpp>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <vector>

namespace imagesoa {
  namespace {
    constexpr unsigned short COLOR_TABLE_SIZE_8  = 8;
    constexpr unsigned short COLOR_TABLE_SIZE_16 = 16;
    constexpr unsigned short COLOR_TABLE_SIZE_32 = 32;

    unsigned short getPixelByteSize(unsigned long const colorTableSize) {
      if (colorTableSize <= (1UL << COLOR_TABLE_SIZE_8)) { return 1; }
      if (colorTableSize <= (1UL << COLOR_TABLE_SIZE_16)) { return 2; }
      if (colorTableSize <= (1UL << COLOR_TABLE_SIZE_32)) { return 4; }
      return 0;
    }
  }  // namespace

  bool Image::saveToFileCompress(std::string const & filePath) const {
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "Failed to open file: " << filePath << '\n';
      return false;
    }

    auto const colorTable = getColorTable();
    if (!writeHeaderCompress(file, colorTable.size())) {
      file.close();
      return false;
    }

    if (!writeColorTable(file, colorTable)) {
      file.close();
      return false;
    }

    if (!writePixelDataCompress(file, colorTable)) {
      file.close();
      return false;
    }

    file.close();
    return true;
  }

  std::unordered_map<image::Pixel, unsigned long> Image::getColorTable() const {
    constexpr unsigned long maxColors = 1UL << 32;
    std::unordered_map<image::Pixel, unsigned long> colorTable;
    colorTable.reserve(red_.size());

    unsigned long index = 0;
    for (std::size_t i = 0; i < red_.size(); ++i) {
      image::Pixel const pixel{.red = red_[i], .green = green_[i], .blue = blue_[i]};
      if (auto const [fst, inserted] = colorTable.insert({pixel, index}); inserted) { ++index; }
    }

    if (colorTable.size() > maxColors) {
      throw std::overflow_error("Color table exceeds 2^32 unique colors.");
    }

    return colorTable;
  }

  bool Image::writeColorTable(
      std::ofstream & file,
      std::unordered_map<image::Pixel, unsigned long> const & colorTable) const {
    unsigned char const scaleFactor = getMaxColorValue() > image::MAX_COLOR_VALUE_8BIT ? 2 : 1;
    std::vector<std::pair<image::Pixel, unsigned long>> sortedColorTable(colorTable.begin(),
                                                                         colorTable.end());
    std::ranges::sort(sortedColorTable, [](auto const & lhs, auto const & rhs) {
      return lhs.second < rhs.second;
    });
    std::vector<char> buffer;
    buffer.reserve(sortedColorTable.size() * 3 * scaleFactor);

    if (scaleFactor == 2) {
      for (auto const & [red, green, blue] : sortedColorTable | std::views::keys) {
        buffer.push_back(static_cast<char>(red >> BYTE_SHIFT));
        buffer.push_back(static_cast<char>(red & BYTE_MASK));
        buffer.push_back(static_cast<char>(green >> BYTE_SHIFT));
        buffer.push_back(static_cast<char>(green & BYTE_MASK));
        buffer.push_back(static_cast<char>(blue >> BYTE_SHIFT));
        buffer.push_back(static_cast<char>(blue & BYTE_MASK));
      }
    } else {
      for (auto const & [red, green, blue] : sortedColorTable | std::views::keys) {
        buffer.push_back(static_cast<char>(red));
        buffer.push_back(static_cast<char>(green));
        buffer.push_back(static_cast<char>(blue));
      }
    }

    file.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    return file.good();
  }

  bool Image::writePixelDataCompress(
      std::ofstream & file,
      std::unordered_map<image::Pixel, unsigned long> const & colorTable) const {
    unsigned short const byteSize = getPixelByteSize(colorTable.size());

    std::vector<char> buffer;
    buffer.reserve(getHeight() * getWidth() * byteSize);

    for (unsigned long yPos = 0; yPos < getHeight(); ++yPos) {
      for (unsigned long xPos = 0; xPos < getWidth(); ++xPos) {
        std::size_t const index = (yPos * getWidth()) + xPos;
        image::Pixel const pixel{.red = red_[index], .green = green_[index], .blue = blue_[index]};
        auto const colorIndex = colorTable.at(pixel);

        if (byteSize == 1) {
          buffer.push_back(static_cast<char>(colorIndex & BYTE_MASK));
        } else if (byteSize == 2) {
          buffer.push_back(static_cast<char>(colorIndex & BYTE_MASK));
          buffer.push_back(static_cast<char>(colorIndex >> COLOR_TABLE_SIZE_8 & BYTE_MASK));
        } else if (byteSize == 4) {
          buffer.push_back(static_cast<char>(colorIndex & BYTE_MASK));
          buffer.push_back(static_cast<char>(colorIndex >> COLOR_TABLE_SIZE_8 & BYTE_MASK));
          buffer.push_back(static_cast<char>(colorIndex >> COLOR_TABLE_SIZE_16 & BYTE_MASK));
          buffer.push_back(static_cast<char>(colorIndex >> COLOR_TABLE_SIZE_32 & BYTE_MASK));
        }
      }
    }

    file.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    return file.good();
  }
}  // namespace imagesoa