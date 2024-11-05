#include <algorithm>  // Para std::clamp
#include <cmath>
#include <cstddef>  // Para size_t
#include <cstdint>  // Para uint8_t y uint16_t
#include <fstream>
#include <iostream>
#include <string>  // Para std::string
#include <vector>

#include "imagesoa.hpp"

namespace {
  constexpr int MAX_COLOR_THRESHOLD = 256;
}


ImageSOA::ImageSOA(const std::string& filePath) {
  std::ifstream file(filePath, std::ios::binary);
  if (!file) {
    std::cerr << "Error: No se pudo abrir el archivo " << filePath << '\n';
    return;
  }

  // Leer el encabezado de la imagen PPM
  std::string format;
  file >> format;
  file >> width >> height >> maxColor;
  file.ignore();  // Ignorar el carácter de nueva línea después del encabezado

  redChannel.resize(static_cast<size_t>(width) * static_cast<size_t>(height));
  greenChannel.resize(static_cast<size_t>(width) * static_cast<size_t>(height));
  blueChannel.resize(static_cast<size_t>(width) * static_cast<size_t>(height));

  // Leer los valores de color en los tres canales
  for (size_t i = 0; i < redChannel.size(); ++i) {
    redChannel[i] = read_binary<std::uint8_t>(file);
    greenChannel[i] = read_binary<std::uint8_t>(file);
    blueChannel[i] = read_binary<std::uint8_t>(file);
  }
    file.close();
}

// Función para guardar la imagen en formato PPM
void ImageSOA::guardarImagen(const std::string& outputPath) const {
  std::ofstream file(outputPath, std::ios::binary);
  if (!file) {
    std::cerr << "Error: No se pudo guardar el archivo en " << outputPath << '\n';
    return;
  }

  // Escribir el encabezado PPM
  file << "P6\n" << width << " " << height << "\n" << maxColor << "\n";

  // Escribir los valores de color en los tres canales
  for (size_t i = 0; i < redChannel.size(); ++i) {
    if (maxColor < MAX_COLOR_THRESHOLD) {
      // Representación de 1 byte por canal (3 bytes por píxel)
      file.put(static_cast<char>(redChannel[i]));
      file.put(static_cast<char>(greenChannel[i]));
      file.put(static_cast<char>(blueChannel[i]));
    } else {
      // Representación de 2 bytes por canal (6 bytes por píxel)
      write_binary<std::uint16_t>(file, redChannel[i]);
      write_binary<std::uint16_t>(file, greenChannel[i]);
      write_binary<std::uint16_t>(file, blueChannel[i]);
    }
  }

    file.close();
}

// Función para escalar la intensidad de la imagen
void ImageSOA::escalarIntensidad(int nuevoMaxColor) {

  if (maxColor == nuevoMaxColor) {
    std::cerr << "El valor de maxColor es el mismo, no se realiza ningún cambio.\n";
    return;
  }
  // Escalar cada componente de color en cada canal (rojo, verde, azul)
  for (size_t i = 0; i < redChannel.size(); ++i) {
    int const redScaled = static_cast<int>(std::round(redChannel[i] * static_cast<double>(nuevoMaxColor) / maxColor));
    int const greenScaled = static_cast<int>(std::round(greenChannel[i] * static_cast<double>(nuevoMaxColor) / maxColor));
    int const blueScaled = static_cast<int>(std::round(blueChannel[i] * static_cast<double>(nuevoMaxColor) / maxColor));

    // Asignar el valor escalado a los canales directamente
    redChannel[i] = static_cast<std::uint16_t>(std::clamp(redScaled, 0, nuevoMaxColor));
    greenChannel[i] = static_cast<std::uint16_t>(std::clamp(greenScaled, 0, nuevoMaxColor));
    blueChannel[i] = static_cast<std::uint16_t>(std::clamp(blueScaled, 0, nuevoMaxColor));
  }

  // Actualizar el valor máximo de color
  maxColor = nuevoMaxColor;
}

void ImageSOA::redimensionar(int, int) {
  // Implementación futura
}

void ImageSOA::eliminarColoresMenosFrecuentes(int) {
  // Implementación futura
}

void ImageSOA::comprimir() {
  // Implementación futura
}


