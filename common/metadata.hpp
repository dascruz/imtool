#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>

// Estructura para almacenar los datos de la imagen
struct ImageMetadata {
  int width;
  int height;
  int maxColor;
};

// Función para leer y presentar los metadatos de una imagen en la salida estándar
void mostrarMetadatos(const std::string& filePath);

#endif // METADATA_HPP
