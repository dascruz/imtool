// resize_soa.cpp
#include "imagesoa.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>


namespace imagesoa {

    namespace {
        // Función auxiliar para interpolar valores
        inline double Interpolate(double value1, double value2, double weight) {
            return (value1 * (1.0 - weight)) + (value2 * weight);
        }

        InterpolationCoords calculateCoords(double x_real, double y_real) {
            auto x_l = static_cast<std::uint32_t>(std::floor(x_real));
            auto y_l = static_cast<std::uint32_t>(std::floor(y_real));
            auto x_h = static_cast<std::uint32_t>(std::ceil(x_real));
            auto y_h = static_cast<std::uint32_t>(std::ceil(y_real));
            double const x_weight = x_real - x_l;
            double const y_weight = y_real - y_l;
            return {.x_l=x_l, .y_l=y_l, .x_h=x_h, .y_h=y_h, .x_weight=x_weight, .y_weight=y_weight};
        }

        struct InterpolationData {
            unsigned short ll, hl, lh, hh;
            double x_weight, y_weight;
        };

        auto interpolateColorComponent(const InterpolationData& data, unsigned short max_value) -> unsigned short {
            double const low = Interpolate(data.ll, data.hl, data.x_weight);
            double const high = Interpolate(data.lh, data.hh, data.x_weight);
            double const color = Interpolate(low, high, data.y_weight);
            return static_cast<unsigned short>(std::clamp(std::round(color), 0.0, static_cast<double>(max_value)));
        }
    }

  unsigned short Image::calculatePixelColor(const InterpolationCoords& coords, char channel) const {
      InterpolationData data{};

      if (channel == 'r') {
        data.ll = getRed(coords.x_l, coords.y_l);
        data.hl = getRed(coords.x_h, coords.y_l);
        data.lh = getRed(coords.x_l, coords.y_h);
        data.hh = getRed(coords.x_h, coords.y_h);
      } else if (channel == 'g') {
        data.ll = getGreen(coords.x_l, coords.y_l);
        data.hl = getGreen(coords.x_h, coords.y_l);
        data.lh = getGreen(coords.x_l, coords.y_h);
        data.hh = getGreen(coords.x_h, coords.y_h);
      } else {
        data.ll = getBlue(coords.x_l, coords.y_l);
        data.hl = getBlue(coords.x_h, coords.y_l);
        data.lh = getBlue(coords.x_l, coords.y_h);
        data.hh = getBlue(coords.x_h, coords.y_h);
      }

      data.x_weight = coords.x_weight;
      data.y_weight = coords.y_weight;

      return interpolateColorComponent(data, getMaxColorValue());
    }

    void Image::resize(unsigned long new_width, unsigned long new_height) {
        std::vector<unsigned short> new_red(new_width * new_height);
        std::vector<unsigned short> new_green(new_width * new_height);
        std::vector<unsigned short> new_blue(new_width * new_height);

        double const x_ratio = (new_width > 1) ? static_cast<double>(getWidth() - 1) / static_cast<double>(new_width - 1) : 0.0;
        double const y_ratio = (new_height > 1) ? static_cast<double>(getHeight() - 1) / static_cast<double>(new_height - 1) : 0.0;

        for (std::uint32_t y_prime = 0; y_prime < new_height; ++y_prime) {
            for (std::uint32_t x_prime = 0; x_prime < new_width; ++x_prime) {
                double const x_real = x_prime * x_ratio;
                double const y_real = y_prime * y_ratio;

                InterpolationCoords const coords = calculateCoords(x_real, y_real);

                new_red[(y_prime * new_width) + x_prime] = calculatePixelColor(coords, 'r');
                new_green[(y_prime * new_width) + x_prime] = calculatePixelColor(coords, 'g');
                new_blue[(y_prime * new_width) + x_prime] = calculatePixelColor(coords, 'b');
            }
        }

        red_ = std::move(new_red);
        green_ = std::move(new_green);
        blue_ = std::move(new_blue);
        setWidth(new_width);
        setHeight(new_height);
    }

}  // namespace imagesoa
