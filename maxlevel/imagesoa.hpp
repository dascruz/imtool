#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <array>
#include <cstdint>  // Para uint8_t y uint16_t


// Plantillas para leer y escribir datos binarios de cualquier tipo
template <typename T>
T read_binary(std::istream& input) {
  T value;
  std::array<char, sizeof(T)> buffer{};
  input.read(buffer.data(), buffer.size());
  std::memcpy(&value, buffer.data(), buffer.size());
  return value;
}


template <typename T>
void write_binary(std::ostream& output, const T& value) {
  std::array<char, sizeof(T)> buffer{};
  std::memcpy(buffer.data(), &value, buffer.size());
  output.write(buffer.data(), buffer.size());
}

class ImageSOA {
  public:
  ImageSOA(const std::string& filePath);
  void escalarIntensidad(int nuevoMaxColor);
  void redimensionar(int nuevoAncho, int nuevoAlto);          // Declaración de redimensionar
  void eliminarColoresMenosFrecuentes(int n);                 // Declaración de eliminarColoresMenosFrecuentes
  void comprimir();                                           // Declaración de comprimir
  void guardarImagen(const std::string& outputPath) const;

  private:
  int width{};
  int height{};
  int maxColor{};
  std::vector<std::uint16_t> redChannel;
  std::vector<std::uint16_t> greenChannel;
  std::vector<std::uint16_t> blueChannel;

  //void cargarDesdeArchivo(const std::string& filePath);
};

#endif // IMAGESOA_HPP

