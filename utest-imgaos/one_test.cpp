#include <cmath>
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

      // Función auxiliar para contar colores en la imagen
      [[nodiscard]] std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int> countColors() const {
        std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int> colorFrequency;
        for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
          for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
            Pixel const & pixel = image.getPixel(x_pos, y_pos);
            auto color          = std::make_tuple(pixel.red, pixel.green, pixel.blue);
            colorFrequency[color]++;
          }
        }
        return colorFrequency;
      }

      Image & getImage() { return image; }
  };

  // T1-Verificar que la lista de colores eliminados contiene los colores menos frecuentes
  TEST_F(ImageTest, VerifyColorsToRemove) {
    constexpr int ncolor = 5;
    auto colorFrequency  = image.countColorFrequencies();
    auto sortedColors    = Image::sortColorsByFrequency(colorFrequency);

    auto [colorsToRemove, _] = Image::splitColors(sortedColors, ncolor);

    EXPECT_EQ(colorsToRemove.size(), ncolor);

    for (std::size_t i = 0; i < ncolor; ++i) {
      EXPECT_EQ(sortedColors[i].first, colorsToRemove[i]);
    }
  }

  // T2-Verificar que la lista de colores a sustituir contiene los colores restantes
  TEST_F(ImageTest, VerifyRemainingColors) {
    constexpr int ncolor = 5;  // Número de colores menos frecuentes
    auto colorFrequency  = image.countColorFrequencies();
    auto sortedColors    = Image::sortColorsByFrequency(colorFrequency);

    auto [_, remainingColors] = Image::splitColors(sortedColors, ncolor);

    EXPECT_EQ(remainingColors.size(), sortedColors.size() - ncolor);

    for (std::size_t i = 0; i < remainingColors.size(); ++i) {
      EXPECT_EQ(sortedColors[i + ncolor].first, remainingColors[i]);
    }
  }

  static constexpr uint16_t COLOR_TO_REMOVE_RED   = 50;
  static constexpr uint16_t COLOR_TO_REMOVE_GREEN = 80;
  static constexpr uint16_t COLOR_TO_REMOVE_BLUE  = 100;

  static constexpr uint16_t COLOR1_RED   = 52;
  static constexpr uint16_t COLOR1_GREEN = 82;
  static constexpr uint16_t COLOR1_BLUE  = 102;

  static constexpr uint16_t COLOR2_RED   = 48;
  static constexpr uint16_t COLOR2_GREEN = 78;
  static constexpr uint16_t COLOR2_BLUE  = 98;

  // T3-Verificar que la función findClosestColor mediante distancia euclidiana al cuadrado
  TEST_F(ImageTest, ClosestColorEuclideanAOS) {
    std::tuple<uint16_t, uint16_t, uint16_t> colorToRemove{
      COLOR_TO_REMOVE_RED, COLOR_TO_REMOVE_GREEN, COLOR_TO_REMOVE_BLUE};

    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> const remainingColors = {
      std::make_tuple(52, 78, 102), std::make_tuple(55, 85, 110), std::make_tuple(70, 90, 120)};

    std::tuple<uint16_t, uint16_t, uint16_t> closestColor =
        imageaos::Image::findClosestColor(colorToRemove, remainingColors);
    EXPECT_EQ(std::get<0>(closestColor), 52);
    EXPECT_EQ(std::get<1>(closestColor), 78);
    EXPECT_EQ(std::get<2>(closestColor), 102);

    // Verificar que la distancia al cuadrado calculada es la menor
    int const minDistanceSquared = ((static_cast<int>(std::get<0>(colorToRemove)) - 52) *
                                    (static_cast<int>(std::get<0>(colorToRemove)) - 52)) +
                                   ((static_cast<int>(std::get<1>(colorToRemove)) - 78) *
                                    (static_cast<int>(std::get<1>(colorToRemove)) - 78)) +
                                   ((static_cast<int>(std::get<2>(colorToRemove)) - 102) *
                                    (static_cast<int>(std::get<2>(colorToRemove)) - 102));

    for (auto const & color : remainingColors) {
      int const distanceSquared =
          ((static_cast<int>(std::get<0>(colorToRemove)) - std::get<0>(color)) *
           (static_cast<int>(std::get<0>(colorToRemove)) - std::get<0>(color))) +
          ((static_cast<int>(std::get<1>(colorToRemove)) - std::get<1>(color)) *
           (static_cast<int>(std::get<1>(colorToRemove)) - std::get<1>(color))) +
          ((static_cast<int>(std::get<2>(colorToRemove)) - std::get<2>(color)) *
           (static_cast<int>(std::get<2>(colorToRemove)) - std::get<2>(color)));
      EXPECT_GE(distanceSquared, minDistanceSquared);
    }
  }

  // T4-Verifica que en caso de empate en distancia, se selecciona el primer color de la lista
  TEST_F(ImageTest, ClosestColorTieBreakerAOS) {
    constexpr std::tuple<uint16_t, uint16_t, uint16_t> colorToRemove{
      COLOR_TO_REMOVE_RED, COLOR_TO_REMOVE_GREEN, COLOR_TO_REMOVE_BLUE};

    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> remainingColors = {
      std::make_tuple(COLOR1_RED, COLOR1_GREEN, COLOR1_BLUE),  // Primer color (esperado)
      std::make_tuple(COLOR2_RED, COLOR2_GREEN, COLOR2_BLUE)   // Segundo color
    };

    std::tuple<uint16_t, uint16_t, uint16_t> const closestColor =
        imageaos::Image::findClosestColor(colorToRemove, remainingColors);

    EXPECT_EQ(closestColor, remainingColors[0]);
  }

  // T5-Verifica el mapa de reemplazo construido correctamente
  TEST_F(ImageTest, VerifyBuildReplacementMap) {
    constexpr int ncolor                   = 5;
    auto colorFrequency                    = image.countColorFrequencies();
    auto sortedColors                      = Image::sortColorsByFrequency(colorFrequency);
    auto [colorsToRemove, remainingColors] = Image::splitColors(sortedColors, ncolor);

    auto replacementMap =
        Image::buildReplacementMap(std::make_pair(colorsToRemove, remainingColors));

    EXPECT_EQ(replacementMap.size(), colorsToRemove.size());

    // Verificar que el color de reemplazo es el más cercano en distancia euclidiana
    for (auto const & colorToRemove : colorsToRemove) {
      auto closestColor = Image::findClosestColor(colorToRemove, remainingColors);
      EXPECT_EQ(replacementMap[colorToRemove], closestColor);
    }
  }

  // T6-Verifica que los colores se reemplazan correctamente en la imagen
  TEST_F(ImageTest, VerifyReplaceColors) {
    constexpr int ncolor                   = 5;
    auto colorFrequency                    = image.countColorFrequencies();
    auto sortedColors                      = imageaos::Image::sortColorsByFrequency(colorFrequency);
    auto [colorsToRemove, remainingColors] = imageaos::Image::splitColors(sortedColors, ncolor);
    auto replacementMap =
        imageaos::Image::buildReplacementMap(std::make_pair(colorsToRemove, remainingColors));

    image.replaceColors(replacementMap);
    auto updatedColorFrequency = countColors();

    for (auto const & color : colorsToRemove) {
      EXPECT_EQ(updatedColorFrequency.count(color), 0);  // El color ya no debería estar presente
    }

    // Verificar que el número de colores únicos es correcto
    EXPECT_LE(updatedColorFrequency.size(),
              (IMAGE_DIMENSIONS.width * IMAGE_DIMENSIONS.height) - ncolor);
  }

  // T7-Verifica que splitColors maneja correctamente casos extremos
  TEST_F(ImageTest, HandleExtremeCasesInSplitColors) {
    auto colorFrequency = image.countColorFrequencies();
    auto sortedColors   = Image::sortColorsByFrequency(colorFrequency);

    // Caso extremo: `n` es mayor que el número de colores
    constexpr std::size_t extraColors = 5;
    std::size_t const ncolor          = sortedColors.size() + extraColors;
    auto [colorsToRemove, remainingColors] =
        Image::splitColors(sortedColors, static_cast<std::uint32_t>(ncolor));

    EXPECT_EQ(colorsToRemove.size(), sortedColors.size());
    EXPECT_EQ(remainingColors.size(), 0);

    // Caso extremo: `n` es cero
    auto [noColorsToRemove, allColorsRemaining] = Image::splitColors(sortedColors, 0);

    EXPECT_EQ(noColorsToRemove.size(), 0);
    EXPECT_EQ(allColorsRemaining.size(), sortedColors.size());
  }

  // T8-Verifica que countColorFrequencies maneja imágenes vacías correctamente
  TEST(ImageTestEmpty, CountColorFrequenciesOnEmptyImage) {
    imageaos::Image const emptyImage({.width = 0, .height = 0});

    auto colorFrequency = emptyImage.countColorFrequencies();
    EXPECT_TRUE(colorFrequency.empty());
  }

}  // namespace imageaos
