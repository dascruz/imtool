#include "metadata.hpp"
#include <iostream>
#include <fstream>
#include <string>

void mostrarMetadatos(const std::string& filePath) {
  std::ifstream file(filePath, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error al abrir el archivo " << filePath << '\n';
    return;
  }

  std::string magicNumber;
  file >> magicNumber;
  if (magicNumber != "P6") {
    std::cerr << "Formato PPM no válido." << '\n';
    return;
  }

  ImageMetadata metadata{};
  file >> metadata.width >> metadata.height >> metadata.maxColor;

  // Imprimir metadatos en el formato solicitado
  std::cout << "Input: " << filePath << '\n';
  std::cout << "Output: out/outs\n";
  std::cout << "Operation: info\n";
  std::cout << "Image size: " << metadata.width << "x" << metadata.height << '\n';
  std::cout << "Max level: " << metadata.maxColor << '\n';
}
