#include <common/image.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace image {
  bool Image::readHeader(std::ifstream & file) {
    std::string line;
    if (!std::getline(file, line) || line != "P6") {
      std::cerr << "Unsupported file format: " << line << '\n';
      return false;
    }

    while (std::getline(file, line)) {
      if (line[0] == '#') { continue; }

      std::istringstream sizeStream(line);
      sizeStream >> width_ >> height_;
      break;
    }

    file >> maxColorValue_;
    file.ignore();

    if (maxColorValue_ < MIN_COLOR_VALUE || maxColorValue_ > MAX_COLOR_VALUE_16BIT) {
      std::cerr << "Unsupported max color value: " << maxColorValue_ << " (must be between "
                << MIN_COLOR_VALUE << " and " << MAX_COLOR_VALUE_16BIT << ").\n";
      return false;
    }

    return true;
  }

  bool Image::writeHeader(std::ofstream & file) const {
    file << "P6\n" << width_ << " " << height_ << "\n" << maxColorValue_ << "\n";
    return file.good();
  }

  bool Image::writeHeaderCompress(std::ofstream & file, unsigned long const colorTableSize) const {
    file << "C6 " << width_ << " " << height_ << " " << maxColorValue_ << " " << colorTableSize
         << "\n";
    return file.good();
  }

}  // namespace image
