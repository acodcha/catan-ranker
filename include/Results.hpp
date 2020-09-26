#pragma once

#include "ResultsSummaryFileWriter.hpp"

namespace CatanLeaderboard {

/// \brief Writer for all results files.
class Results {

public:

  Results(const std::experimental::filesystem::path& prefix, const Games& games, const Players& players) noexcept {
    std::experimental::filesystem::create_directory(prefix);
    if (std::experimental::filesystem::exists(prefix) && std::experimental::filesystem::is_directory(prefix)) {
      ResultsSummaryFileWriter{prefix, games, players};
    } else {
      error("Could not create the directory '" + prefix.string() + "'.");
    }
  }

};

} // namespace CatanLeaderboard
