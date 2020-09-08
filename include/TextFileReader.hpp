#pragma once

#include "FileReader.hpp"

namespace CatanLeaderboard {

class TextFileReader : public FileReader {

protected:

  TextFileReader(const std::experimental::filesystem::path& path) noexcept : FileReader(path) {
    if (stream_.is_open()) {
      std::string line;
      while (std::getline(stream_, line)) {
        lines_.push_back(line);
      }
    }
  }

  std::vector<std::string> lines_;

};

} // namespace CatanLeaderboard
