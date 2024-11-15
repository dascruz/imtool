#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <imgaos/imageaos.hpp>
#include <vector>

// Función auxiliar para comparar píxeles con tolerancia
namespace {
  bool comparePixelsWithTolerance(imageaos::Pixel const & pos1, imageaos::Pixel const & pos2,
                                  int const tolerance) {
    return (std::abs(static_cast<int>(pos1.red) - static_cast<int>(pos2.red)) <= tolerance) &&
           (std::abs(static_cast<int>(pos1.green) - static_cast<int>(pos2.green)) <= tolerance) &&
           (std::abs(static_cast<int>(pos1.blue) - static_cast<int>(pos2.blue)) <= tolerance);
  }

  bool loadImageDimensions(std::string const & filePath, unsigned long & width,
                           unsigned long & height) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "No se pudo abrir el archivo: " << filePath << '\n';
      return false;
    }

    std::string format;
    file >> format;
    if (format != "P6") {
      std::cerr << "Formato no soportado: " << format << '\n';
      return false;
    }

    file >> width >> height;
    file.ignore(std::numeric_limits<std::streamsize>::max(),
                '\n');  // Ignorar hasta el final de la línea
    return true;
  }
}  // namespace

// Test de la función resize con múltiples archivos de prueba
TEST(ImageResizeTest, ResizeMultipleFiles) {
  struct TestFile {
      std::string inputPath;
      std::string expectedOutputPath;
  };

  // Lista de archivos de entrada y sus correspondientes salidas esperadas
  std::vector<TestFile> const testFiles = {
    {.inputPath          = "./in/deer-large.ppm",
     .expectedOutputPath = "./expected_outputs/deer-large-100.ppm" },
    {.inputPath          = "./in/deer-large.ppm",
     .expectedOutputPath = "./expected_outputs/deer-large-1000.ppm"},
    {.inputPath          = "./in/deer-small.ppm",
     .expectedOutputPath = "./expected_outputs/deer-small-100.ppm" },
    {.inputPath          = "./in/deer-small.ppm",
     .expectedOutputPath = "./expected_outputs/deer-small-1000.ppm"},
    {.inputPath          = "./in/lake-large.ppm",
     .expectedOutputPath = "./expected_outputs/lake-large-100.ppm" },
    {.inputPath          = "./in/lake-large.ppm",
     .expectedOutputPath = "./expected_outputs/lake-large-1000.ppm"},
    {.inputPath          = "./in/lake-small.ppm",
     .expectedOutputPath = "./expected_outputs/lake-small-100.ppm" },
    {.inputPath          = "./in/lake-small.ppm",
     .expectedOutputPath = "./expected_outputs/lake-small-1000.ppm"}
  };

  // Recorrer cada archivo de prueba
  for (auto const & file : testFiles) {
    imageaos::Image inputImage;
    imageaos::Image expectedImage;

    // Cargar la imagen de entrada y la esperada
    ASSERT_TRUE(inputImage.loadFromFile(file.inputPath))
        << "No se pudo cargar la imagen de entrada: " << file.inputPath;
    ASSERT_TRUE(expectedImage.loadFromFile(file.expectedOutputPath))
        << "No se pudo cargar la imagen esperada: " << file.expectedOutputPath;

    // Obtener las dimensiones de la imagen esperada
    unsigned long newWidth  = 0;
    unsigned long newHeight = 0;
    ASSERT_TRUE(loadImageDimensions(file.expectedOutputPath, newWidth, newHeight))
        << "No se pudieron cargar las dimensiones de la imagen esperada: "
        << file.expectedOutputPath;

    // Aplicar la función resize a la imagen de entrada
    inputImage.resize(newWidth, newHeight);

    // Comparar píxeles de la imagen redimensionada y la imagen esperada
    for (unsigned long posy = 0; posy < newHeight; ++posy) {
      for (unsigned long posx = 0; posx < newWidth; ++posx) {
        constexpr int tolerance               = 5;
        imageaos::Pixel const & resultPixel   = inputImage.getPixel(posx, posy);
        imageaos::Pixel const & expectedPixel = expectedImage.getPixel(posx, posy);

        ASSERT_TRUE(comparePixelsWithTolerance(resultPixel, expectedPixel, tolerance))
            << "Diferencia en píxeles en la posición (" << posx << ", " << posy
            << ") entre la imagen redimensionada y la esperada para el archivo: " << file.inputPath;
      }
    }
    }
}