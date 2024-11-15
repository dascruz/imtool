#include <common/image.hpp>
#include <fstream>
#include <imgsoa/imagesoa.hpp>
#include <iostream>

namespace imagesoa {
  bool Image::readPixelData(std::ifstream & file) {
    red_.resize(getWidth() * getHeight());
    green_.resize(getWidth() * getHeight());
    blue_.resize(getWidth() * getHeight());

    unsigned char const scaleFactor = getMaxColorValue() > image::MAX_COLOR_VALUE_8BIT ? 2 : 1;

    for (unsigned long yPos = 0; yPos < getHeight(); ++yPos) {
      for (unsigned long xPos = 0; xPos < getWidth(); ++xPos) {
        unsigned long const index = (yPos * getWidth()) + xPos;

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

        red_[index]   = red;
        green_[index] = green;
        blue_[index]  = blue;
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
        unsigned long const index = (yPos * getWidth()) + xPos;

        if (scaleFactor == 2) {
          file.put(static_cast<char>(red_[index] >> BYTE_SHIFT));
          file.put(static_cast<char>(red_[index] & BYTE_MASK));
          file.put(static_cast<char>(green_[index] >> BYTE_SHIFT));
          file.put(static_cast<char>(green_[index] & BYTE_MASK));
          file.put(static_cast<char>(blue_[index] >> BYTE_SHIFT));
          file.put(static_cast<char>(blue_[index] & BYTE_MASK));
        } else {
          file.put(static_cast<char>(red_[index]));
          file.put(static_cast<char>(green_[index]));
          file.put(static_cast<char>(blue_[index]));
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

  void Image::modifyMaxLevel(unsigned short const newMaxColorValue) {
    for (unsigned long yPos = 0; yPos < getHeight(); ++yPos) {
      for (unsigned long xPos = 0; xPos < getWidth(); ++xPos) {
        unsigned long const index = (yPos * getWidth()) + xPos;

        red_[index] =
            static_cast<unsigned short>(red_[index] * newMaxColorValue / getMaxColorValue());
        green_[index] =
            static_cast<unsigned short>(green_[index] * newMaxColorValue / getMaxColorValue());
        blue_[index] =
            static_cast<unsigned short>(blue_[index] * newMaxColorValue / getMaxColorValue());
      }
    }

    setMaxColorValue(newMaxColorValue);
  }

  void Image::displayMetadata() const {
    std::cout << "Image Metadata:\n"
              << "Width: " << getWidth() << "\n"
              << "Height: " << getHeight() << "\n"
              << "Max Color Value: " << getMaxColorValue() << "\n";
  }

}  // namespace imagesoa
