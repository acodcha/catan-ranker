#pragma once

#include "Path.hpp"
#include "TextFileWriter.hpp"

namespace CatanRanker {

class GnuplotFileWriter : public TextFileWriter {
public:
  GnuplotFileWriter(const std::experimental::filesystem::path& path) noexcept
    : TextFileWriter(path) {
    line("set terminal pngcairo size " + std::to_string(width_pixels_) + ","
         + std::to_string(height_pixels_) + " enhanced font \"Verdana,10\"");
    line("set output \"" + png_file_path().string() + "\"");
  }

  ~GnuplotFileWriter() noexcept {
    blank_line();
  }

protected:
  std::experimental::filesystem::path png_file_path() const noexcept {
    return Path::gnuplot_path_to_png_path(path_);
  }

  int64_t nearest_higher_nice_number(
      const double value, const int64_t increment) const noexcept {
    return increment * static_cast<int64_t>(std::ceil(value / increment));
  }

  int64_t nearest_lower_nice_number(
      const double value, const int64_t increment) const noexcept {
    return increment * static_cast<int64_t>(std::floor(value / increment));
  }

  const int64_t width_pixels_{1000};

  const int64_t height_pixels_{750};
};

}  // namespace CatanRanker
