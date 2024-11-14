/*#include <gtest/gtest.h>
#include <imgsoa/imagesoa.hpp>
#include <sstream>

namespace imagesoa {
  class ImageSOATest : public ::testing::Test {
    public:
    Image image;
    static constexpr Dimensions IMAGE_DIMENSIONS = {.width = 200, .height = 200};
    static constexpr int COLOR                   = 50;

    void SetUp() override {
      image = Image(IMAGE_DIMENSIONS);

      // Configurar colores en la imagen con frecuencias controladas
      for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
        for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
          auto redValue   = static_cast<uint16_t>((x_pos + y_pos) % 3 * COLOR);
          auto greenValue = static_cast<uint16_t>((x_pos * y_pos) % 3 * COLOR);
          auto blueValue  = static_cast<uint16_t>((x_pos - y_pos) % 3 * COLOR);

          image.setRed(x_pos, y_pos, redValue);
          image.setGreen(x_pos, y_pos, greenValue);
          image.setBlue(x_pos, y_pos, blueValue);
        }
      }
    }

    Image & getImage() { return image; }
  };

  // Test para verificar los metadatos de la imagen usando displayMetadata
  TEST_F(ImageSOATest, DisplayMetadata) {
  }
}*/