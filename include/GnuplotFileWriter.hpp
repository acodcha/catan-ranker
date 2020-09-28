#pragma once

#include "TextFileWriter.hpp"

namespace CatanLeaderboardGenerator {

class GnuplotFileWriter : public TextFileWriter {

public:

  GnuplotFileWriter(const std::experimental::filesystem::path& path, const std::string& title, const std::string& x_axis_title, const std::string& y_axis_title) noexcept : TextFileWriter(path) {
    if (!title.empty()) {
      line("set title \"" + title + "\"");
    }
    if (!x_axis_title.empty()) {
      line("set xlabel \"" + x_axis_title + "\"");
    }
    if (!y_axis_title.empty()) {
      line("set ylabel \"" + y_axis_title + "\"");
    }
  }

  ~GnuplotFileWriter() noexcept {
    blank_line();
  }

protected:

  std::experimental::filesystem::path data_table_file_path() const noexcept {
    std::experimental::filesystem::path new_path{path_};
    new_path.replace_extension(".dat");
    return new_path;
  }

  std::experimental::filesystem::path png_file_path() const noexcept {
    std::experimental::filesystem::path new_path{path_};
    new_path.replace_extension(".png");
    return new_path;
  }

};

} // namespace CatanLeaderboardGenerator
