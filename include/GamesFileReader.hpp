#pragma once

#include "Games.hpp"
#include "TextFileReader.hpp"

namespace CatanLeaderboard {

class GamesFileReader : public TextFileReader {

public:

  GamesFileReader(const std::experimental::filesystem::path& path) noexcept : TextFileReader(path) {
    message("Reading the games file...");
  }

  Games data() const noexcept {
    std::vector<Game> data;
    for (const std::string& line : lines_) {
      if (!remove_whitespace(line).empty()) {
        data.emplace_back(line);
      }
    }
    std::sort(data.begin(), data.end(), Game::sort_by_ascending_date());
    return {data};
  }

};

} // namespace CatanLeaderboard
