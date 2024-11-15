#include <gtest/gtest.h>
#include <imgaos/imageaos.hpp>
#include <cmath>

namespace imageaos {

  struct ImageDimensions {
      unsigned long width;
      unsigned long height;
  };

  class ImageTest : public ::testing::Test {
    public:
      Image image;
      static constexpr ImageDimensions IMAGE_DIMENSIONS       = {.width = 200, .height = 200};
      static constexpr int COLOR                              = 50;
      static constexpr ImageDimensions SMALL_DIMENSIONS       = {.width = 50, .height = 50};
      static constexpr ImageDimensions MEDIUM_DIMENSIONS      = {.width = 300, .height = 300};
      static constexpr ImageDimensions LARGE_DIMENSIONS       = {.width = 500, .height = 500};
      static constexpr ImageDimensions EXTRA_LARGE_DIMENSIONS = {.width = 1000, .height = 1000};
      static constexpr unsigned long INITIAL_SIZE        = 100;
      static constexpr unsigned long TARGET_HEIGHT       = 20;

      void SetUp() override {
        image =
            Image(Dimensions{.width = IMAGE_DIMENSIONS.width, .height = IMAGE_DIMENSIONS.height});

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

      static void checkRedValue(Pixel const & pixel) {
        EXPECT_GE(pixel.red, 0);
        EXPECT_LE(pixel.red, image::MAX_COLOR_VALUE_16BIT);
      }

      static void checkGreenValue(Pixel const & pixel) {
        EXPECT_GE(pixel.green, 0);
        EXPECT_LE(pixel.green, image::MAX_COLOR_VALUE_16BIT);
      }

      static void checkBlueValue(Pixel const & pixel) {
        EXPECT_GE(pixel.blue, 0);
        EXPECT_LE(pixel.blue, image::MAX_COLOR_VALUE_16BIT);
      }

      static void checkPixelValues(Image const & img, ImageDimensions const & dimensions) {
        for (unsigned long posy = 0; posy < dimensions.height; ++posy) {
          for (unsigned long posx = 0; posx < dimensions.width; ++posx) {
            Pixel const pixel = img.getPixel(posx, posy);
            checkRedValue(pixel);
            checkGreenValue(pixel);
            checkBlueValue(pixel);
          }
        }
      }
  };

  TEST_F(ImageTest, ResizeImageContentSmall) {
    getImage().resize(SMALL_DIMENSIONS.width, SMALL_DIMENSIONS.height);
    EXPECT_EQ(getImage().getWidth(), SMALL_DIMENSIONS.width);
    EXPECT_EQ(getImage().getHeight(), SMALL_DIMENSIONS.height);

    ImageTest::checkPixelValues(getImage(), SMALL_DIMENSIONS);
  }

  TEST_F(ImageTest, ResizeImageContentMedium) {
    getImage().resize(MEDIUM_DIMENSIONS.width, MEDIUM_DIMENSIONS.height);
    EXPECT_EQ(getImage().getWidth(), MEDIUM_DIMENSIONS.width);
    EXPECT_EQ(getImage().getHeight(), MEDIUM_DIMENSIONS.height);

    ImageTest::checkPixelValues(getImage(), MEDIUM_DIMENSIONS);
  }

  TEST_F(ImageTest, ResizeImageContentLarge) {
    getImage().resize(LARGE_DIMENSIONS.width, LARGE_DIMENSIONS.height);
    EXPECT_EQ(getImage().getWidth(), LARGE_DIMENSIONS.width);
    EXPECT_EQ(getImage().getHeight(), LARGE_DIMENSIONS.height);

    ImageTest::checkPixelValues(getImage(), LARGE_DIMENSIONS);
  }

  TEST_F(ImageTest, ResizeImageContentExtraLarge) {
    getImage().resize(EXTRA_LARGE_DIMENSIONS.width, EXTRA_LARGE_DIMENSIONS.height);
    EXPECT_EQ(getImage().getWidth(), EXTRA_LARGE_DIMENSIONS.width);
    EXPECT_EQ(getImage().getHeight(), EXTRA_LARGE_DIMENSIONS.height);

    ImageTest::checkPixelValues(getImage(), EXTRA_LARGE_DIMENSIONS);
  }

  TEST_F(ImageTest, ResizeToSpecificSize) {
    getImage().resize(INITIAL_SIZE, TARGET_HEIGHT);
    EXPECT_EQ(getImage().getWidth(), INITIAL_SIZE);
    EXPECT_EQ(getImage().getHeight(), TARGET_HEIGHT);

    ImageTest::checkPixelValues(getImage(), ImageDimensions{.width = INITIAL_SIZE, .height = TARGET_HEIGHT});
  }

}  // namespace imageaos
