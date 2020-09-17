#pragma once

#include "Player.hpp"

namespace CatanLeaderboard {

class Players {

public:

  Players(const Games& games) noexcept {
    std::set<std::string> games_player_names;
    for (const Game& game : games.data()) {
      const std::set<std::string> game_player_names{game.player_names()};
      games_player_names.insert(game_player_names.cbegin(), game_player_names.cend());
    }
    for (const std::string& player_name : games_player_names) {
      data_.emplace(player_name, games);
    }
  }

  const std::set<Player, Player::sort_by_alphabetical_name>& data() const noexcept {
    return data_;
  }

  std::string print() const noexcept {
    std::stringstream stream;
    stream << "There are " << data_.size() << " players:" << std::endl;
    uint_least64_t index{0};
    for (const Player& player : data_) {
      stream << "- " << std::to_string(index + 1) << ": " << player.name();
      if (index + 1 < data_.size()) {
        stream << std::endl;
      }
      ++index;
    }
    return stream.str();
  }

  void message() const noexcept {
    CatanLeaderboard::message(print());
  }

protected:

  std::set<Player, Player::sort_by_alphabetical_name> data_;

};

} // namespace CatanLeaderboard
