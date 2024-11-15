// resize_soa_utest.cpp
#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <imgsoa/imagesoa.hpp>
#include <vector>
#include <string>

namespace {
  constexpr int TOLERANCE = 5;

  struct ImageDimensions {
    unsigned long width;
    unsigned long height;
  };

  bool comparePixelsWithTolerance(unsigned short color1, unsigned short color2,
                                  int const tolerance) {
    return std::abs(static_cast<int>(color1) - static_cast<int>(color2)) <= tolerance;
  }

  bool loadImageDimensions(const std::string& filePath, ImageDimensions& dimensions) {
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

    file >> dimensions.width >> dimensions.height;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
  }

  bool loadImages(const std::string& inputPath, const std::string& expectedPath,
                  imagesoa::Image& inputImage, imagesoa::Image& expectedImage) {
    if (!inputImage.loadFromFile(inputPath)) {
      std::cerr << "No se pudo cargar la imagen de entrada: " << inputPath << '\n';
      return false;
    }
    if (!expectedImage.loadFromFile(expectedPath)) {
      std::cerr << "No se pudo cargar la imagen esperada: " << expectedPath << '\n';
      return false;
    }
    return true;
  }

  bool resizeImage(const std::string& expectedPath, imagesoa::Image& inputImage,
                   ImageDimensions& dimensions) {
    if (!loadImageDimensions(expectedPath, dimensions)) {
      std::cerr << "No se pudieron cargar las dimensiones de la imagen esperada: " << expectedPath << '\n';
      return false;
    }
    inputImage.resize(dimensions.width, dimensions.height);
    return true;
  }

  void compareImageChannels(const imagesoa::Image& inputImage, const imagesoa::Image& expectedImage,
                            const ImageDimensions& dimensions) {
    for (unsigned long posY = 0; posY < dimensions.height; ++posY) {
      for (unsigned long posX = 0; posX < dimensions.width; ++posX) {
        unsigned short const resultRed = inputImage.getRed(posX, posY);
        unsigned short const expectedRed = expectedImage.getRed(posX, posY);
        ASSERT_TRUE(comparePixelsWithTolerance(resultRed, expectedRed, TOLERANCE))
            << "Diferencia en el canal rojo en (" << posX << ", " << posY << ")";

        unsigned short const resultGreen = inputImage.getGreen(posX, posY);
        unsigned short const expectedGreen = expectedImage.getGreen(posX, posY);
        ASSERT_TRUE(comparePixelsWithTolerance(resultGreen, expectedGreen, TOLERANCE))
            << "Diferencia en el canal verde en (" << posX << ", " << posY << ")";

        unsigned short const resultBlue = inputImage.getBlue(posX, posY);
        unsigned short const expectedBlue = expectedImage.getBlue(posX, posY);
        ASSERT_TRUE(comparePixelsWithTolerance(resultBlue, expectedBlue, TOLERANCE))
            << "Diferencia en el canal azul en (" << posX << ", " << posY << ")";
      }
    }
  }
}  // namespace

TEST(ImageResizeSOATest, ResizeMultipleFiles) {
  struct TestFile {
      std::string inputPath;
      std::string expectedOutputPath;
  };

  std::vector<TestFile> const testFiles = {
    {.inputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/in/deer-large.ppm",
     .expectedOutputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/expected_outputs/deer-large-100.ppm"},
    {.inputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/in/deer-large.ppm",
     .expectedOutputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/expected_outputs/deer-large-1000.ppm"},
    {.inputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/in/deer-small.ppm",
     .expectedOutputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/expected_outputs/deer-small-100.ppm"},
    {.inputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/in/deer-small.ppm",
     .expectedOutputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/expected_outputs/deer-small-1000.ppm"},
    {.inputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/in/lake-large.ppm",
     .expectedOutputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/expected_outputs/lake-large-100.ppm"},
    {.inputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/in/lake-large.ppm",
     .expectedOutputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/expected_outputs/lake-large-1000.ppm"},
    {.inputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/in/lake-small.ppm",
     .expectedOutputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/expected_outputs/lake-small-100.ppm"},
    {.inputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/in/lake-small.ppm",
     .expectedOutputPath="/home/maria/Escritorio/ARQUITECTURA COMPU/imtool.p/expected_outputs/lake-small-1000.ppm"}
  };

  for (auto const& file : testFiles) {
    imagesoa::Image inputImage;
    imagesoa::Image expectedImage;
    ImageDimensions dimensions{};

    ASSERT_TRUE(loadImages(file.inputPath, file.expectedOutputPath, inputImage, expectedImage));
    ASSERT_TRUE(resizeImage(file.expectedOutputPath, inputImage, dimensions));

    compareImageChannels(inputImage, expectedImage, dimensions);
  }
}