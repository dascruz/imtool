#include <gtest/gtest.h>
#include <imgsoa/imagesoa.hpp>
#include <cmath>

namespace imagesoa {

  // Clase derivada para exponer `getMaxColorValue`
  class TestableImage : public Image {
  public:
    using Image::Image; // Hereda los constructores
    [[nodiscard]] unsigned short getMaxColorValuePublic() const {
      return getMaxColorValue();
    }
  };

  class ImageSOATest : public ::testing::Test {
  public:
    TestableImage image;
    static constexpr Dimensions IMAGE_DIMENSIONS = {.width = 200, .height = 200};
    static constexpr int COLOR = 50;

    static constexpr Dimensions SMALL_DIMENSIONS = {.width = 50, .height = 50};
    static constexpr Dimensions MEDIUM_DIMENSIONS = {.width = 300, .height = 300};
    static constexpr Dimensions LARGE_DIMENSIONS = {.width = 500, .height = 500};
    static constexpr Dimensions EXTRA_LARGE_DIMENSIONS = {.width = 1000, .height = 1000};
    static constexpr Dimensions SPECIFIC_DIMENSIONS = {.width = 100, .height = 20};

    void SetUp() override {
      image = TestableImage(IMAGE_DIMENSIONS);

      for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
        for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
          auto redValue = static_cast<uint16_t>((x_pos + y_pos) % 3 * COLOR);
          auto greenValue = static_cast<uint16_t>((x_pos * y_pos) % 3 * COLOR);
          auto blueValue = static_cast<uint16_t>((x_pos - y_pos) % 3 * COLOR);

          image.setRed(x_pos, y_pos, redValue);
          image.setGreen(x_pos, y_pos, greenValue);
          image.setBlue(x_pos, y_pos, blueValue);
        }
      }
    }

    // Helper function to verify a single pixel's color channels
    void VerifyPixel(unsigned long posx, unsigned long posy) const {
      EXPECT_GE(image.getRed(posx, posy), 0) << "Red channel out of range at (" << posx << ", " << posy << ")";
      EXPECT_LE(image.getRed(posx, posy), image.getMaxColorValuePublic())
          << "Red channel out of range at (" << posx << ", " << posy << ")";

      EXPECT_GE(image.getGreen(posx, posy), 0) << "Green channel out of range at (" << posx << ", " << posy << ")";
      EXPECT_LE(image.getGreen(posx, posy), image.getMaxColorValuePublic())
          << "Green channel out of range at (" << posx << ", " << posy << ")";

      EXPECT_GE(image.getBlue(posx, posy), 0) << "Blue channel out of range at (" << posx << ", " << posy << ")";
      EXPECT_LE(image.getBlue(posx, posy), image.getMaxColorValuePublic())
          << "Blue channel out of range at (" << posx << ", " << posy << ")";
    }

    // Verify all pixels in the resized image
    void VerifyAllPixels(Dimensions dimensions) const {
      for (unsigned long posy = 0; posy < dimensions.height; ++posy) {
        for (unsigned long posx = 0; posx < dimensions.width; ++posx) {
          VerifyPixel(posx, posy);
        }
      }
    }
  };

  TEST_F(ImageSOATest, ResizeImageContent) {
    image.resize(MEDIUM_DIMENSIONS.width, MEDIUM_DIMENSIONS.height);
    VerifyAllPixels(MEDIUM_DIMENSIONS);
  }

  TEST_F(ImageSOATest, ResizeToSmallerDimensions) {
    image.resize(SMALL_DIMENSIONS.width, SMALL_DIMENSIONS.height);
    VerifyAllPixels(SMALL_DIMENSIONS);
  }

  TEST_F(ImageSOATest, ResizeToLargerDimensions) {
    image.resize(LARGE_DIMENSIONS.width, LARGE_DIMENSIONS.height);
    VerifyAllPixels(LARGE_DIMENSIONS);
  }

  TEST_F(ImageSOATest, ResizeToExtraLargerDimensions) {
    image.resize(EXTRA_LARGE_DIMENSIONS.width, EXTRA_LARGE_DIMENSIONS.height);
    VerifyAllPixels(EXTRA_LARGE_DIMENSIONS);
  }

  TEST_F(ImageSOATest, ResizeImageTo100x20) {
    image.resize(SPECIFIC_DIMENSIONS.width, SPECIFIC_DIMENSIONS.height);
    VerifyAllPixels(SPECIFIC_DIMENSIONS);
  }

}  // namespace imagesoa