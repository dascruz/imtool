#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <imgaos/imageaos.hpp>
#include <limits>
#include <map>
#include <tuple>
#include <utility>
#include <vector>

namespace imageaos {
  // Función para contar la frecuencia de colores
  std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int> Image::countColorFrequencies() const {
    std::map<std::tuple<uint16_t, uint16_t, uint16_t>, int> colorFrequency;
    for (Pixel const & pixel : pixels_) {
      auto color = std::make_tuple(pixel.red, pixel.green, pixel.blue);
      colorFrequency[color]++;
    }
    return colorFrequency;
  }

  // Función que crea un vector de colores ordenado por frecuencia
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

  // Función para separar los colores en dos grupos: los colores menos frecuentes y los que deben
  // permanecer.
  std::pair<std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>,
            std::vector<std::tuple<uint16_t, uint16_t, uint16_t>>>
      Image::splitColors(
          std::vector<std::pair<std::tuple<uint16_t, uint16_t, uint16_t>, int>> const &
              colorFreqVector,
          std::uint32_t n) {
    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> colorsToRemove;
    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> remainingColors;

    for (std::size_t i = 0; i < colorFreqVector.size(); ++i) {
      if (i < static_cast<std::size_t>(n)) {
        colorsToRemove.push_back(colorFreqVector[i].first);
      } else {
        remainingColors.push_back(colorFreqVector[i].first);
      }
    }
    return {colorsToRemove, remainingColors};
  }

  // Función para buscar el color más cercano en una lista de colores, utilizando distancia
  // euclidiana al cuadrado
  std::tuple<uint16_t, uint16_t, uint16_t> Image::findClosestColor(
      std::tuple<uint16_t, uint16_t, uint16_t> const & colorToRemove,
      std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> const & remainingColors) {
    auto [r1, g1, b1]      = colorToRemove;
    int minDistanceSquared = std::numeric_limits<int>::max();
    std::tuple<uint16_t, uint16_t, uint16_t> closestColor;

    for (auto const & color : remainingColors) {
      auto [r2, g2, b2]         = color;
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

  // Función que construye el mapa de reemplazo
  std::map<std::tuple<uint16_t, uint16_t, uint16_t>, std::tuple<uint16_t, uint16_t, uint16_t>>
      Image::buildReplacementMap(
          std::pair<std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> const &,
                    std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> const &> const & colors) {
    std::map<std::tuple<uint16_t, uint16_t, uint16_t>, std::tuple<uint16_t, uint16_t, uint16_t>>
        replacementMap;

    auto const & colorsToRemove  = colors.first;
    auto const & remainingColors = colors.second;

    for (auto const & colorToRemove : colorsToRemove) {
      replacementMap[colorToRemove] = findClosestColor(colorToRemove, remainingColors);
    }
    return replacementMap;
  }

  // Función para sustituir los colores con menor frecuencia
  void Image::replaceColors(
      std::map<std::tuple<uint16_t, uint16_t, uint16_t>,
               std::tuple<uint16_t, uint16_t, uint16_t>> const & replacementMap) {
    for (Pixel & pixel : pixels_) {
      auto color = std::make_tuple(pixel.red, pixel.green, pixel.blue);
      if (replacementMap.contains(color)) {
        auto [newRed, newGreen, newBlue] = replacementMap.at(color);
        pixel.red                        = newRed;
        pixel.green                      = newGreen;
        pixel.blue                       = newBlue;
      }
    }
  }

  // Función cutfreq
  void Image::cutfreq(std::uint32_t n) {
    auto colorFrequency                    = countColorFrequencies();
    auto sortedColors                      = sortColorsByFrequency(colorFrequency);
    auto [colorsToRemove, remainingColors] = splitColors(sortedColors, n);
    auto replacementMap = buildReplacementMap(std::make_pair(colorsToRemove, remainingColors));

    replaceColors(replacementMap);
  }
}