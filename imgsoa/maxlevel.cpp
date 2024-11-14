#include <imgsoa/imagesoa.hpp>


namespace imagesoa {
  void Image::modifyMaxLevel(unsigned short const newMaxColorValue) {
    for (unsigned long yPos = 0; yPos < getHeight(); ++yPos) {
      for (unsigned long xPos = 0; xPos < getWidth(); ++xPos) {
        unsigned long const index = (yPos * getWidth()) + xPos;

        red_[index] = static_cast<unsigned short>(red_[index] * newMaxColorValue / getMaxColorValue());
        green_[index] = static_cast<unsigned short>(green_[index] * newMaxColorValue / getMaxColorValue());
        blue_[index] = static_cast<unsigned short>(blue_[index] * newMaxColorValue / getMaxColorValue());
      }
    }

    setMaxColorValue(newMaxColorValue);
  }
}