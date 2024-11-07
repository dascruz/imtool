#include <fstream>
#include <imgaos/imageaos.hpp>
#include <iostream>
#include <vector>

namespace imageaos {
  bool Image::readPixelData(std::ifstream & file) {
    pixels_.resize(static_cast<std::vector<Pixel>::size_type>(getWidth()) *
                   static_cast<std::vector<Pixel>::size_type>(getHeight()));

    int const scaleFactor = (getMaxColorValue() > MAX_COLOR_VALUE_8BIT) ? 2 : 1;

    for (int yPos = 0; yPos < getHeight(); ++yPos) {
      for (int xPos = 0; xPos < getWidth(); ++xPos) {
        Pixel & pixel = getPixel(xPos, yPos);

        int const red   = (scaleFactor == 2) ? (file.get() << 8 | file.get()) : file.get();
        int const green = (scaleFactor == 2) ? (file.get() << 8 | file.get()) : file.get();
        int const blue  = (scaleFactor == 2) ? (file.get() << 8 | file.get()) : file.get();

        if (file.eof()) {
          std::cerr << "Unexpected end of file while reading pixel data.\n";
          return false;
        }

        pixel.setRed(static_cast<unsigned char>(red * MAX_COLOR_VALUE_8BIT / getMaxColorValue()));
        pixel.setGreen(
            static_cast<unsigned char>(green * MAX_COLOR_VALUE_8BIT / getMaxColorValue()));
        pixel.setBlue(static_cast<unsigned char>(blue * MAX_COLOR_VALUE_8BIT / getMaxColorValue()));
      }
    }

    return true;
  }

  bool Image::loadFromFile(std::string const & filePath) noexcept {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "Failed to open file: " << filePath << '\n';
      return false;
    }

    bool const headerRead = readHeader(file);
    if (!headerRead) {
      file.close();
      return false;
    }

    bool const pixelDataRead = readPixelData(file);
    file.close();

    return pixelDataRead;
  }

  bool Image::writePixelData(std::ofstream & file) const {
    int const scaleFactor = (getMaxColorValue() > MAX_COLOR_VALUE_8BIT) ? 2 : 1;

    for (int yPos = 0; yPos < getHeight(); ++yPos) {
      for (int xPos = 0; xPos < getWidth(); ++xPos) {
        auto const & [red, green, blue] = getPixel(xPos, yPos);

        int const scaledRed   = red * getMaxColorValue() / MAX_COLOR_VALUE_8BIT;
        int const scaledGreen = green * getMaxColorValue() / MAX_COLOR_VALUE_8BIT;
        int const scaledBlue  = blue * getMaxColorValue() / MAX_COLOR_VALUE_8BIT;

        unsigned char const uRed =
            static_cast<unsigned char>(std::min(std::max(scaledRed, 0), 255));
        unsigned char const uGreen =
            static_cast<unsigned char>(std::min(std::max(scaledGreen, 0), 255));
        unsigned char const uBlue =
            static_cast<unsigned char>(std::min(std::max(scaledBlue, 0), 255));

        if (scaleFactor == 2) {
          file.put(static_cast<char>(uRed >> BIT_SHIFT));
          file.put(static_cast<char>(uRed & BYTE_MASK));
          file.put(static_cast<char>(uGreen >> BIT_SHIFT));
          file.put(static_cast<char>(uGreen & BYTE_MASK));
          file.put(static_cast<char>(uBlue >> BIT_SHIFT));
          file.put(static_cast<char>(uBlue & BYTE_MASK));
        } else {
          file.put(static_cast<char>(uRed));
          file.put(static_cast<char>(uGreen));
          file.put(static_cast<char>(uBlue));
        }
      }
    }

    return file.good();
  }

  bool Image::saveToFile(std::string const & filePath) const noexcept {
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "Failed to open file: " << filePath << '\n';
      return false;
    }

    bool const headerWritten = writeHeader(file);
    if (!headerWritten) {
      file.close();
      return false;
    }

    bool const pixelDataWritten = writePixelData(file);
    file.close();

    return pixelDataWritten;
  }

  void Image::displayMetadata() const noexcept {
    std::cout << "Image Metadata:\n"
              << "Width: " << getWidth() << "\n"
              << "Height: " << getHeight() << "\n"
              << "Max Color Value: " << getMaxColorValue() << "\n";
  }

  void Image::modifyMaxLevel(int const newMaxColorValue) noexcept {
    if (newMaxColorValue < MIN_COLOR_VALUE || newMaxColorValue > MAX_COLOR_VALUE_16BIT) {
      std::cerr << "Invalid max color value: " << newMaxColorValue << ". Must be between "
                << MIN_COLOR_VALUE << " and " << MAX_COLOR_VALUE_16BIT << ".\n";
      return;
    }

    if (getMaxColorValue() <= MAX_COLOR_VALUE_8BIT && newMaxColorValue > MAX_COLOR_VALUE_8BIT) {
      for (int yPos = 0; yPos < getHeight(); ++yPos) {
        for (int xPos = 0; xPos < getWidth(); ++xPos) {
          Pixel & pixel = getPixel(xPos, yPos);

          pixel.setRed(
              static_cast<unsigned char>(pixel.red * newMaxColorValue / MAX_COLOR_VALUE_8BIT));
          pixel.setGreen(
              static_cast<unsigned char>(pixel.green * newMaxColorValue / MAX_COLOR_VALUE_8BIT));
          pixel.setBlue(
              static_cast<unsigned char>(pixel.blue * newMaxColorValue / MAX_COLOR_VALUE_8BIT));
        }
      }
    }

    else if (getMaxColorValue() > MAX_COLOR_VALUE_8BIT &&
             newMaxColorValue <= MAX_COLOR_VALUE_8BIT) {
      for (int yPos = 0; yPos < getHeight(); ++yPos) {
        for (int xPos = 0; xPos < getWidth(); ++xPos) {
          Pixel & pixel = getPixel(xPos, yPos);

          pixel.setRed(
              static_cast<unsigned char>(pixel.red * newMaxColorValue / getMaxColorValue()));
          pixel.setGreen(
              static_cast<unsigned char>(pixel.green * newMaxColorValue / getMaxColorValue()));
          pixel.setBlue(
              static_cast<unsigned char>(pixel.blue * newMaxColorValue / getMaxColorValue()));
        }
      }
    }

    setMaxColorValue(newMaxColorValue);
  }

  Pixel & Image::getPixel(int const xPos, int const yPos) {
    return pixels_.at((static_cast<std::vector<Pixel>::size_type>(yPos) *
                       static_cast<std::vector<Pixel>::size_type>(getWidth())) +
                      static_cast<std::vector<Pixel>::size_type>(xPos));
  }

  Pixel const & Image::getPixel(int const xPos, int const yPos) const {
    return pixels_.at((static_cast<std::vector<Pixel>::size_type>(yPos) *
                       static_cast<std::vector<Pixel>::size_type>(getWidth())) +
                      static_cast<std::vector<Pixel>::size_type>(xPos));
  }
}  // namespace imageaos
