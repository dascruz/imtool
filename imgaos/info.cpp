#include <imgaos/imageaos.hpp>
#include <iostream>

namespace imageaos {
  void Image::displayMetadata() const {
    std::cout << "Image Metadata:\n"
              << "Width: " << getWidth() << "\n"
              << "Height: " << getHeight() << "\n"
              << "Max Color Value: " << getMaxColorValue() << "\n";
  }
}