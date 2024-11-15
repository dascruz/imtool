#include <gtest/gtest.h>
#include <imgaos/imageaos.hpp>

namespace imageaos {
  class ImageTest : public ::testing::Test {
    public:
      Image image;
      static constexpr Dimensions IMAGE_DIMENSIONS = {.width = 200, .height = 200};
      static constexpr int COLOR                   = 50;

      void SetUp() override {
        image = Image(IMAGE_DIMENSIONS);

        // Configurar colores en la imagen con frecuencias controladas
        for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
          for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
            Pixel pixel;
            pixel.setRed(static_cast<uint16_t>((x_pos + y_pos) % 3 * COLOR));
            pixel.setGreen(static_cast<uint16_t>((x_pos * y_pos) % 3 * COLOR));
            pixel.setBlue(static_cast<uint16_t>((x_pos - y_pos) % 3 * COLOR));
            image.setPixel(x_pos, y_pos, pixel);
          }
        }
      }

      Image & getImage() { return image; }
  };

  // Test para Verificar que los Valores de los Píxeles se Escalen Correctamente
  TEST_F(ImageTest, ModifyMaxLevelScalesPixelColors) {
    constexpr unsigned short newMaxColorValue  = 500;
    unsigned short const originalMaxColorValue = getImage().getMaxColorValue();

    // Almacenar los valores originales de los píxeles antes de escalar
    std::vector<std::vector<Pixel>> originalPixels(IMAGE_DIMENSIONS.height,
                                                   std::vector<Pixel>(IMAGE_DIMENSIONS.width));
    for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
      for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
        originalPixels[y_pos][x_pos] = getImage().getPixel(x_pos, y_pos);
      }
    }

    // Aplicar modifyMaxLevel
    getImage().modifyMaxLevel(newMaxColorValue);

    for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
      for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
        Pixel const & pixel         = getImage().getPixel(x_pos, y_pos);
        Pixel const & originalPixel = originalPixels[y_pos][x_pos];

        auto expectedRed   = static_cast<unsigned short>(originalPixel.red * newMaxColorValue /
                                                       originalMaxColorValue);
        auto expectedGreen = static_cast<unsigned short>(originalPixel.green * newMaxColorValue /
                                                         originalMaxColorValue);
        auto expectedBlue  = static_cast<unsigned short>(originalPixel.blue * newMaxColorValue /
                                                        originalMaxColorValue);

        EXPECT_EQ(pixel.red, expectedRed);
        EXPECT_EQ(pixel.green, expectedGreen);
        EXPECT_EQ(pixel.blue, expectedBlue);
      }
    }
  }

  // Test para verificar el caso de que el Nuevo Valor Máximo es Igual al Actual
  TEST_F(ImageTest, ModifyMaxLevelNoChangeIfSameMaxValue) {
    unsigned short const originalMaxColorValue = getImage().getMaxColorValue();

    std::vector<std::vector<Pixel>> originalPixels(IMAGE_DIMENSIONS.height,
                                                   std::vector<Pixel>(IMAGE_DIMENSIONS.width));
    for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
      for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
        originalPixels[y_pos][x_pos] = getImage().getPixel(x_pos, y_pos);
      }
    }

    getImage().modifyMaxLevel(originalMaxColorValue);

    for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
      for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
        Pixel const & pixel         = getImage().getPixel(x_pos, y_pos);
        Pixel const & originalPixel = originalPixels[y_pos][x_pos];

        EXPECT_EQ(pixel.red, originalPixel.red);
        EXPECT_EQ(pixel.green, originalPixel.green);
        EXPECT_EQ(pixel.blue, originalPixel.blue);
      }
    }
  }

  // Test: max color value = 1
  TEST_F(ImageTest, ModifyMaxLevelMinColorValue) {
    constexpr unsigned short newMaxColorValue  = 1;
    unsigned short const originalMaxColorValue = getImage().getMaxColorValue();

    std::vector<std::vector<Pixel>> originalPixels(IMAGE_DIMENSIONS.height,
                                                   std::vector<Pixel>(IMAGE_DIMENSIONS.width));
    for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
      for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
        originalPixels[y_pos][x_pos] = getImage().getPixel(x_pos, y_pos);
      }
    }

    getImage().modifyMaxLevel(newMaxColorValue);

    for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
      for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
        Pixel const & pixel         = getImage().getPixel(x_pos, y_pos);
        Pixel const & originalPixel = originalPixels[y_pos][x_pos];

        auto expectedRed   = static_cast<unsigned short>(originalPixel.red * newMaxColorValue /
                                                       originalMaxColorValue);
        auto expectedGreen = static_cast<unsigned short>(originalPixel.green * newMaxColorValue /
                                                         originalMaxColorValue);
        auto expectedBlue  = static_cast<unsigned short>(originalPixel.blue * newMaxColorValue /
                                                        originalMaxColorValue);

        EXPECT_EQ(pixel.red, expectedRed);
        EXPECT_EQ(pixel.green, expectedGreen);
        EXPECT_EQ(pixel.blue, expectedBlue);
      }
    }
  }

  // Test: max color value = 65535
  TEST_F(ImageTest, ModifyMaxLevelMaxColorValue) {
    constexpr unsigned short newMaxColorValue  = 65535;
    unsigned short const originalMaxColorValue = getImage().getMaxColorValue();

    std::vector<std::vector<Pixel>> originalPixels(IMAGE_DIMENSIONS.height,
                                                   std::vector<Pixel>(IMAGE_DIMENSIONS.width));
    for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
      for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
        originalPixels[y_pos][x_pos] = getImage().getPixel(x_pos, y_pos);
      }
    }

    getImage().modifyMaxLevel(newMaxColorValue);

    for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
      for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
        Pixel const & pixel         = getImage().getPixel(x_pos, y_pos);
        Pixel const & originalPixel = originalPixels[y_pos][x_pos];

        auto expectedRed   = static_cast<unsigned short>(originalPixel.red * newMaxColorValue /
                                                       originalMaxColorValue);
        auto expectedGreen = static_cast<unsigned short>(originalPixel.green * newMaxColorValue /
                                                         originalMaxColorValue);
        auto expectedBlue  = static_cast<unsigned short>(originalPixel.blue * newMaxColorValue /
                                                        originalMaxColorValue);

        EXPECT_EQ(pixel.red, expectedRed);
        EXPECT_EQ(pixel.green, expectedGreen);
        EXPECT_EQ(pixel.blue, expectedBlue);
      }
    }
  }

  // Imagen en blanco
  TEST_F(ImageTest, ModifyMaxLevelWithAllZeroPixels) {
    constexpr unsigned short newMaxColorValue = 500;

    // Configurar todos los píxeles a (0, 0, 0)
    for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
      for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
        constexpr Pixel blackPixel = {.red = 0, .green = 0, .blue = 0};
        getImage().setPixel(x_pos, y_pos, blackPixel);
      }
    }

    getImage().modifyMaxLevel(newMaxColorValue);

    for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
      for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
        Pixel const & pixel = getImage().getPixel(x_pos, y_pos);

        EXPECT_EQ(pixel.red, 0);
        EXPECT_EQ(pixel.green, 0);
        EXPECT_EQ(pixel.blue, 0);
      }
    }
  }

}  // namespace imageaos