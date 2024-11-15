#include <algorithm>
#include <cmath>
#include <common/image.hpp>
#include <imgsoa/imagesoa.hpp>

namespace imagesoa {
  // Cuenta la frecuencia de cada color en la imagen
  std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int>
      imagesoa::Image::countColorFrequencies() const {
    std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int> colorFrequency;
    for (unsigned long yPos = 0; yPos < getHeight(); ++yPos) {
      for (unsigned long xPos = 0; xPos < getWidth(); ++xPos) {
        auto color = std::make_tuple(getRed(xPos, yPos), getGreen(xPos, yPos), getBlue(xPos, yPos));
        colorFrequency[color]++;
      }
    }
    return colorFrequency;
  }

  // Ordena los colores por frecuencia, de menor a mayor
  std::vector<std::pair<std::tuple<uint16_t, uint16_t, uint16_t>, int>>
      Image::sortColorsByFrequency(
          std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int> const & colorFrequency) {
    std::vector<std::pair<std::tuple<uint16_t, uint16_t, uint16_t>, int>> colorFreqVector(
        colorFrequency.begin(), colorFrequency.end());
    std::ranges::sort(colorFreqVector, [](auto const & colorFreq1, auto const & colorFreq2) {
      return colorFreq1.second < colorFreq2.second;
    });
    return colorFreqVector;
  }

  // Divide los colores en dos grupos: los menos frecuentes para eliminar y el resto para conservar
  std::pair<std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>,
            std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>>
      Image::splitColors(
          std::vector<std::pair<std::tuple<uint16_t, uint16_t, uint16_t>, int>> const &
              colorFreqVector,
          uint32_t n) {
    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> colorsToRemove;
    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> remainingColors;

    for (std::size_t i = 0; i < colorFreqVector.size(); ++i) {
      if (i < n) {
        colorsToRemove.push_back(colorFreqVector[i].first);
      } else {
        remainingColors.push_back(colorFreqVector[i].first);
      }
    }
    return {colorsToRemove, remainingColors};
  }

  // Encuentra el color más cercano al color especificado en una lista de colores
  std::tuple<uint16_t, uint16_t, uint16_t> Image::findClosestColor(
      std::tuple<uint16_t, uint16_t, uint16_t> const & colorToRemove,
      std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> const & remainingColors) {
    auto [r1, g1, b1]      = colorToRemove;
    int minDistanceSquared = std::numeric_limits<int>::max();
    std::tuple<uint16_t, uint16_t, uint16_t> closestColor;

    for (auto const & color : remainingColors) {
      auto [r2, g2, b2] = color;
      // Calculamos la distancia euclidiana al cuadrado, sin la raíz cuadrada para optimizar
      int const distanceSquared = ((static_cast<int>(r1) - r2) * (static_cast<int>(r1) - r2)) +
                                  ((static_cast<int>(g1) - g2) * (static_cast<int>(g1) - g2)) +
                                  ((static_cast<int>(b1) - b2) * (static_cast<int>(b1) - b2));

      if (distanceSquared < minDistanceSquared) {
        minDistanceSquared = distanceSquared;
        closestColor       = color;
      }
    }
    return closestColor;
  }

  // Construye un mapa de reemplazo donde cada color menos frecuente tiene un color de sustitución
  std::map<std::tuple<uint16_t, uint16_t, uint16_t>, std::tuple<uint16_t, uint16_t, uint16_t>>
      Image::buildReplacementMap(
          std::pair<std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>,
                    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>> const & colors) {
    std::map<std::tuple<uint16_t, uint16_t, uint16_t>, std::tuple<uint16_t, uint16_t, uint16_t>>
        replacementMap;
    auto const & colorsToRemove  = colors.first;
    auto const & remainingColors = colors.second;

    for (auto const & colorToRemove : colorsToRemove) {
      replacementMap[colorToRemove] = findClosestColor(colorToRemove, remainingColors);
    }
    return replacementMap;
  }

  // Reemplaza los colores menos frecuentes en la imagen utilizando el mapa de reemplazo
  void Image::replaceColors(
      std::map<std::tuple<uint16_t, uint16_t, uint16_t>,
               std::tuple<uint16_t, uint16_t, uint16_t>> const & replacementMap) {
    for (unsigned long y_pos = 0; y_pos < getHeight(); ++y_pos) {
      for (unsigned long x_pos = 0; x_pos < getWidth(); ++x_pos) {
        auto color =
            std::make_tuple(getRed(x_pos, y_pos), getGreen(x_pos, y_pos), getBlue(x_pos, y_pos));
        if (replacementMap.contains(color)) {
          auto [newRed, newGreen, newBlue] = replacementMap.at(color);
          setRed(x_pos, y_pos, newRed);
          setGreen(x_pos, y_pos, newGreen);
          setBlue(x_pos, y_pos, newBlue);
        }
      }
    }
  }

  // Realiza el proceso completo de eliminación de colores menos frecuentes y reemplazo en la imagen
  void Image::cutfreq(uint32_t n) {
    auto colorFrequency                    = countColorFrequencies();
    auto sortedColors                      = sortColorsByFrequency(colorFrequency);
    auto [colorsToRemove, remainingColors] = splitColors(sortedColors, n);
    auto replacementMap = buildReplacementMap(std::make_pair(colorsToRemove, remainingColors));

    replaceColors(replacementMap);
  }
}