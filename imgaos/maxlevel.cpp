#include <imgaos/imageaos.hpp>

namespace imageaos {
  void Image::modifyMaxLevel(unsigned short const newMaxColorValue) {
    // Calcular la proporción una vez
    float const scale =
        static_cast<float>(newMaxColorValue) / static_cast<float>(getMaxColorValue());

    for (unsigned long yPos = 0; yPos < getHeight(); ++yPos) {
      for (unsigned long xPos = 0; xPos < getWidth(); ++xPos) {
        Pixel & pixel = getPixel(xPos, yPos);

        // Escalar cada canal de color usando la proporción precomputada
        pixel.setRed(static_cast<unsigned short>(static_cast<float>(pixel.red) * scale));
        pixel.setGreen(static_cast<unsigned short>(static_cast<float>(pixel.green) * scale));
        pixel.setBlue(static_cast<unsigned short>(static_cast<float>(pixel.blue) * scale));
      }
    }

    // Actualizar el valor máximo de color
    setMaxColorValue(newMaxColorValue);
  }
}