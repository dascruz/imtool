#include <gtest/gtest.h>
#include <imgaos/imageaos.hpp>
#include <sstream>
#include <iostream>


namespace imageaos {
  class ImageTest : public ::testing::Test {
    Image image;
    static constexpr Dimensions IMAGE_DIMENSIONS = {.width=200, .height=200};
    static constexpr int COLOR = 50;
  protected:
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
    Image& getImage() {
      return image;
    }
  };

  TEST_F(ImageTest, DisplayMetadata) {
    std::stringstream const buffer;
    std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());

    getImage().displayMetadata();

    std::cout.rdbuf(prevcoutbuf);

    std::string const expectedOutput = "Image Metadata:\nWidth: 200\nHeight: 200\nMax Color Value: 255\n";
    EXPECT_EQ(buffer.str(), expectedOutput);
  }

  // Imagen con min color value 1
  TEST_F(ImageTest, MinMaxColorMetadata) {
    constexpr Dimensions dims = {.width=200, .height=200};
    Image const minColorImage(dims, 1);

    std::stringstream const buffer;
    std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());

    minColorImage.displayMetadata();

    std::cout.rdbuf(prevcoutbuf);

    std::string const expectedOutput = "Image Metadata:\nWidth: 200\nHeight: 200\nMax Color Value: 1\n";
    EXPECT_EQ(buffer.str(), expectedOutput);
  }


  // Imagen con max color value 65535
  TEST_F(ImageTest, MaxPossibleColorMetadata) {
    constexpr Dimensions dims = {.width=200, .height=200};
    Image const maxColorImage(dims, 65535);

    std::stringstream const buffer;
    std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());

    maxColorImage.displayMetadata();

    std::cout.rdbuf(prevcoutbuf);

    std::string const expectedOutput = "Image Metadata:\nWidth: 200\nHeight: 200\nMax Color Value: 65535\n";
    EXPECT_EQ(buffer.str(), expectedOutput);
  }
}
