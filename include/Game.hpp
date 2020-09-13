#pragma once

#include "Date.hpp"
#include "GamePlayerPoints.hpp"

namespace CatanLeaderboard {

class Game {

public:

  Game() noexcept {}

  Game(const Date& date, const std::set<GamePlayerPoints, SortByDescendingPoints>& players_and_points) noexcept : date_(date), players_and_points_(players_and_points) {}

  constexpr const Date& date() const noexcept {
    return date_;
  }

  constexpr const std::set<GamePlayerPoints, SortByDescendingPoints>& players_and_points() const noexcept {
    return players_and_points_;
  }

  std::string print() const noexcept {
    std::string text{date_.print() + " : "};
    std::size_t counter{0};
    for (const GamePlayerPoints& player_and_points : players_and_points_) {
      text.append(player_and_points.print());
      if (counter + 1 < players_and_points_.size()) {
        text.append(" , ");
      }
      ++counter;
    }
    return text;
  }

  bool operator==(const Game& other) const noexcept {
    return date_ == other.date_ && players_and_points_ == other.players_and_points_;
  }

  bool operator!=(const Game& other) const noexcept {
    return date_ != other.date_ || players_and_points_ != other.players_and_points_;
  }

protected:

  Date date_;

  std::set<GamePlayerPoints, SortByDescendingPoints> players_and_points_;

};

class SortByAscendingDate {

public:

  constexpr SortByAscendingDate() noexcept {}

  bool operator()(const Game& game_1, const Game& game_2) const noexcept {
    return game_1.date() < game_2.date();
  }

};

} // namespace CatanLeaderboard
