#pragma once

#include "ResultsSummaryFileWriter.hpp"

namespace CatanLeaderboardGenerator {

/// \brief Writer for all results files.
class Results {

public:

  Results(const std::experimental::filesystem::path& directory, const Games& games, const Players& players) noexcept {
    if (!directory.empty()) {
      std::experimental::filesystem::create_directory(directory);
      if (std::experimental::filesystem::exists(directory) && std::experimental::filesystem::is_directory(directory)) {
        ResultsSummaryFileWriter{directory / "README.md", games, players};
      } else {
        error("Could not create the directory '" + directory.string() + "'.");
      }
    }
  }

};

} // namespace CatanLeaderboardGenerator
