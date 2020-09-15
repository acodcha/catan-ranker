#pragma once

#include "Game.hpp"

namespace CatanLeaderboard {

class Games {

public:

  Games() noexcept {}

  Games(const std::vector<Game>& games_data) noexcept : games_data_(games_data) {
    initialize_player_names();
    message(print());
  }

  constexpr const std::vector<Game>& games_data() const noexcept {
    return games_data_;
  }

  constexpr const std::set<std::string>& player_names() const noexcept {
    return player_names_;
  }

  std::string print() const noexcept {
    if (games_data_.empty()) {
      return "The games file is empty.";
    }
    std::stringstream stream;
    stream << "There are " << games_data_.size() << " games:" << std::endl;
    uint_least64_t index{0};
    for (const Game& game : games_data_) {
      stream << "- " << std::to_string(index + 1) << ": " << game.print() << std::endl;
      ++index;
    }
    stream << "There are " << player_names_.size() << " players:" << std::endl;
    index = 0;
    for (const std::string& player_name : player_names_) {
      stream << "- " << std::to_string(index + 1) << ": " << player_name;
      if (index + 1 < player_names_.size()) {
        stream << std::endl;
      }
      ++index;
    }
    return stream.str();
  }

protected:

  std::vector<Game> games_data_;

  std::set<std::string> player_names_;

  void initialize_player_names() noexcept {
    for (const Game& game : games_data_) {
      const std::set<std::string> player_names{game.player_names()};
      player_names_.insert(player_names.cbegin(), player_names.cend());
    }
  }

};

} // namespace CatanLeaderboard
