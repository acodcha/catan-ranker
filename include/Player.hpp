#pragma once

#include "Games.hpp"
#include "PlayerProperties.hpp"

namespace CatanLeaderboard {

class Player {

public:

  Player() noexcept {}

  Player(const PlayerName& name) noexcept : name_(name) {}

  Player(const PlayerName& name, const Games& games) noexcept : name_(name) {
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

  const PlayerName& name() const noexcept {
    return name_;
  }

  std::string print() const noexcept {
    return name_.value() + " : " + std::to_string(history_.back().game_number()) + " games , " + real_number_to_string(history_.back().average_points_per_game()) + " points/game , " + std::to_string((uint_least8_t)std::round((history_.back().place_ratio({1}) * 100))) + "% win rate , " + std::to_string(history_.back().place_count({1})) + " 1st places , " + std::to_string(history_.back().place_count({2})) + " 2nd places , " + std::to_string(history_.back().place_count({3})) + " 3rd places";
  }

  bool operator==(const Player& other) const noexcept {
    return name_ == other.name_;
  }

  bool operator!=(const Player& other) const noexcept {
    return name_ != other.name_;
  }

  struct sort_by_alphabetical_name {
    bool operator()(const Player& player_1, const Player& player_2) const noexcept {
      return PlayerName::sort_alphabetical()(player_1.name(), player_2.name());
    }
  };

  struct hash {
    std::size_t operator()(const Player& player) const {
      return PlayerName::hash()(player.name());
    }
  };

protected:

  PlayerName name_;

  std::vector<PlayerProperties> history_;

};

} // namespace CatanLeaderboard
