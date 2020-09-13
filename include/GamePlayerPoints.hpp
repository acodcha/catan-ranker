#pragma once

#include "Base.hpp"

namespace CatanLeaderboard {

/// \brief Class that contains a player and his/her points in a game.
class GamePlayerPoints {

public:

  GamePlayerPoints() noexcept {}

  GamePlayerPoints(const std::string& player, const uint_least8_t points) noexcept : player_(player), points_(points) {}

  /// \brief Constructor that takes a string containing a player and a points.
  /// \details The player and the points must be separated by whitespace. For example: "Bob 8", "9 Alice".
  GamePlayerPoints(const std::string& player_and_points) {
    const std::vector<std::string> data{split_by_whitespace(player_and_points)};
    if (data.size() != 2) {
      error("Cannot parse '" + player_and_points + "' into a player and a points.");
    }
    const std::optional<uint_least64_t> number{string_to_natural_number(data[1])};
    if (number.has_value()) {
      player_ = data[0];
      points_ = (uint_least8_t)number.value();
    } else {
      const std::optional<uint_least64_t> other_number{string_to_natural_number(data[0])};
      if (other_number.has_value()) {
        player_ = data[1];
        points_ = (uint_least8_t)other_number.value();
      } else {
        error("Cannot parse '" + player_and_points + "' into a player and a points.");
      }
    }
  }

  const std::string& player() const noexcept {
    return player_;
  }

  constexpr uint_least8_t points() const noexcept {
    return points_;
  }

  std::string print() const noexcept {
    return player_ + " " + std::to_string(points_);
  }

  bool operator==(const GamePlayerPoints& other) const noexcept {
    return player_ == other.player_ && points_ == other.points_;
  }

  bool operator!=(const GamePlayerPoints& other) const noexcept {
    return player_ != other.player_ || points_ != other.points_;
  }

protected:

  std::string player_;

  uint_least8_t points_{0};

};

class SortByDescendingPoints {

public:

  constexpr SortByDescendingPoints() noexcept {}

  bool operator()(const GamePlayerPoints& game_player_points_1, const GamePlayerPoints& game_player_points_2) const noexcept {
    if (game_player_points_1.points() > game_player_points_2.points()) {
      return true;
    } else if (game_player_points_1.points() < game_player_points_2.points()) {
      return false;
    } else {
      return game_player_points_1.player() < game_player_points_2.player();
    }
  }

};

} // namespace CatanLeaderboard
