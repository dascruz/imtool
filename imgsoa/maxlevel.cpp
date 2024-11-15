#include <common/image.hpp>
#include <imgsoa/imagesoa.hpp>

namespace imagesoa {
  void Image::modifyMaxLevel(unsigned short const newMaxColorValue) {
    // Calcular la proporción una vez
    float const scale =
        static_cast<float>(newMaxColorValue) / static_cast<float>(getMaxColorValue());

    for (unsigned long yPos = 0; yPos < getHeight(); ++yPos) {
      for (unsigned long xPos = 0; xPos < getWidth(); ++xPos) {
        unsigned long const index = (yPos * getWidth()) + xPos;

        // Aplicar la proporción precomputada
        red_[index]   = static_cast<unsigned short>(static_cast<float>(red_[index]) * scale);
        green_[index] = static_cast<unsigned short>(static_cast<float>(green_[index]) * scale);
        blue_[index]  = static_cast<unsigned short>(static_cast<float>(blue_[index]) * scale);
      }
    }

    // Actualizar el valor máximo de color
    setMaxColorValue(newMaxColorValue);
  }
}  // namespace imagesoa
