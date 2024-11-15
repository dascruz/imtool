#include <fstream>
#include <imgaos/imageaos.hpp>
#include <iostream>
#include <string>

namespace imageaos {
  bool Image::readPixelData(std::ifstream & file) {
    pixels_.resize(getWidth() * getHeight());

    unsigned char const scaleFactor = getMaxColorValue() > image::MAX_COLOR_VALUE_8BIT ? 2 : 1;

    for (unsigned long yPos = 0; yPos < getHeight(); ++yPos) {
      for (unsigned long xPos = 0; xPos < getWidth(); ++xPos) {
        Pixel & pixel = getPixel(xPos, yPos);

        unsigned short const red =
            scaleFactor == 2 ? static_cast<unsigned short>(file.get() << BYTE_SHIFT | file.get())
                             : static_cast<unsigned short>(file.get());
        unsigned short const green =
            scaleFactor == 2 ? static_cast<unsigned short>(file.get() << BYTE_SHIFT | file.get())
                             : static_cast<unsigned short>(file.get());
        unsigned short const blue =
            scaleFactor == 2 ? static_cast<unsigned short>(file.get() << BYTE_SHIFT | file.get())
                             : static_cast<unsigned short>(file.get());

        if (file.eof()) {
          std::cerr << "Unexpected end of file while reading pixel data.\n";
          return false;
        }

        pixel.setRed(red);
        pixel.setGreen(green);
        pixel.setBlue(blue);
      }
    }

    return true;
  }

  bool Image::loadFromFile(std::string const & filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "Failed to open file: " << filePath << '\n';
      return false;
    }

    if (bool const headerRead = readHeader(file); !headerRead) {
      file.close();
      return false;
    }

    bool const pixelDataRead = readPixelData(file);
    file.close();

    return pixelDataRead;
  }

  bool Image::writePixelData(std::ofstream & file) const {
    unsigned char const scaleFactor = getMaxColorValue() > image::MAX_COLOR_VALUE_8BIT ? 2 : 1;

    for (unsigned long yPos = 0; yPos < getHeight(); ++yPos) {
      for (unsigned long xPos = 0; xPos < getWidth(); ++xPos) {
        auto const & [red, green, blue] = getPixel(xPos, yPos);

        if (scaleFactor == 2) {
          file.put(static_cast<char>(red >> BYTE_SHIFT));
          file.put(static_cast<char>(red & BYTE_MASK));
          file.put(static_cast<char>(green >> BYTE_SHIFT));
          file.put(static_cast<char>(green & BYTE_MASK));
          file.put(static_cast<char>(blue >> BYTE_SHIFT));
          file.put(static_cast<char>(blue & BYTE_MASK));
        } else {
          file.put(static_cast<char>(red));
          file.put(static_cast<char>(green));
          file.put(static_cast<char>(blue));
        }
      }
    }

    return file.good();
  }

  bool Image::saveToFile(std::string const & filePath) const {
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "Failed to open file: " << filePath << '\n';
      return false;
    }

    if (bool const headerWritten = writeHeader(file); !headerWritten) {
      file.close();
      return false;
    }

    bool const pixelDataWritten = writePixelData(file);
    file.close();

    return pixelDataWritten;
  }

  Pixel & Image::getPixel(unsigned long const xPos, unsigned long const yPos) {
    return pixels_.at((yPos * getWidth()) + xPos);
  }

  Pixel const & Image::getPixel(unsigned long const xPos, unsigned long const yPos) const {
    return pixels_.at((yPos * getWidth()) + xPos);
  }

  void Image::setPixel(unsigned long xPos, unsigned long yPos, Pixel const & pixel) {
    pixels_.at((yPos * getWidth()) + xPos) = pixel;
  }

}  // namespace imageaos
