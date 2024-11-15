#include <gtest/gtest.h>
#include <imgsoa/imagesoa.hpp>

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

      // Función auxiliar para contar colores en la imagen
      [[nodiscard]] std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int> countColors() const {
        std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int> colorFrequency;
        for (unsigned long y_pos = 0; y_pos < IMAGE_DIMENSIONS.height; ++y_pos) {
          for (unsigned long x_pos = 0; x_pos < IMAGE_DIMENSIONS.width; ++x_pos) {
            auto color = std::make_tuple(image.getRed(x_pos, y_pos), image.getGreen(x_pos, y_pos),
                                         image.getBlue(x_pos, y_pos));
            colorFrequency[color]++;
          }
        }
        return colorFrequency;
      }

      Image & getImage() { return image; }
  };

  // T1-Verificar que cutfreq elimina los colores menos frecuentes
  TEST_F(ImageSOATest, CutFreqRemovesLeastFrequentColors) {
    constexpr uint32_t ncolor  = 5;  // Número de colores menos frecuentes a eliminar
    auto initialColorFrequency = countColors();

    image.cutfreq(ncolor);
    auto updatedColorFrequency = countColors();

    auto sortedColors = Image::sortColorsByFrequency(initialColorFrequency);
    for (std::size_t i = 0; i < ncolor; ++i) {
      EXPECT_EQ(updatedColorFrequency.count(sortedColors[i].first), 0);
    }
  }

  // T2-Verificar que cutfreq reemplaza colores eliminados con los más cercanos
  TEST_F(ImageSOATest, CutFreqReplacesWithClosestColors) {
    constexpr uint32_t ncolor  = 5;
    auto initialColorFrequency = countColors();

    image.cutfreq(ncolor);
    auto updatedColorFrequency = countColors();

    auto sortedColors                      = Image::sortColorsByFrequency(initialColorFrequency);
    auto [colorsToRemove, remainingColors] = Image::splitColors(sortedColors, ncolor);
    auto replacementMap = Image::buildReplacementMap({colorsToRemove, remainingColors});

    for (auto const & color : colorsToRemove) {
      EXPECT_EQ(updatedColorFrequency.count(color), 0);

      // Verificar que el color de reemplazo está presente
      auto closestColor = replacementMap[color];
      EXPECT_GT(updatedColorFrequency.count(closestColor), 0);
    }
  }

  // T3-Verificar el número de colores únicos después de cutfreq
  TEST_F(ImageSOATest, UniqueColorsCountAfterCutFreq) {
    constexpr uint32_t ncolor  = 5;
    auto initialColorFrequency = countColors();

    image.cutfreq(ncolor);
    auto updatedColorFrequency = countColors();

    EXPECT_LE(updatedColorFrequency.size(), initialColorFrequency.size() - ncolor);
  }

  // T4-Verificar que cutfreq no modifica colores cuando `n` es cero
  TEST_F(ImageSOATest, CutFreqNoChangeWhenZeroColorsToRemove) {
    auto initialColorFrequency = countColors();

    // Ejecutamos cutfreq con `n = 0`
    image.cutfreq(0);
    auto updatedColorFrequency = countColors();

    EXPECT_EQ(initialColorFrequency, updatedColorFrequency);
  }

  // T5-Verificar que cutfreq elimina todos los colores si `n` es mayor o igual al número de colores
  // únicos
  TEST_F(ImageSOATest, CutFreqRemovesAllColorsIfNExceedsUniqueColors) {
    auto initialColorFrequency = countColors();
    auto ncolor                = static_cast<uint32_t>(initialColorFrequency.size());

    image.cutfreq(ncolor);
    auto updatedColorFrequency = countColors();

    EXPECT_LE(updatedColorFrequency.size(), 1);
  }

  static constexpr uint16_t COLOR_TO_REMOVE_RED   = 50;
  static constexpr uint16_t COLOR_TO_REMOVE_GREEN = 80;
  static constexpr uint16_t COLOR_TO_REMOVE_BLUE  = 100;

  // T6-Verificar que la función findClosestColor mediante distancia euclidiana
  TEST_F(ImageSOATest, VerifyFindClosestColor) {
    std::tuple<uint16_t, uint16_t, uint16_t> colorToRemove = {
      COLOR_TO_REMOVE_RED, COLOR_TO_REMOVE_GREEN, COLOR_TO_REMOVE_BLUE};

    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> const remainingColors = {
      {52, 78, 102}, // Más cercano
      {55, 85, 110},
      {70, 90, 120}
    };

    auto closestColor = imagesoa::Image::findClosestColor(colorToRemove, remainingColors);
    EXPECT_EQ(closestColor, std::make_tuple(52, 78, 102));

    // Verificar manualmente que la distancia calculada es la mínima
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

}
