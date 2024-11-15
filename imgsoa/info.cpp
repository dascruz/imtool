#include <common/image.hpp>
#include <fstream>
#include <imgsoa/imagesoa.hpp>
#include <iostream>

namespace imagesoa {
  void Image::displayMetadata() const {
    std::cout << "Image Metadata:\n"
              << "Width: " << getWidth() << "\n"
              << "Height: " << getHeight() << "\n"
              << "Max Color Value: " << getMaxColorValue() << "\n";
  }
}  // namespace imagesoa