#pragma once

#include "Games.hpp"
#include "PlayerProperties.hpp"

namespace CatanLeaderboard {

class Player {

public:

  Player() noexcept {}

  Player(const std::string& name) noexcept : name_(name) {}

  Player(const std::string& name, const Games& games) noexcept : name_(name) {
    for (const Game& game : games.data()) {
      if (game.participant(name_)) {
        if (history_.empty()) {
          history_.emplace_back(name_, game);
        } else {
          history_.emplace_back(name_, game, history_.back());
        }
      }
    }
  }

  const std::string& name() const noexcept {
    return name_;
  }

  bool operator==(const Player& other) const noexcept {
    return name_ == other.name_;
  }

  bool operator!=(const Player& other) const noexcept {
    return name_ != other.name_;
  }

  struct sort_by_alphabetical_name {
    bool operator()(const Player& player_1, const Player& player_2) const noexcept {
      return player_1.name() < player_2.name();
    }
  };

protected:

  std::string name_;

  std::vector<PlayerProperties> history_;

};

} // namespace CatanLeaderboard
