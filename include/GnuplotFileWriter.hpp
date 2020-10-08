#pragma once

#include "Path.hpp"
#include "TextFileWriter.hpp"

namespace CatanLeaderboardGenerator {

class GnuplotFileWriter : public TextFileWriter {

public:

  GnuplotFileWriter(const std::experimental::filesystem::path& path) noexcept : TextFileWriter(path) {}

  ~GnuplotFileWriter() noexcept {
    blank_line();
  }

protected:

  std::experimental::filesystem::path png_file_path() const noexcept {
    return Path::gnuplot_path_to_png_path(path_);
  }

  uint_least64_t nearest_higher_nice_number(const double value, const uint_least64_t increment) const noexcept {
    return increment * (uint_least64_t)std::ceil(value / increment);
  }

  uint_least64_t nearest_lower_nice_number(const double value, const uint_least64_t increment) const noexcept {
    return increment * (uint_least64_t)std::floor(value / increment);
  }

};

} // namespace CatanLeaderboardGenerator
