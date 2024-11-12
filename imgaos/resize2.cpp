#include <cmath>
#include <imgaos/imageaos.hpp>
#include <vector>

namespace imageaos {
  namespace {
    float lerp(float start, float end, float tFactor) {
      return start + (tFactor * (end - start));
    }

    Pixel interpolatePixel(Pixel const & pixel0, Pixel const & pixel1, float t_factor) {
      Pixel result;
      result.red   = static_cast<unsigned short>(lerp(pixel0.red, pixel1.red, t_factor));
      result.green = static_cast<unsigned short>(lerp(pixel0.green, pixel1.green, t_factor));
      result.blue  = static_cast<unsigned short>(lerp(pixel0.blue, pixel1.blue, t_factor));
      return result;
    }
  }  // namespace

  Pixel Image::interpolate2(InterpolateArgs const & interpolate_args) const {
    auto [x, y]        = interpolate_args;
    float const x_low  = std::floor(x);
    float const x_high = std::ceil(x);
    float const y_low  = std::floor(y);
    float const y_high = std::ceil(y);

    Pixel const p00 =
        getPixel(static_cast<unsigned long>(x_low), static_cast<unsigned long>(y_low));
    Pixel const p10 =
        getPixel(static_cast<unsigned long>(x_high), static_cast<unsigned long>(y_low));
    Pixel const p01 =
        getPixel(static_cast<unsigned long>(x_low), static_cast<unsigned long>(y_high));
    Pixel const p11 =
        getPixel(static_cast<unsigned long>(x_high), static_cast<unsigned long>(y_high));

    float t_factor_x = 0.0F;
    float t_factor_y = 0.0F;

    if (x_high != x_low) { t_factor_x = (x - x_low) / (x_high - x_low); }
    if (y_high != y_low) { t_factor_y = (y - y_low) / (y_high - y_low); }

    Pixel const topPixel    = interpolatePixel(p00, p10, t_factor_x);
    Pixel const bottomPixel = interpolatePixel(p01, p11, t_factor_x);

    Pixel const result = interpolatePixel(topPixel, bottomPixel, t_factor_y);

    return result;
  }

  void Image::resize(unsigned long const new_width, unsigned long const new_height) {
    std::vector<Pixel> new_pixels(new_width * new_height);

    float const x_ratio = static_cast<float>(getWidth() - 1) / static_cast<float>(new_width - 1);
    float const y_ratio = static_cast<float>(getHeight() - 1) / static_cast<float>(new_height - 1);

    for (unsigned long new_y = 0; new_y < new_height; new_y++) {
      for (unsigned long new_x = 0; new_x < new_width; new_x++) {
        float const x_original = static_cast<float>(new_x) * x_ratio;
        float const y_original = static_cast<float>(new_y) * y_ratio;

        auto const interpolate_args             = InterpolateArgs(x_original, y_original);
        Pixel const new_pixel                   = interpolate2(interpolate_args);
        new_pixels[(new_y * new_width) + new_x] = new_pixel;
      }
    }

    setWidth(new_width);
    setHeight(new_height);
    pixels_ = new_pixels;
  }
}  // namespace imageaos