#pragma once

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
    std::experimental::filesystem::path png_file_path{path_};
    png_file_path.replace_extension(".png");
    return png_file_path;
  }

};

} // namespace CatanLeaderboardGenerator
