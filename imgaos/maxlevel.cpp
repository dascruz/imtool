#include <imgaos/imageaos.hpp>

namespace imageaos {
  void Image::modifyMaxLevel(unsigned short const newMaxColorValue) {
    for (unsigned long yPos = 0; yPos < getHeight(); ++yPos) {
      for (unsigned long xPos = 0; xPos < getWidth(); ++xPos) {
        Pixel & pixel = getPixel(xPos, yPos);

        pixel.setRed(
            static_cast<unsigned short>(pixel.red * newMaxColorValue / getMaxColorValue()));
        pixel.setGreen(
            static_cast<unsigned short>(pixel.green * newMaxColorValue / getMaxColorValue()));
        pixel.setBlue(
            static_cast<unsigned short>(pixel.blue * newMaxColorValue / getMaxColorValue()));
      }
    }

    setMaxColorValue(newMaxColorValue);
  }
}