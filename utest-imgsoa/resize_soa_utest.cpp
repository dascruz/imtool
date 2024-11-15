#include <gtest/gtest.h>
#include <imgsoa/imagesoa.hpp>
#include <cmath>

namespace imagesoa {

  class ImageSOATest : public ::testing::Test {
  public:
    Image image;
    static constexpr Dimensions IMAGE_DIMENSIONS = {.width = 200, .height = 200};
    static constexpr int COLOR = 50;

    static constexpr unsigned long SMALL_DIMENSION = 50;
    static constexpr unsigned long MEDIUM_DIMENSION = 300;
    static constexpr unsigned long LARGE_DIMENSION = 500;
    static constexpr unsigned long EXTRA_LARGE_DIMENSION = 1000;
    static constexpr unsigned long SPECIFIC_WIDTH = 100;
    static constexpr unsigned long SPECIFIC_HEIGHT = 20;
    static constexpr unsigned long TEST_POSITION = 10;
    static constexpr unsigned long CENTRAL_POSITION_SMALL = 25;
    static constexpr unsigned long CENTRAL_POSITION_LARGE = 150;

    void SetUp() override {
      image = Image(IMAGE_DIMENSIONS);

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
  };

  TEST_F(ImageSOATest, ResizeImageContent) {
    image.resize(MEDIUM_DIMENSION, MEDIUM_DIMENSION);

    EXPECT_NEAR(image.getRed(TEST_POSITION, TEST_POSITION), 5, 5);
    EXPECT_NEAR(image.getGreen(TEST_POSITION, TEST_POSITION), 0, 5);
    EXPECT_NEAR(image.getBlue(TEST_POSITION, TEST_POSITION), 5, 5);
  }

  TEST_F(ImageSOATest, ResizeToSmallerDimensions) {
    image.resize(SMALL_DIMENSION, SMALL_DIMENSION);
    EXPECT_EQ(SMALL_DIMENSION, SMALL_DIMENSION);

    EXPECT_NEAR(image.getRed(CENTRAL_POSITION_SMALL, CENTRAL_POSITION_SMALL), 5, 5);
    EXPECT_NEAR(image.getGreen(CENTRAL_POSITION_SMALL, CENTRAL_POSITION_SMALL), 0, 5);
    EXPECT_NEAR(image.getBlue(CENTRAL_POSITION_SMALL, CENTRAL_POSITION_SMALL), 5, 5);
  }

  TEST_F(ImageSOATest, ResizeToLargerDimensions) {
    image.resize(LARGE_DIMENSION, LARGE_DIMENSION);
    EXPECT_EQ(LARGE_DIMENSION, LARGE_DIMENSION);

    EXPECT_NEAR(image.getRed(CENTRAL_POSITION_LARGE, CENTRAL_POSITION_LARGE), 5, 5);
    EXPECT_NEAR(image.getGreen(CENTRAL_POSITION_LARGE, CENTRAL_POSITION_LARGE), 0, 5);
    EXPECT_NEAR(image.getBlue(CENTRAL_POSITION_LARGE, CENTRAL_POSITION_LARGE), 5, 5);
  }

  TEST_F(ImageSOATest, ResizeToExtraLargerDimensions) {
    image.resize(EXTRA_LARGE_DIMENSION, EXTRA_LARGE_DIMENSION);
    EXPECT_EQ(EXTRA_LARGE_DIMENSION, EXTRA_LARGE_DIMENSION);

    EXPECT_NEAR(image.getRed(CENTRAL_POSITION_LARGE, CENTRAL_POSITION_LARGE), 5, 5);
    EXPECT_NEAR(image.getGreen(CENTRAL_POSITION_LARGE, CENTRAL_POSITION_LARGE), 0, 5);
    EXPECT_NEAR(image.getBlue(CENTRAL_POSITION_LARGE, CENTRAL_POSITION_LARGE), 5, 5);
  }

  TEST_F(ImageSOATest, ResizeImageTo100x20) {
    image.resize(SPECIFIC_WIDTH, SPECIFIC_HEIGHT);
    EXPECT_EQ(SPECIFIC_WIDTH, SPECIFIC_WIDTH);
    EXPECT_EQ(SPECIFIC_HEIGHT, SPECIFIC_HEIGHT);

    EXPECT_NEAR(image.getRed(TEST_POSITION, TEST_POSITION), 5, 5);
    EXPECT_NEAR(image.getGreen(TEST_POSITION, TEST_POSITION), 0, 5);
    EXPECT_NEAR(image.getBlue(TEST_POSITION, TEST_POSITION), 5, 5);
  }

}  // namespace imagesoa
