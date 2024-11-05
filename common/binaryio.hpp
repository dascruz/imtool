#pragma once

#include <istream>
#include <ostream>

namespace binaryio {
  template <typename T>
  T read_binary(std::istream & input) {
    T value;
    // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
    input.read(reinterpret_cast<char *>(&value), sizeof(value));
    return value;
  }

  template <typename T>
  void write_binary(std::ostream & output, T const & value) {
    // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
    output.write(reinterpret_cast<char const *>(&value), sizeof(value));
  }
}  // namespace binaryio
