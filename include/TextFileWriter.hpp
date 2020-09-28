#pragma once

#include "FileWriter.hpp"

namespace CatanLeaderboardGenerator {

class TextFileWriter : public FileWriter {

protected:

  TextFileWriter(const std::experimental::filesystem::path& path, const std::experimental::filesystem::perms& permissions = {std::experimental::filesystem::perms::owner_read | std::experimental::filesystem::perms::owner_write | std::experimental::filesystem::perms::group_read | std::experimental::filesystem::perms::others_read}) noexcept : FileWriter(path, permissions) {}

  void line(const std::string& text) noexcept {
    if (stream_.is_open()) {
      stream_ << text << std::endl;
    }
  }

  void blank_line() noexcept {
    line("");
  }

};

} // namespace CatanLeaderboardGenerator
